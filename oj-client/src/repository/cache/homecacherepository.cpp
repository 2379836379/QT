#include "homecacherepository.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QUuid>
#include <QVariant>

namespace
{
QDir projectRootDir()
{
    QDir dir(QCoreApplication::applicationDirPath());
    if (dir.dirName().compare("build", Qt::CaseInsensitive) == 0) {
        dir.cdUp();
    } else if ((dir.dirName().compare("debug", Qt::CaseInsensitive) == 0
                || dir.dirName().compare("release", Qt::CaseInsensitive) == 0)
               && dir.cdUp()
               && dir.dirName().compare("build", Qt::CaseInsensitive) == 0) {
        dir.cdUp();
    }
    return dir;
}

QString databasePath()
{
    QDir dir(projectRootDir().filePath("cache"));
    dir.mkpath(".");
    return dir.filePath("home_cache.db");
}
}

HomeCacheRepository::HomeCacheRepository()
    : m_connectionName(QUuid::createUuid().toString(QUuid::WithoutBraces))
{
}

HomeCacheRepository::~HomeCacheRepository()
{
    if (QSqlDatabase::contains(m_connectionName)) {
        QSqlDatabase database = QSqlDatabase::database(m_connectionName);
        database.close();
    }
    QSqlDatabase::removeDatabase(m_connectionName);
}

bool HomeCacheRepository::loadHome(const QString &homeUrl,
                                   QList<JoinedClassInfo> *classes)
{
    if (!ensureReady()) {
        return false;
    }

    QSqlQuery query(QSqlDatabase::database(m_connectionName));
    query.prepare(
        "SELECT class_name, class_url FROM home_cache_items "
        "WHERE home_url = :home_url ORDER BY row_index ASC");
    query.bindValue(":home_url", homeUrl);
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }

    QList<JoinedClassInfo> loadedClasses;
    while (query.next()) {
        loadedClasses << JoinedClassInfo{query.value(0).toString(),
                                         query.value(1).toString()};
    }

    if (loadedClasses.isEmpty()) {
        return false;
    }

    if (classes != nullptr) {
        *classes = loadedClasses;
    }
    return true;
}

bool HomeCacheRepository::saveHome(const QString &homeUrl,
                                   const QList<JoinedClassInfo> &classes)
{
    if (!ensureReady()) {
        return false;
    }

    QSqlDatabase database = QSqlDatabase::database(m_connectionName);
    if (!database.transaction()) {
        m_lastError = database.lastError().text();
        return false;
    }

    QSqlQuery deleteQuery(database);
    deleteQuery.prepare("DELETE FROM home_cache_items WHERE home_url = :home_url");
    deleteQuery.bindValue(":home_url", homeUrl);
    if (!deleteQuery.exec()) {
        m_lastError = deleteQuery.lastError().text();
        database.rollback();
        return false;
    }

    QSqlQuery insertQuery(database);
    insertQuery.prepare(
        "INSERT INTO home_cache_items ("
        "home_url, row_index, class_name, class_url, cached_at"
        ") VALUES ("
        ":home_url, :row_index, :class_name, :class_url, CURRENT_TIMESTAMP"
        ")");
    for (int i = 0; i < classes.size(); ++i) {
        const JoinedClassInfo &joinedClass = classes.at(i);
        insertQuery.bindValue(":home_url", homeUrl);
        insertQuery.bindValue(":row_index", i);
        insertQuery.bindValue(":class_name", joinedClass.name);
        insertQuery.bindValue(":class_url", joinedClass.url);
        if (!insertQuery.exec()) {
            m_lastError = insertQuery.lastError().text();
            database.rollback();
            return false;
        }
    }

    if (!database.commit()) {
        m_lastError = database.lastError().text();
        database.rollback();
        return false;
    }

    return true;
}

QString HomeCacheRepository::lastError() const
{
    return m_lastError;
}

qint64 HomeCacheRepository::cacheSizeBytes() const
{
    const QString path = databasePath();
    qint64 total = 0;
    for (const QString &suffix : {QString(), QString("-wal"), QString("-shm")}) {
        const QFileInfo info(path + suffix);
        if (info.exists()) {
            total += info.size();
        }
    }
    return total;
}

bool HomeCacheRepository::clearCache()
{
    const QString path = databasePath();
    {
        if (QSqlDatabase::contains(m_connectionName)) {
            QSqlDatabase database = QSqlDatabase::database(m_connectionName);
            database.close();
        }
    }
    QSqlDatabase::removeDatabase(m_connectionName);
    m_initialized = false;

    bool ok = true;
    for (const QString &suffix : {QString(), QString("-wal"), QString("-shm")}) {
        const QString filePath = path + suffix;
        if (QFile::exists(filePath) && !QFile::remove(filePath)) {
            ok = false;
        }
    }
    if (!ok) {
        m_lastError = QString("Failed to remove cache file: %1").arg(path);
    }
    return ok;
}

bool HomeCacheRepository::ensureReady()
{
    if (m_initialized) {
        return true;
    }
    if (!openDatabase()) {
        return false;
    }
    if (!createTable()) {
        return false;
    }
    m_initialized = true;
    return true;
}

bool HomeCacheRepository::openDatabase()
{
    if (!QSqlDatabase::contains(m_connectionName)) {
        QSqlDatabase database =
            QSqlDatabase::addDatabase("QSQLITE", m_connectionName);
        database.setDatabaseName(databasePath());
        if (!database.open()) {
            m_lastError = database.lastError().text();
            return false;
        }
        return true;
    }

    QSqlDatabase database = QSqlDatabase::database(m_connectionName);
    if (database.isOpen()) {
        return true;
    }

    const bool ok = database.open();
    if (!ok) {
        m_lastError = database.lastError().text();
    }
    return ok;
}

bool HomeCacheRepository::createTable()
{
    QSqlQuery query(QSqlDatabase::database(m_connectionName));
    const bool ok = query.exec(
        "CREATE TABLE IF NOT EXISTS home_cache_items ("
        "home_url TEXT NOT NULL,"
        "row_index INTEGER NOT NULL,"
        "class_name TEXT,"
        "class_url TEXT NOT NULL,"
        "cached_at TEXT DEFAULT CURRENT_TIMESTAMP,"
        "PRIMARY KEY (home_url, row_index)"
        ")");
    if (!ok) {
        m_lastError = query.lastError().text();
    }
    return ok;
}
