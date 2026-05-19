#include "submitrepository.h"

#include "network/openjudgeclient.h"

SubmitRepository::SubmitRepository(OpenJudgeClient *client, QObject *parent)
    : QObject(parent)
    , m_client(client)
{
    connect(
        m_client,
        &OpenJudgeClient::submitPageFinished,
        this,
        [this](const NetworkResult &result) {
            if (!m_expectedSubmitPageUrl.isEmpty()
                && result.finalUrl != m_expectedSubmitPageUrl) {
                return;
            }

            m_expectedSubmitPageUrl = QUrl();
            if (!result.ok) {
                emit requestFailed("submitPage", result.errorString);
                return;
            }

            emit submitPageFetched(
                SubmitParser::parseSubmitPage(result.body, result.finalUrl));
        });

    connect(
        m_client,
        &OpenJudgeClient::solutionSubmitted,
        this,
        [this](const NetworkResult &result) {
            m_expectedSubmitActionUrl = QUrl();
            if (!result.ok) {
                emit requestFailed("submitSolution", result.errorString);
                return;
            }

            emit solutionSubmitted(result);
        });
}

void SubmitRepository::fetchSubmitPage(const QUrl &submitPageUrl)
{
    m_expectedSubmitPageUrl = submitPageUrl;
    m_client->fetchSubmitPage(submitPageUrl);
}

void SubmitRepository::submitSolution(const QUrl &submitActionUrl,
                                      const QByteArray &payload,
                                      const QUrl &refererUrl)
{
    m_expectedSubmitActionUrl = submitActionUrl;
    m_client->submitSolution(submitActionUrl, payload, refererUrl);
}
