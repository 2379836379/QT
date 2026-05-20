#include "contestcacherepository.h"

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
    return dir.filePath("contest_cache.db");
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

ContestCacheRepository::ContestCacheRepository()
    : m_connectionName(QUuid::createUuid().toString(QUuid::WithoutBraces))
{
}

ContestCacheRepository::~ContestCacheRepository()
{
    if (QSqlDatabase::contains(m_connectionName)) {
        QSqlDatabase database = QSqlDatabase::database(m_connectionName);
        database.close();
    }
    QSqlDatabase::removeDatabase(m_connectionName);
}

bool ContestCacheRepository::loadContest(
    const QString &contestPageUrl, ContestPageInfo *contestPageInfo)
{
    if (!ensureReady()) {
        return false;
    }

    QSqlQuery query(QSqlDatabase::database(m_connectionName));
    query.prepare(
        "SELECT problem_id, title, problem_url, score, submissions, solved "
        "FROM contest_cache_items "
        "WHERE contest_page_url = :contest_page_url "
        "ORDER BY row_index ASC");
    query.bindValue(":contest_page_url", contestPageUrl);
    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }

    ContestPageInfo loadedContestInfo;
    loadedContestInfo.contestPageUrl = contestPageUrl;
    while (query.next()) {
        ContestProblemInfo problem;
        problem.problemId = query.value(0).toString();
        problem.title = query.value(1).toString();
        problem.problemUrl = query.value(2).toString();
        problem.accept_prople = query.value(3).toInt();
        problem.submission_people = query.value(4).toInt();
        problem.solved = query.value(5).toBool();
        loadedContestInfo.problems << problem;
    }

    if (loadedContestInfo.problems.isEmpty()) {
        return false;
    }

    if (contestPageInfo != nullptr) {
        *contestPageInfo = loadedContestInfo;
    }
    return true;
}

bool ContestCacheRepository::saveContest(const ContestPageInfo &contestPageInfo)
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
    deleteQuery.prepare(
        "DELETE FROM contest_cache_items WHERE contest_page_url = :contest_page_url");
    deleteQuery.bindValue(":contest_page_url", contestPageInfo.contestPageUrl);
    if (!deleteQuery.exec()) {
        m_lastError = deleteQuery.lastError().text();
        database.rollback();
        return false;
    }

    QSqlQuery insertQuery(database);
    insertQuery.prepare(
        "INSERT INTO contest_cache_items ("
        "contest_page_url, row_index, problem_id, title, problem_url, score, submissions, solved, cached_at"
        ") VALUES ("
        ":contest_page_url, :row_index, :problem_id, :title, :problem_url, :score, :submissions, :solved, CURRENT_TIMESTAMP"
        ")");

    for (int i = 0; i < contestPageInfo.problems.size(); ++i) {
        const ContestProblemInfo &problem = contestPageInfo.problems.at(i);
        insertQuery.bindValue(":contest_page_url", contestPageInfo.contestPageUrl);
        insertQuery.bindValue(":row_index", i);
        insertQuery.bindValue(":problem_id", problem.problemId);
        insertQuery.bindValue(":title", problem.title);
        insertQuery.bindValue(":problem_url", problem.problemUrl);
        insertQuery.bindValue(":score", problem.accept_prople);
        insertQuery.bindValue(":submissions", problem.submission_people);
        insertQuery.bindValue(":solved", problem.solved);
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

QString ContestCacheRepository::lastError() const
{
    return m_lastError;
}

qint64 ContestCacheRepository::cacheSizeBytes() const
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

bool ContestCacheRepository::clearCache()
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

bool ContestCacheRepository::ensureReady()
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

bool ContestCacheRepository::openDatabase()
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

bool ContestCacheRepository::createTable()
{
    QSqlDatabase database = QSqlDatabase::database(m_connectionName);
    QSqlQuery query(database);
    const bool ok = query.exec(
        "CREATE TABLE IF NOT EXISTS contest_cache_items ("
        "contest_page_url TEXT NOT NULL,"
        "row_index INTEGER NOT NULL,"
        "problem_id TEXT,"
        "title TEXT,"
        "problem_url TEXT NOT NULL,"
        "score INTEGER,"
        "submissions INTEGER,"
        "solved INTEGER,"
        "cached_at TEXT DEFAULT CURRENT_TIMESTAMP,"
        "PRIMARY KEY (contest_page_url, row_index)"
        ")");
    if (!ok) {
        m_lastError = query.lastError().text();
        return false;
    }

    if (!ensureColumn(database, "contest_cache_items", "score", "INTEGER", &m_lastError)) {
        return false;
    }
    if (!ensureColumn(database, "contest_cache_items", "submissions", "INTEGER", &m_lastError)) {
        return false;
    }
    if (!ensureColumn(database, "contest_cache_items", "solved", "INTEGER", &m_lastError)) {
        return false;
    }
    return true;
}
