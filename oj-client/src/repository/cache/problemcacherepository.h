#pragma once

#include "parser/problemparser.h"

#include <QString>
#include <QtGlobal>

class ProblemCacheRepository
{
public:
    ProblemCacheRepository();
    ~ProblemCacheRepository();

    bool loadProblem(const QString &problemUrl, ProblemPageInfo *problemPageInfo);
    bool saveProblem(const ProblemPageInfo &problemPageInfo);
    qint64 cacheSizeBytes() const;
    bool clearCache();
    QString lastError() const;

private:
    bool ensureReady();
    bool openDatabase();
    bool createTable();
    ProblemPageInfo readProblem(const class QSqlQuery &query) const;

    QString m_connectionName;
    QString m_lastError;
    bool m_initialized = false;
};
