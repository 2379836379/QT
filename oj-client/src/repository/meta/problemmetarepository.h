#pragma once

#include <QHash>
#include <QList>
#include <QString>
#include <QStringList>

struct ProblemMeta
{
    QString problemUrl;
    QString title;
    QString note;
    int difficulty = 0;
    QString taskStatus = "todo";
    int priority = 0;
    QString deadline;
    bool reviewFlag = false;
    QStringList tags;
};

class QSqlQuery;

class ProblemMetaRepository
{
public:
    ProblemMetaRepository();
    ~ProblemMetaRepository();

    bool initialize();
    bool upsertMeta(const ProblemMeta &meta);
    bool loadMeta(const QString &problemUrl, ProblemMeta *meta) const;
    QList<ProblemMeta> loadAllMeta() const;
    bool setTags(const QString &problemUrl, const QStringList &tagNames);
    QStringList loadTags(const QString &problemUrl) const;
    QStringList loadAllTags() const;
    QHash<QString, int> statusCounts() const;
    QHash<QString, int> tagCounts() const;
    QList<ProblemMeta> reviewProblems() const;
    int notesCount() const;
    QString lastError() const;

private:
    bool openDatabase();
    bool ensureSchema();
    ProblemMeta readMeta(const QSqlQuery &query) const;

    QString m_connectionName;
    mutable QString m_lastError;
};
