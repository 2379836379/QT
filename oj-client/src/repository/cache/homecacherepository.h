#pragma once

#include "parser/loginparser.h"

#include <QList>
#include <QString>
#include <QtGlobal>

class HomeCacheRepository
{
public:
    HomeCacheRepository();
    ~HomeCacheRepository();

    bool loadHome(const QString &homeUrl, QList<JoinedClassInfo> *classes);
    bool saveHome(const QString &homeUrl, const QList<JoinedClassInfo> &classes);
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
