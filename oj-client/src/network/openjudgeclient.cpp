#include "openjudgeclient.h"
#include "cookiestore.h"

#include <QDebug>

OpenJudgeClient::OpenJudgeClient(QObject *parent)
    : QObject(parent)
{
}
void OpenJudgeClient::setBaseUrl(const QString &baseUrl)
{
    m_baseUrl = baseUrl;
}

void OpenJudgeClient::setCookieStore(CookieStore *cookieStore)
{
    m_cookieStore = cookieStore;
    if (m_cookieStore) {
        m_manager.setCookieJar(m_cookieStore);
        if (!m_cookieStore->parent()) {
            m_cookieStore->setParent(this);
        }
    }
}

void OpenJudgeClient::logout()
{
    if (m_cookieStore != nullptr) {
        m_cookieStore->clearCookies();
    }
}

void OpenJudgeClient::login(const QString &username, const QString &password)
{
    QUrl url(m_baseUrl + "/api/auth/login/");
    QNetworkRequest request = createRequest(url);

    QUrlQuery form;
    form.addQueryItem("email", username);
    form.addQueryItem("password", password);
    form.addQueryItem("redirectUrl", m_baseUrl);

    QNetworkReply *reply = m_manager.post(
        request,
        form.toString(QUrl::FullyEncoded).toUtf8());

    connectReply(reply, [this](const NetworkResult &result) {
        emit loginFinished(result);
    });
}

void OpenJudgeClient::fetchHomePage()
{
    const QUrl url(m_baseUrl);
    QNetworkReply *reply = m_manager.get(createRequest(url));
    connectReply(reply, [this](const NetworkResult &result) {
        emit homePageFinished(result);
    });
}

void OpenJudgeClient::fetchUserHomePage(const QUrl &url)
{
    QNetworkReply *reply = m_manager.get(createRequest(url));
    connectReply(reply, [this](const NetworkResult &result) {
        emit userHomePageFinished(result);
    });
}

void OpenJudgeClient::fetchClassPage(const QUrl &url)
{
    QNetworkReply *reply = m_manager.get(createRequest(url));
    connectReply(reply, [this](const NetworkResult &result) {
        emit classPageFinished(result);
    });
}

void OpenJudgeClient::fetchGroupPage(const QUrl &url)
{
    QNetworkReply *reply = m_manager.get(createRequest(url));
    connectReply(reply, [this](const NetworkResult &result) {
        emit groupPageFinished(result);
    });
}

void OpenJudgeClient::fetchContestPage(const QUrl &url)
{
    QNetworkReply *reply = m_manager.get(createRequest(url));
    connectReply(reply, [this](const NetworkResult &result) {
        emit contestPageFinished(result);
    });
}

void OpenJudgeClient::fetchProblemPage(const QUrl &url)
{
    QNetworkReply *reply = m_manager.get(createRequest(url));
    connectReply(reply, [this](const NetworkResult &result) {
        emit problemPageFinished(result);
    });
}

void OpenJudgeClient::fetchResultPage(const QUrl &url)
{
    QNetworkReply *reply = m_manager.get(createRequest(url));
    connectReply(reply, [this](const NetworkResult &result) {
        emit resultPageFinished(result);
    });
}

void OpenJudgeClient::fetchSubmitPage(const QUrl &url)
{
    QNetworkReply *reply = m_manager.get(createRequest(url));
    connectReply(reply, [this](const NetworkResult &result) {
        emit submitPageFinished(result);
    });
}

void OpenJudgeClient::submitSolution(const QUrl &submitActionUrl,
                                     const QByteArray &payload,
                                     const QUrl &refererUrl)
{
    QNetworkRequest request = createRequest(submitActionUrl, refererUrl);
    request.setRawHeader("X-Requested-With", "XMLHttpRequest");
    QNetworkReply *reply = m_manager.post(request, payload);
    connectReply(reply, [this](const NetworkResult &result) {
        emit solutionSubmitted(result);
    });
}

void OpenJudgeClient::fetchProblemList(int page)
{
    QUrl url(m_baseUrl + "/problem");
    QUrlQuery query;
    query.addQueryItem("page", QString::number(page));
    url.setQuery(query);

    QNetworkReply *reply = m_manager.get(createRequest(url));
    connectReply(reply, [this](const NetworkResult &result) {
        emit problemListFinished(result);
    });
}

void OpenJudgeClient::fetchProblemDetail(const QString &problemId)
{
    const QUrl url(m_baseUrl + "/problem/" + problemId);
    QNetworkReply *reply = m_manager.get(createRequest(url));
    connectReply(reply, [this](const NetworkResult &result) {
        emit problemDetailFinished(result);
    });
}

void OpenJudgeClient::fetchSubmissionList(int page)
{
    QUrl url(m_baseUrl + "/status");
    QUrlQuery query;
    query.addQueryItem("page", QString::number(page));
    url.setQuery(query);

    QNetworkReply *reply = m_manager.get(createRequest(url));
    connectReply(reply, [this](const NetworkResult &result) {
        emit submissionListFinished(result);
    });
}

QNetworkRequest OpenJudgeClient::createRequest(const QUrl &url,
                                               const QUrl &refererUrl) const
{
    QNetworkRequest request(url);

    request.setHeader(
        QNetworkRequest::UserAgentHeader,
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 "
        "(KHTML, like Gecko) Chrome/124.0.0.0 Safari/537.36");
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/x-www-form-urlencoded");
    request.setRawHeader(
        "Accept",
        "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    request.setRawHeader(
        "Referer",
        (refererUrl.isEmpty() ? m_baseUrl : refererUrl.toString()).toUtf8());
    request.setRawHeader("Connection", "keep-alive");

    return request;
}

void OpenJudgeClient::connectReply(
    QNetworkReply *reply,
    const std::function<void(const NetworkResult &)> &callback)
{
    connect(reply, &QNetworkReply::finished, this, [reply, callback]() {
        NetworkResult result;
        result.statusCode =
            reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        result.networkError = reply->error();
        result.errorString = reply->errorString();
        result.requestUrl = reply->request().url();
        result.finalUrl = reply->url();
        result.body = reply->readAll();
        result.ok = (result.networkError == QNetworkReply::NoError);

        if (result.ok) {
            qDebug() << "network ok, status =" << result.statusCode
                     << "url =" << result.finalUrl;
        } else {
            qDebug() << "network failed:" << result.errorString
                     << "status =" << result.statusCode
                     << "url =" << result.requestUrl;
        }

        callback(result);
        reply->deleteLater();
    });
}
