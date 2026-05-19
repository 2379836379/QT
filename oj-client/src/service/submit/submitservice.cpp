#include "submitservice.h"

#include "repository/submit/submitrepository.h"

SubmitService::SubmitService(SubmitRepository *repository, QObject *parent)
    : QObject(parent)
    , m_repository(repository)
{
    connect(
        m_repository,
        &SubmitRepository::submitPageFetched,
        this,
        [this](const SubmitPageInfo &submitPageInfo) {
            m_currentSubmitPageInfo = submitPageInfo;
            m_loading = false;
            emit loadingChanged(false);
            emit submitPageLoaded(submitPageInfo);
        });

    connect(
        m_repository,
        &SubmitRepository::solutionSubmitted,
        this,
        [this](const NetworkResult &result) {
            m_submitting = false;
            emit submittingChanged(false);
            emit solutionSubmitted(result);
        });

    connect(
        m_repository,
        &SubmitRepository::requestFailed,
        this,
        [this](const QString &stage, const QString &message) {
            if (stage == "submitPage") {
                m_loading = false;
                emit loadingChanged(false);
            } else if (stage == "submitSolution") {
                m_submitting = false;
                emit submittingChanged(false);
            }
            emit failed(message);
        });
}

void SubmitService::openSubmit(const QUrl &submitPageUrl)
{
    m_currentSubmitUrl = submitPageUrl;
    m_loading = true;
    emit loadingChanged(true);
    m_repository->fetchSubmitPage(submitPageUrl);
}

void SubmitService::submitSolution(const QString &language,
                                   const QString &sourceText)
{
    if (m_currentSubmitPageInfo.submitActionUrl.isEmpty()) {
        emit failed("submit action url not found");
        return;
    }

    const QByteArray payload = SubmitParser::buildSubmitPayload(
        m_currentSubmitPageInfo, language, sourceText);
    emit submitPayloadBuilt(language, sourceText, payload);
    m_submitting = true;
    emit submittingChanged(true);
    m_repository->submitSolution(
        QUrl(m_currentSubmitPageInfo.submitActionUrl),
        payload,
        QUrl(m_currentSubmitPageInfo.pageUrl));
}

QUrl SubmitService::currentSubmitUrl() const
{
    return m_currentSubmitUrl;
}

SubmitPageInfo SubmitService::currentSubmitPageInfo() const
{
    return m_currentSubmitPageInfo;
}

QString SubmitService::defaultLanguage() const
{
    return SubmitParser::defaultLanguage(m_currentSubmitPageInfo);
}
