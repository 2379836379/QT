#include "problemparser.h"

#include "parsercommon.h"

#include <QRegularExpression>

namespace
{
QString stripHtml(const QString &html)
{
    QString text = html;
    text.remove(QRegularExpression("<[^>]+>"));
    return text.simplified();
}

QString extractDlValue(const QString &html, const QString &key)
{
    const QString pattern = QString(
        "<dt>\\s*%1\\s*</dt>\\s*<dd>([\\s\\S]*?)</dd>")
                                .arg(QRegularExpression::escape(key));
    const QRegularExpression regex(
        pattern,
        QRegularExpression::CaseInsensitiveOption);
    const QRegularExpressionMatch match = regex.match(html);
    if (!match.hasMatch()) {
        return QString();
    }

    return stripHtml(match.captured(1));
}
}

namespace ProblemParser
{
ProblemPageInfo parseProblemPage(const QByteArray &html, const QUrl &baseUrl)
{
    ProblemPageInfo info;
    info.problemUrl = baseUrl.toString();

    const QString text = ParserCommon::toUtf8String(html);

    const QRegularExpression titleRegex(
        "<div\\s+id=\"pageTitle\"[^>]*>\\s*<h2>([^<]+)</h2>",
        QRegularExpression::CaseInsensitiveOption);
    const QRegularExpressionMatch titleMatch = titleRegex.match(text);
    if (titleMatch.hasMatch()) {
        info.title = titleMatch.captured(1).trimmed();
    }

    const QRegularExpression submitRegex(
        "<li[^>]*>\\s*<a\\s+href=\"([^\"]+/submit/)\"[^>]*>\\s*提交\\s*</a>\\s*</li>",
        QRegularExpression::CaseInsensitiveOption);
    const QRegularExpressionMatch submitMatch = submitRegex.match(text);
    if (submitMatch.hasMatch()) {
        info.submitUrl =
            baseUrl.resolved(QUrl(submitMatch.captured(1))).toString();
    }

    info.timeLimit = extractDlValue(text, "总时间限制:");
    info.memoryLimit = extractDlValue(text, "内存限制:");
    info.description = extractDlValue(text, "描述");
    info.inputSpec = extractDlValue(text, "输入");
    info.outputSpec = extractDlValue(text, "输出");
    info.sampleInput = extractDlValue(text, "样例输入");
    info.sampleOutput = extractDlValue(text, "样例输出");
    info.hint = extractDlValue(text, "提示");
    info.tried_people = extractDlValue(text, "尝试人数").toInt();
    info.passed_people = extractDlValue(text, "通过人数").toInt();
    return info;
}
}
