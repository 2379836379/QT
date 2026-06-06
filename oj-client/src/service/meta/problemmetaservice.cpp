#include "service/meta/problemmetaservice.h"

ProblemMetaService::ProblemMetaService(ProblemMetaRepository *repository,
                                       QObject *parent)
    : QObject(parent)
    , m_repository(repository)
{
}

bool ProblemMetaService::initialize()
{
    if (m_repository == nullptr) {
        emit failed("Problem meta repository not available");
        return false;
    }

    if (!m_repository->initialize()) {
        emit failed(m_repository->lastError());
        return false;
    }
    return true;
}

bool ProblemMetaService::saveMeta(const ProblemMeta &meta)
{
    if (m_repository == nullptr) {
        emit failed("Problem meta repository not available");
        return false;
    }

    if (!m_repository->upsertMeta(meta)) {
        emit failed(m_repository->lastError());
        return false;
    }
    if (!m_repository->setTags(meta.problemUrl, meta.tags)) {
        emit failed(m_repository->lastError());
        return false;
    }

    emit metaSaved(meta);
    return true;
}

bool ProblemMetaService::loadMeta(const QString &problemUrl, ProblemMeta *meta) const
{
    if (m_repository == nullptr) {
        return false;
    }
    return m_repository->loadMeta(problemUrl, meta);
}

QList<ProblemMeta> ProblemMetaService::loadAllMeta() const
{
    return m_repository == nullptr ? QList<ProblemMeta>()
                                   : m_repository->loadAllMeta();
}

bool ProblemMetaService::setTags(const QString &problemUrl, const QStringList &tagNames)
{
    if (m_repository == nullptr) {
        emit failed("Problem meta repository not available");
        return false;
    }
    if (!m_repository->setTags(problemUrl, tagNames)) {
        emit failed(m_repository->lastError());
        return false;
    }
    return true;
}

QStringList ProblemMetaService::loadAllTags() const
{
    return m_repository == nullptr ? QStringList() : m_repository->loadAllTags();
}

QHash<QString, int> ProblemMetaService::statusCounts() const
{
    return m_repository == nullptr ? QHash<QString, int>()
                                   : m_repository->statusCounts();
}

QHash<QString, int> ProblemMetaService::tagCounts() const
{
    return m_repository == nullptr ? QHash<QString, int>()
                                   : m_repository->tagCounts();
}

QList<ProblemMeta> ProblemMetaService::reviewProblems() const
{
    return m_repository == nullptr ? QList<ProblemMeta>()
                                   : m_repository->reviewProblems();
}

int ProblemMetaService::notesCount() const
{
    return m_repository == nullptr ? 0 : m_repository->notesCount();
}

QString ProblemMetaService::lastError() const
{
    return m_repository == nullptr ? QString() : m_repository->lastError();
}
