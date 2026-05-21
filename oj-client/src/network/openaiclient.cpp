#include "network/openaiclient.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <memory>

namespace
{
struct StreamState
{
    QByteArray buffer;
    bool completed = false;
    bool sawStreamEvent = false;
};

int findEventDelimiter(const QByteArray &buffer)
{
    const int lfLf = buffer.indexOf("\n\n");
    const int crlfCrlf = buffer.indexOf("\r\n\r\n");
    if (lfLf < 0) {
        return crlfCrlf;
    }
    if (crlfCrlf < 0) {
        return lfLf;
    }
    return qMin(lfLf, crlfCrlf);
}

int delimiterLength(const QByteArray &buffer, int index)
{
    if (index < 0 || index >= buffer.size()) {
        return 0;
    }
    if (buffer.mid(index, 4) == "\r\n\r\n") {
        return 4;
    }
    return 2;
}

QString eventNameFromBlock(const QByteArray &block)
{
    const QList<QByteArray> lines = block.split('\n');
    for (QByteArray line : lines) {
        line = line.trimmed();
        if (line.startsWith("event:")) {
            return QString::fromUtf8(line.mid(6).trimmed());
        }
    }
    return QString();
}

QByteArray dataFromBlock(const QByteArray &block)
{
    const QList<QByteArray> lines = block.split('\n');
    QByteArray data;
    for (QByteArray line : lines) {
        line = line.trimmed();
        if (!line.startsWith("data:")) {
            continue;
        }
        if (!data.isEmpty()) {
            data += '\n';
        }
        data += line.mid(5).trimmed();
    }
    return data;
}

QUrl responsesUrl(const QUrl &baseUrl)
{
    QUrl url(baseUrl);
    QString path = url.path();
    if (path.isEmpty()) {
        path = "/";
    }
    if (!path.endsWith('/')) {
        path += '/';
    }
    path += "responses";
    url.setPath(path);
    return url;
}

bool processEventBlock(const QByteArray &block,
                       const std::shared_ptr<StreamState> &state,
                       OpenAiClient *client)
{
    const QByteArray trimmedBlock = block.trimmed();
    if (trimmedBlock.isEmpty()) {
        return false;
    }

    const QByteArray data = dataFromBlock(trimmedBlock);
    if (data.isEmpty()) {
        return false;
    }

    state->sawStreamEvent = true;
    if (data == "[DONE]") {
        state->completed = true;
        return true;
    }

    const QString eventName = eventNameFromBlock(trimmedBlock);
    const QJsonDocument document = QJsonDocument::fromJson(data);
    const QJsonObject object = document.isObject() ? document.object() : QJsonObject();

    if (eventName == "response.output_text.delta") {
        const QString delta = object.value("delta").toString();
        if (!delta.isEmpty()) {
            emit client->responseDelta(delta);
        }
        return false;
    }

    if (eventName == "response.completed" || eventName == "response.done") {
        state->completed = true;
        emit client->responseCompleted(object);
        return true;
    }

    return false;
}
}

OpenAiClient::OpenAiClient(QObject *parent)
    : QObject(parent)
{
}

void OpenAiClient::setBaseUrl(const QUrl &baseUrl)
{
    if (baseUrl.isValid() && !baseUrl.scheme().isEmpty()) {
        m_baseUrl = baseUrl;
    }
}

void OpenAiClient::createResponseStream(const QString &apiKey,
                                        const QJsonObject &payload)
{
    QUrl url = responsesUrl(m_baseUrl);
    QNetworkRequest request(url);
    request.setTransferTimeout(30000);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/json; charset=utf-8");
    request.setRawHeader("Accept", "text/event-stream");
    request.setRawHeader("Cache-Control", "no-cache");
    request.setRawHeader("Authorization",
                         "Bearer " + apiKey.trimmed().toUtf8());

    QJsonObject requestPayload = payload;
    requestPayload["stream"] = true;

    QNetworkReply *reply = m_manager.post(
        request,
        QJsonDocument(requestPayload).toJson(QJsonDocument::Compact));

    auto state = std::make_shared<StreamState>();
    connect(reply, &QNetworkReply::readyRead, this, [this, reply, state]() {
        state->buffer += reply->readAll();
        while (true) {
            const int delimiterIndex = findEventDelimiter(state->buffer);
            if (delimiterIndex < 0) {
                break;
            }

            const int removedLength =
                delimiterIndex + delimiterLength(state->buffer, delimiterIndex);
            const QByteArray block = state->buffer.left(delimiterIndex);
            state->buffer.remove(0, removedLength);
            if (processEventBlock(block, state, this)) {
                break;
            }
        }
    });

    connect(reply, &QNetworkReply::finished, this, [this, reply, state]() {
        const QByteArray body = reply->readAll();
        if (reply->error() != QNetworkReply::NoError) {
            QString message =
                QString("OpenAI request failed.\nHTTP status: %1\nEndpoint: %2\n%3")
                    .arg(QString::number(reply->attribute(
                                             QNetworkRequest::HttpStatusCodeAttribute)
                                             .toInt()),
                         reply->url().toString(),
                         reply->errorString());
            const QJsonDocument errorDocument = QJsonDocument::fromJson(body);
            if (errorDocument.isObject()) {
                const QString apiMessage = errorDocument.object()
                                               .value("error")
                                               .toObject()
                                               .value("message")
                                               .toString();
                if (!apiMessage.isEmpty()) {
                    message += "\n" + apiMessage;
                }
            }
            if (!body.trimmed().isEmpty()) {
                message += "\n\nRaw body:\n" + QString::fromUtf8(body);
            }
            emit requestFailed(message);
            reply->deleteLater();
            return;
        }

        if (!state->completed && !state->buffer.trimmed().isEmpty()) {
            const QByteArray remaining = state->buffer.trimmed();
            const QList<QByteArray> blocks = remaining.split('\n');
            for (const QByteArray &block : blocks) {
                if (processEventBlock(block, state, this)) {
                    break;
                }
            }
        }

        if (!state->completed) {
            const QByteArray remaining = state->buffer.trimmed();
            const QByteArray candidateBody = !body.trimmed().isEmpty() ? body.trimmed()
                                                                       : remaining;
            const QJsonDocument document = QJsonDocument::fromJson(candidateBody);
            if (document.isObject()) {
                state->completed = true;
                emit responseCompleted(document.object());
            } else if (!state->sawStreamEvent) {
                QString message =
                    QString("OpenAI response could not be parsed.\nEndpoint: %1")
                        .arg(reply->url().toString());
                if (!candidateBody.isEmpty()) {
                    message += "\n\nRaw body:\n" + QString::fromUtf8(candidateBody);
                }
                emit requestFailed(message);
            }
        }

        reply->deleteLater();
    });
}
