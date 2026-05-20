#include "problemcacherepository.h"

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
    return dir.filePath("problem_cache.db");
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

ProblemCacheRepository::ProblemCacheRepository()
    : m_connectionName(QUuid::createUuid().toString(QUuid::WithoutBraces))
{
}

ProblemCacheRepository::~ProblemCacheRepository()
{
    if (QSqlDatabase::contains(m_connectionName)) {
        QSqlDatabase database = QSqlDatabase::database(m_connectionName);
        database.close();
    }
    QSqlDatabase::removeDatabase(m_connectionName);
}

bool ProblemCacheRepository::loadProblem(
    const QString &problemUrl, ProblemPageInfo *problemPageInfo)
{
    if (!ensureReady()) {
        return false;
    }

    QSqlQuery query(QSqlDatabase::database(m_connectionName));
    query.prepare(
        "SELECT problem_url, title, submit_url, time_limit, memory_limit, "
        "description, input_spec, output_spec, sample_input, sample_output, hint, tried_people, passed_people "
        "FROM problem_cache WHERE problem_url = :problem_url");
    query.bindValue(":problem_url", problemUrl);
    if (!query.exec() || !query.next()) {
        return false;
    }

    if (problemPageInfo != nullptr) {
        *problemPageInfo = readProblem(query);
    }
    return true;
}

bool ProblemCacheRepository::saveProblem(const ProblemPageInfo &problemPageInfo)
{
    if (!ensureReady()) {
        return false;
    }

    QSqlQuery query(QSqlDatabase::database(m_connectionName));
    query.prepare(
        "INSERT INTO problem_cache ("
        "problem_url, title, submit_url, time_limit, memory_limit, "
        "description, input_spec, output_spec, sample_input, sample_output, hint, tried_people, passed_people, cached_at"
        ") VALUES ("
        ":problem_url, :title, :submit_url, :time_limit, :memory_limit, "
        ":description, :input_spec, :output_spec, :sample_input, :sample_output, :hint, :tried_people, :passed_people, CURRENT_TIMESTAMP"
        ") "
        "ON CONFLICT(problem_url) DO UPDATE SET "
        "title = excluded.title, "
        "submit_url = excluded.submit_url, "
        "time_limit = excluded.time_limit, "
        "memory_limit = excluded.memory_limit, "
        "description = excluded.description, "
        "input_spec = excluded.input_spec, "
        "output_spec = excluded.output_spec, "
        "sample_input = excluded.sample_input, "
        "sample_output = excluded.sample_output, "
        "hint = excluded.hint, "
        "tried_people = excluded.tried_people, "
        "passed_people = excluded.passed_people, "
        "cached_at = CURRENT_TIMESTAMP");
    query.bindValue(":problem_url", problemPageInfo.problemUrl);
    query.bindValue(":title", problemPageInfo.title);
    query.bindValue(":submit_url", problemPageInfo.submitUrl);
    query.bindValue(":time_limit", problemPageInfo.timeLimit);
    query.bindValue(":memory_limit", problemPageInfo.memoryLimit);
    query.bindValue(":description", problemPageInfo.description);
    query.bindValue(":input_spec", problemPageInfo.inputSpec);
    query.bindValue(":output_spec", problemPageInfo.outputSpec);
    query.bindValue(":sample_input", problemPageInfo.sampleInput);
    query.bindValue(":sample_output", problemPageInfo.sampleOutput);
    query.bindValue(":hint", problemPageInfo.hint);
    query.bindValue(":tried_people", problemPageInfo.tried_people);
    query.bindValue(":passed_people", problemPageInfo.passed_people);

    const bool ok = query.exec();
    if (!ok) {
        m_lastError = query.lastError().text();
    }
    return ok;
}

QString ProblemCacheRepository::lastError() const
{
    return m_lastError;
}

qint64 ProblemCacheRepository::cacheSizeBytes() const
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

bool ProblemCacheRepository::clearCache()
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

bool ProblemCacheRepository::ensureReady()
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

bool ProblemCacheRepository::openDatabase()
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

bool ProblemCacheRepository::createTable()
{
    QSqlDatabase database = QSqlDatabase::database(m_connectionName);
    QSqlQuery query(database);
    const bool ok = query.exec(
        "CREATE TABLE IF NOT EXISTS problem_cache ("
        "problem_url TEXT PRIMARY KEY,"
        "title TEXT,"
        "submit_url TEXT,"
        "time_limit TEXT,"
        "memory_limit TEXT,"
        "description TEXT,"
        "input_spec TEXT,"
        "output_spec TEXT,"
        "sample_input TEXT,"
        "sample_output TEXT,"
        "hint TEXT,"
        "tried_people INTEGER DEFAULT 0,"
        "passed_people INTEGER DEFAULT 0,"
        "cached_at TEXT DEFAULT CURRENT_TIMESTAMP"
        ")");
    if (!ok) {
        m_lastError = query.lastError().text();
        return false;
    }

    if (!ensureColumn(database, "problem_cache", "tried_people", "INTEGER DEFAULT 0", &m_lastError)) {
        return false;
    }
    if (!ensureColumn(database, "problem_cache", "passed_people", "INTEGER DEFAULT 0", &m_lastError)) {
        return false;
    }
    return true;
}

ProblemPageInfo ProblemCacheRepository::readProblem(const QSqlQuery &query) const
{
    ProblemPageInfo info;
    info.problemUrl = query.value(0).toString();
    info.title = query.value(1).toString();
    info.submitUrl = query.value(2).toString();
    info.timeLimit = query.value(3).toString();
    info.memoryLimit = query.value(4).toString();
    info.description = query.value(5).toString();
    info.inputSpec = query.value(6).toString();
    info.outputSpec = query.value(7).toString();
    info.sampleInput = query.value(8).toString();
    info.sampleOutput = query.value(9).toString();
    info.hint = query.value(10).toString();
    info.tried_people = query.value(11).toInt();
    info.passed_people = query.value(12).toInt();
    return info;
}
