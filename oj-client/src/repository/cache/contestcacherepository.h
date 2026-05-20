#pragma once

#include "parser/contestparser.h"

#include <QString>
#include <QtGlobal>

class ContestCacheRepository
{
public:
    ContestCacheRepository();
    ~ContestCacheRepository();

    bool loadContest(const QString &contestPageUrl, ContestPageInfo *contestPageInfo);
    bool saveContest(const ContestPageInfo &contestPageInfo);
    qint64 cacheSizeBytes() const;
    bool clearCache();
    QString lastError() const;

private:
    bool ensureReady();
    bool openDatabase();
    bool createTable();

    QString m_connectionName;
    QString m_lastError;
    bool m_initialized = false;
};
