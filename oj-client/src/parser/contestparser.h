#pragma once

#include <QByteArray>
#include <QList>
#include <QString>
#include <QUrl>

struct ContestProblemInfo
{
    QString problemId;
    QString title;
    QString problemUrl;
    int accept_prople = 0;
    int submission_people = 0;
    bool solved = false;
};

struct ContestPageInfo
{
    QString contestPageUrl;
    QList<ContestProblemInfo> problems;
};

namespace ContestParser
{
ContestPageInfo parseContestPage(const QByteArray &html, const QUrl &baseUrl);
}
