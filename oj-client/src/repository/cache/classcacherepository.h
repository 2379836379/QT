#pragma once

#include "parser/classparser.h"
#include "parser/groupparser.h"

#include <QString>
#include <QtGlobal>

class ClassCacheRepository
{
public:
    ClassCacheRepository();
    ~ClassCacheRepository();

    bool loadClass(const QString &classPageUrl,
                   ClassPageInfo *classPageInfo,
                   GroupPageInfo *groupPageInfo);
    bool saveClass(const ClassPageInfo &classPageInfo,
                   const GroupPageInfo &groupPageInfo);
    qint64 cacheSizeBytes() const;
    bool clearCache();
    QString lastError() const;

private:
    bool ensureReady();
    bool openDatabase();
    bool createTables();

    QString m_connectionName;
    QString m_lastError;
    bool m_initialized = false;
};
