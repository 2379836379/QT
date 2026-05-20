#include "classcacherepository.h"

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
    return dir.filePath("class_cache.db");
}

bool ensureColumn(QSqlDatabase &database,
                  const QString &tableName,
                  const QString &columnName,
                  const QString &definition,
                  QString *error)
{
    QSqlQuery infoQuery(database);
    if (!infoQuery.exec(QString("PRAGMA table_info(%1)").arg(tableName))) {
        if (error != nullptr) {
            *error = infoQuery.lastError().text();
        }
        return false;
    }

    while (infoQuery.next()) {
        if (infoQuery.value(1).toString() == columnName) {
            return true;
        }
    }

    QSqlQuery alterQuery(database);
    const bool ok = alterQuery.exec(
        QString("ALTER TABLE %1 ADD COLUMN %2 %3")
            .arg(tableName, columnName, definition));
    if (!ok && error != nullptr) {
        *error = alterQuery.lastError().text();
    }
    return ok;
}
}

ClassCacheRepository::ClassCacheRepository()
    : m_connectionName(QUuid::createUuid().toString(QUuid::WithoutBraces))
{
}

ClassCacheRepository::~ClassCacheRepository()
{
    if (QSqlDatabase::contains(m_connectionName)) {
        QSqlDatabase database = QSqlDatabase::database(m_connectionName);
        database.close();
    }
    QSqlDatabase::removeDatabase(m_connectionName);
}

bool ClassCacheRepository::loadClass(const QString &classPageUrl,
                                     ClassPageInfo *classPageInfo,
                                     GroupPageInfo *groupPageInfo)
{
    if (!ensureReady()) {
        return false;
    }

    QSqlQuery classQuery(QSqlDatabase::database(m_connectionName));
    classQuery.prepare(
        "SELECT class_page_url, group_entry_url, course_name, group_page_url "
        "FROM class_cache WHERE class_page_url = :class_page_url");
    classQuery.bindValue(":class_page_url", classPageUrl);
    if (!classQuery.exec() || !classQuery.next()) {
        if (!classQuery.lastError().text().isEmpty()) {
            m_lastError = classQuery.lastError().text();
        }
        return false;
    }

    ClassPageInfo loadedClassInfo;
    loadedClassInfo.classPageUrl = classQuery.value(0).toString();
    loadedClassInfo.groupEntryUrl = classQuery.value(1).toString();
    loadedClassInfo.courseName = classQuery.value(2).toString();

    GroupPageInfo loadedGroupInfo;
    loadedGroupInfo.groupPageUrl = classQuery.value(3).toString();

    QSqlQuery itemsQuery(QSqlDatabase::database(m_connectionName));
    itemsQuery.prepare(
        "SELECT url, title, item_class, problem_number, end_time, extra_text "
        "FROM group_cache_items WHERE class_page_url = :class_page_url "
        "ORDER BY row_index ASC");
    itemsQuery.bindValue(":class_page_url", classPageUrl);
    if (!itemsQuery.exec()) {
        m_lastError = itemsQuery.lastError().text();
        return false;
    }

    while (itemsQuery.next()) {
        ContestSetInfo contestSet;
        contestSet.url = itemsQuery.value(0).toString();
        contestSet.title = itemsQuery.value(1).toString();
        contestSet.itemClass = itemsQuery.value(2).toString();
        contestSet.problemnumber = itemsQuery.value(3).toString();
        contestSet.endtime = itemsQuery.value(4).toString();
        contestSet.extraText = itemsQuery.value(5).toString();
        loadedGroupInfo.contestSets << contestSet;
    }

    if (loadedGroupInfo.contestSets.isEmpty()) {
        return false;
    }

    if (classPageInfo != nullptr) {
        *classPageInfo = loadedClassInfo;
    }
    if (groupPageInfo != nullptr) {
        *groupPageInfo = loadedGroupInfo;
    }
    return true;
}

bool ClassCacheRepository::saveClass(const ClassPageInfo &classPageInfo,
                                     const GroupPageInfo &groupPageInfo)
{
    if (!ensureReady()) {
        return false;
    }

    QSqlDatabase database = QSqlDatabase::database(m_connectionName);
    if (!database.transaction()) {
        m_lastError = database.lastError().text();
        return false;
    }

    QSqlQuery upsertClassQuery(database);
    upsertClassQuery.prepare(
        "INSERT INTO class_cache ("
        "class_page_url, group_entry_url, course_name, group_page_url, cached_at"
        ") VALUES ("
        ":class_page_url, :group_entry_url, :course_name, :group_page_url, CURRENT_TIMESTAMP"
        ") "
        "ON CONFLICT(class_page_url) DO UPDATE SET "
        "group_entry_url = excluded.group_entry_url, "
        "course_name = excluded.course_name, "
        "group_page_url = excluded.group_page_url, "
        "cached_at = CURRENT_TIMESTAMP");
    upsertClassQuery.bindValue(":class_page_url", classPageInfo.classPageUrl);
    upsertClassQuery.bindValue(":group_entry_url", classPageInfo.groupEntryUrl);
    upsertClassQuery.bindValue(":course_name", classPageInfo.courseName);
    upsertClassQuery.bindValue(":group_page_url", groupPageInfo.groupPageUrl);
    if (!upsertClassQuery.exec()) {
        m_lastError = upsertClassQuery.lastError().text();
        database.rollback();
        return false;
    }

    QSqlQuery deleteItemsQuery(database);
    deleteItemsQuery.prepare(
        "DELETE FROM group_cache_items WHERE class_page_url = :class_page_url");
    deleteItemsQuery.bindValue(":class_page_url", classPageInfo.classPageUrl);
    if (!deleteItemsQuery.exec()) {
        m_lastError = deleteItemsQuery.lastError().text();
        database.rollback();
        return false;
    }

    QSqlQuery insertItemQuery(database);
    insertItemQuery.prepare(
        "INSERT INTO group_cache_items ("
        "class_page_url, row_index, url, title, item_class, problem_number, end_time, extra_text"
        ") VALUES ("
        ":class_page_url, :row_index, :url, :title, :item_class, :problem_number, :end_time, :extra_text"
        ")");
    for (int i = 0; i < groupPageInfo.contestSets.size(); ++i) {
        const ContestSetInfo &contestSet = groupPageInfo.contestSets.at(i);
        insertItemQuery.bindValue(":class_page_url", classPageInfo.classPageUrl);
        insertItemQuery.bindValue(":row_index", i);
        insertItemQuery.bindValue(":url", contestSet.url);
        insertItemQuery.bindValue(":title", contestSet.title);
        insertItemQuery.bindValue(":item_class", contestSet.itemClass);
        insertItemQuery.bindValue(":problem_number", contestSet.problemnumber);
        insertItemQuery.bindValue(":end_time", contestSet.endtime);
        insertItemQuery.bindValue(":extra_text", contestSet.extraText);
        if (!insertItemQuery.exec()) {
            m_lastError = insertItemQuery.lastError().text();
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

QString ClassCacheRepository::lastError() const
{
    return m_lastError;
}

qint64 ClassCacheRepository::cacheSizeBytes() const
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

bool ClassCacheRepository::clearCache()
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

bool ClassCacheRepository::ensureReady()
{
    if (m_initialized) {
        return true;
    }
    if (!openDatabase()) {
        return false;
    }
    if (!createTables()) {
        return false;
    }
    m_initialized = true;
    return true;
}

bool ClassCacheRepository::openDatabase()
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

bool ClassCacheRepository::createTables()
{
    QSqlDatabase database = QSqlDatabase::database(m_connectionName);
    QSqlQuery query(database);
    const bool classOk = query.exec(
        "CREATE TABLE IF NOT EXISTS class_cache ("
        "class_page_url TEXT PRIMARY KEY,"
        "group_entry_url TEXT,"
        "course_name TEXT,"
        "group_page_url TEXT,"
        "cached_at TEXT DEFAULT CURRENT_TIMESTAMP"
        ")");
    if (!classOk) {
        m_lastError = query.lastError().text();
        return false;
    }

    const bool itemsOk = query.exec(
        "CREATE TABLE IF NOT EXISTS group_cache_items ("
        "class_page_url TEXT NOT NULL,"
        "row_index INTEGER NOT NULL,"
        "url TEXT,"
        "title TEXT,"
        "item_class TEXT,"
        "problem_number TEXT,"
        "end_time TEXT,"
        "extra_text TEXT,"
        "PRIMARY KEY (class_page_url, row_index)"
        ")");
    if (!itemsOk) {
        m_lastError = query.lastError().text();
        return false;
    }

    if (!ensureColumn(database, "group_cache_items", "problem_number", "TEXT", &m_lastError)) {
        return false;
    }
    if (!ensureColumn(database, "group_cache_items", "end_time", "TEXT", &m_lastError)) {
        return false;
    }
    return true;
}
