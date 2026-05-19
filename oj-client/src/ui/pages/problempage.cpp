#include "ui/pages/problempage.h"

#include <QComboBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QRegularExpression>
#include <QTextEdit>
#include <QVBoxLayout>

namespace
{
QString formatProblemDetail(const ProblemPageInfo &problemPageInfo)
{
    return QString(
               "Title\n%1\n\n"
               "Problem URL\n%2\n\n"
               "Submit URL\n%3\n\n"
               "Time Limit\n%4\n\n"
               "Memory Limit\n%5\n\n"
               "Description\n%6\n\n"
               "Input\n%7\n\n"
               "Output\n%8\n\n"
               "Sample Input\n%9\n\n"
               "Sample Output\n%10\n\n"
               "Hint\n%11")
        .arg(problemPageInfo.title,
             problemPageInfo.problemUrl,
             problemPageInfo.submitUrl,
             problemPageInfo.timeLimit,
             problemPageInfo.memoryLimit,
             problemPageInfo.description,
             problemPageInfo.inputSpec,
             problemPageInfo.outputSpec,
             problemPageInfo.sampleInput,
             problemPageInfo.sampleOutput,
             problemPageInfo.hint);
}

QString extractFirstMatch(const QString &text, const QString &pattern)
{
    const QRegularExpression regex(
        pattern,
        QRegularExpression::CaseInsensitiveOption
            | QRegularExpression::DotMatchesEverythingOption);
    const QRegularExpressionMatch match = regex.match(text);
    return match.hasMatch() ? match.captured(1).simplified() : QString();
}

QString classifySubmitResponse(const NetworkResult &result, const QString &bodyText)
{
    if (!result.ok) {
        return "network error";
    }
    if (result.statusCode == 401) {
        return "authentication required";
    }
    if (bodyText.trimmed().startsWith("<")) {
        if (bodyText.contains("Host requires authentication",
                              Qt::CaseInsensitive)) {
            return "authentication page";
        }
        if (result.finalUrl != result.requestUrl) {
            return "redirected html page";
        }
        return "html page";
    }
    if (bodyText.trimmed().startsWith("{")
        || bodyText.trimmed().startsWith("[")) {
        return "json-like text";
    }
    return "plain text or unknown";
}

QString formatSubmitPayloadPreview(const QString &languageLabel,
                                   const QString &languageValue,
                                   const QString &sourceText,
                                   const QByteArray &payload)
{
    QString preview = QString::fromUtf8(payload);
    if (preview.size() > 1200) {
        preview = preview.left(1200) + "\n...[truncated]";
    }

    return QString(
               "Submit Request Preview\n"
               "Selected label: %1\n"
               "Selected value: %2\n"
               "Source length: %3\n\n"
               "Payload:\n%4")
        .arg(languageLabel.isEmpty() ? "<none>" : languageLabel,
             languageValue.isEmpty() ? "<none>" : languageValue,
             QString::number(sourceText.size()),
             preview);
}

QString formatSubmitResult(const NetworkResult &result)
{
    const QString bodyText = QString::fromUtf8(result.body);
    const QString responseType = classifySubmitResponse(result, bodyText);
    const QString pageTitle =
        extractFirstMatch(bodyText, "<title[^>]*>(.*?)</title>");
    QString mainHeading = extractFirstMatch(bodyText, "<h1[^>]*>(.*?)</h1>");
    if (mainHeading.isEmpty()) {
        mainHeading = extractFirstMatch(bodyText, "<h2[^>]*>(.*?)</h2>");
    }

    QString preview = bodyText;
    preview.replace("\r\n", "\n");
    preview.replace('\r', '\n');
    if (preview.size() > 1200) {
        preview = preview.left(1200) + "\n...[truncated]";
    }

    return QString(
               "Submit Result\n"
               "HTTP status: %1\n"
               "Network OK: %2\n"
               "Request URL: %3\n"
               "Final URL: %4\n"
               "Response type: %5\n"
               "Page title: %6\n"
               "Main heading: %7\n"
               "Body bytes: %8\n\n"
               "Preview:\n%9")
        .arg(QString::number(result.statusCode),
             result.ok ? "true" : "false",
             result.requestUrl.toString(),
             result.finalUrl.toString(),
             responseType,
             pageTitle.isEmpty() ? "<none>" : pageTitle,
             mainHeading.isEmpty() ? "<none>" : mainHeading,
             QString::number(result.body.size()),
             preview);
}

QString formatResultPageInfo(const ResultPageInfo &resultPageInfo)
{
    QString text = QString(
                       "Parsed Result Page\n"
                       "Submission ID: %1\n"
                       "Status: %2\n"
                       "Status class: %3\n"
                       "Solution URL: %4")
                       .arg(resultPageInfo.submissionId.isEmpty()
                                ? "<none>"
                                : resultPageInfo.submissionId,
                            resultPageInfo.statusText.isEmpty()
                                ? "<none>"
                                : resultPageInfo.statusText,
                            resultPageInfo.statusClass.isEmpty()
                                ? "<none>"
                                : resultPageInfo.statusClass,
                            resultPageInfo.solutionUrl.isEmpty()
                                ? "<none>"
                                : resultPageInfo.solutionUrl);
    if (!resultPageInfo.detailText.isEmpty()) {
        text += QString("\n\n%1\n%2")
                    .arg(resultPageInfo.detailTitle.isEmpty()
                             ? QString("Detail")
                             : resultPageInfo.detailTitle,
                         resultPageInfo.detailText);
    }
    return text;
}
}

ProblemPage::ProblemPage(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(18);

    auto *topFrame = new QFrame(this);
    topFrame->setObjectName("problemTopFrame");
    topFrame->setFixedHeight(80);
    auto *topLayout = new QHBoxLayout(topFrame);
    topLayout->setContentsMargins(24, 18, 24, 18);
    topLayout->setSpacing(16);

    auto *backButton = new QPushButton("Back", topFrame);
    backButton->setObjectName("problemBackButton");
    m_titleLabel = new QLabel("Problem", topFrame);
    m_titleLabel->setObjectName("problemTitleLabel");

    topLayout->addWidget(backButton);
    topLayout->addWidget(m_titleLabel, 1);

    auto *bottomLayout = new QHBoxLayout();
    bottomLayout->setSpacing(18);

    auto *leftFrame = new QFrame(this);
    leftFrame->setObjectName("problemLeftFrame");
    auto *leftLayout = new QVBoxLayout(leftFrame);
    leftLayout->setContentsMargins(20, 18, 20, 18);
    leftLayout->setSpacing(12);

    auto *toolsLabel = new QLabel("Tools", leftFrame);
    toolsLabel->setObjectName("problemSectionLabel");

    m_toolsListWidget = new QListWidget(leftFrame);
    m_toolsListWidget->setObjectName("problemToolsList");
    auto *backItem = new QListWidgetItem("Back", m_toolsListWidget);
    backItem->setData(Qt::UserRole, "back");
    auto *favoriteItem = new QListWidgetItem("Favorite Current Problem", m_toolsListWidget);
    favoriteItem->setData(Qt::UserRole, "favorite");

    leftLayout->addWidget(toolsLabel);
    leftLayout->addWidget(m_toolsListWidget);
    leftLayout->addStretch();

    auto *problemFrame = new QFrame(this);
    problemFrame->setObjectName("problemMiddleFrame");
    auto *problemLayout = new QVBoxLayout(problemFrame);
    problemLayout->setContentsMargins(20, 18, 20, 18);
    problemLayout->setSpacing(14);

    auto *problemLabel = new QLabel("Problem", problemFrame);
    problemLabel->setObjectName("problemSectionLabel");

    m_detailTextEdit = new QTextEdit(problemFrame);
    m_detailTextEdit->setObjectName("problemDetailText");
    m_detailTextEdit->setReadOnly(true);

    problemLayout->addWidget(problemLabel);
    problemLayout->addWidget(m_detailTextEdit, 1);

    auto *submitFrame = new QFrame(this);
    submitFrame->setObjectName("problemRightFrame");
    auto *submitLayout = new QVBoxLayout(submitFrame);
    submitLayout->setContentsMargins(20, 18, 20, 18);
    submitLayout->setSpacing(14);

    auto *submitLabel = new QLabel("Submit", submitFrame);
    submitLabel->setObjectName("problemSectionLabel");

    m_languageComboBox = new QComboBox(submitFrame);
    m_languageComboBox->setObjectName("problemLanguageCombo");
    m_codeEdit = new QPlainTextEdit(submitFrame);
    m_codeEdit->setObjectName("problemCodeEdit");
    m_codeEdit->setPlaceholderText("Write your source code here.");
    m_codeEdit->setMinimumHeight(180);
    m_submitButton = new QPushButton("Submit Code", submitFrame);
    m_submitButton->setObjectName("problemSubmitButton");
    m_resultTextEdit = new QTextEdit(submitFrame);
    m_resultTextEdit->setObjectName("problemResultText");
    m_resultTextEdit->setReadOnly(true);
    m_resultTextEdit->setMinimumHeight(140);

    submitLayout->addWidget(submitLabel);
    submitLayout->addWidget(m_languageComboBox);
    submitLayout->addWidget(m_codeEdit, 1);
    submitLayout->addWidget(m_submitButton);
    submitLayout->addWidget(m_resultTextEdit, 1);

    bottomLayout->addWidget(leftFrame, 1);
    bottomLayout->addWidget(problemFrame, 2);
    bottomLayout->addWidget(submitFrame, 2);

    layout->addWidget(topFrame);
    layout->addLayout(bottomLayout, 1);

    setStyleSheet(
        "ProblemPage { background: #f3f1eb; }"
        "#problemTopFrame, #problemLeftFrame, #problemMiddleFrame, #problemRightFrame {"
        "  background: #fbfaf7;"
        "  border: 1px solid #ded8cc;"
        "  border-radius: 16px;"
        "}"
        "#problemTitleLabel {"
        "  font-size: 28px;"
        "  font-weight: 600;"
        "  color: #1f2328;"
        "}"
        "#problemSectionLabel {"
        "  font-size: 16px;"
        "  font-weight: 600;"
        "  color: #2f3a33;"
        "}"
        "#problemBackButton, #problemSubmitButton {"
        "  min-width: 88px;"
        "  padding: 8px 14px;"
        "  border: 1px solid #cdd7cf;"
        "  border-radius: 10px;"
        "  background: #f7f5ef;"
        "  color: #243029;"
        "}"
        "#problemBackButton:hover, #problemSubmitButton:hover {"
        "  background: #eef4ef;"
        "}"
        "#problemToolsList {"
        "  background: transparent;"
        "  border: none;"
        "  border-radius: 0px;"
        "  padding: 0px;"
        "  outline: none;"
        "}"
        "#problemToolsList::item {"
        "  padding: 12px 4px;"
        "  border-radius: 8px;"
        "  margin: 2px 0px;"
        "}"
        "#problemToolsList::item:selected {"
        "  background: #dcefea;"
        "  color: #12343b;"
        "}"
        "#problemToolsList::item:hover {"
        "  background: #eef4ef;"
        "}"
        "#problemDetailText, #problemResultText, #problemCodeEdit, #problemLanguageCombo {"
        "  background: #fcfbf8;"
        "  border: 1px solid #d9d4c8;"
        "  border-radius: 12px;"
        "  color: #1f2328;"
        "}"
    );

    connect(backButton, &QPushButton::clicked, this, &ProblemPage::backRequested);
    connect(
        m_toolsListWidget,
        &QListWidget::itemClicked,
        this,
        [this](QListWidgetItem *item) {
            const QString action = item->data(Qt::UserRole).toString();
            if (action == "back") {
                emit backRequested();
            } else if (action == "favorite") {
                emit favoriteRequested();
            }
        });
    connect(
        m_submitButton,
        &QPushButton::clicked,
        this,
        [this]() {
            emit submitRequested(
                m_languageComboBox->currentData().toString(),
                m_codeEdit->toPlainText());
        });

    openProblem();
}

void ProblemPage::openProblem(const QString &problemTitle)
{
    m_titleLabel->setText(problemTitle.isEmpty() ? "Problem" : problemTitle);
    m_detailTextEdit->setPlainText("Loading problem detail...");
    resetSubmitPanel();
    setFavoriteEnabled(false);
    setSubmitEnabled(false);
}

void ProblemPage::showProblemLoadedFromFavorites(const ProblemPageInfo &problemPageInfo)
{
    m_titleLabel->setText(
        problemPageInfo.title.isEmpty() ? QString("Problem") : problemPageInfo.title);
    m_detailTextEdit->setPlainText(formatProblemDetail(problemPageInfo));
}

void ProblemPage::showProblem(const ProblemPageInfo &problemPageInfo)
{
    m_titleLabel->setText(
        problemPageInfo.title.isEmpty() ? QString("Problem") : problemPageInfo.title);
    m_detailTextEdit->setPlainText(formatProblemDetail(problemPageInfo));
}

void ProblemPage::showProblemLoadFailed(const QString &message)
{
    m_detailTextEdit->setPlainText(message);
}

void ProblemPage::openSubmit(const ProblemPageInfo &problemPageInfo)
{
    Q_UNUSED(problemPageInfo);
    resetSubmitPanel();
}

void ProblemPage::showLoadingSubmitOptions(bool loading)
{
    Q_UNUSED(loading);
}

void ProblemPage::showSubmitPageLoaded(const SubmitPageInfo &submitPageInfo,
                                       const QString &defaultLanguage)
{
    m_languageComboBox->clear();
    for (const SubmitLanguageOption &option : submitPageInfo.languages) {
        m_languageComboBox->addItem(option.label, option.value);
    }

    const int index = m_languageComboBox->findData(defaultLanguage);
    if (index >= 0) {
        m_languageComboBox->setCurrentIndex(index);
    }

    const bool hasLanguages = m_languageComboBox->count() > 0;
    m_languageComboBox->setEnabled(hasLanguages);
    m_codeEdit->setEnabled(true);
    m_submitButton->setEnabled(hasLanguages);
    m_resultTextEdit->setPlainText(
        QString("Submit page loaded.\nAction: %1")
            .arg(submitPageInfo.submitActionUrl));
}

void ProblemPage::showSubmitting(bool submitting)
{
    m_submitButton->setEnabled(
        !submitting && m_languageComboBox->count() > 0 && m_languageComboBox->isEnabled());
}

void ProblemPage::showMissingLanguage()
{
    m_resultTextEdit->setPlainText("No language selected.");
}

void ProblemPage::showSubmitPayloadBuilt(const QString &languageValue,
                                         const QByteArray &payload)
{
    m_lastSubmitPreview = formatSubmitPayloadPreview(
        currentLanguageLabel(),
        languageValue,
        m_codeEdit->toPlainText(),
        payload);
    m_resultTextEdit->setPlainText(m_lastSubmitPreview);
}

void ProblemPage::showSubmitResult(const NetworkResult &result)
{
    const QString resultText = formatSubmitResult(result);
    m_resultTextEdit->setPlainText(
        m_lastSubmitPreview.isEmpty()
            ? resultText
            : m_lastSubmitPreview + "\n\n" + resultText);
}

void ProblemPage::showSubmitFailed(const QString &message)
{
    m_resultTextEdit->setPlainText(message);
}

void ProblemPage::appendResultPageInfo(const ResultPageInfo &resultPageInfo)
{
    m_resultTextEdit->append("\n\n" + formatResultPageInfo(resultPageInfo));
}

void ProblemPage::appendResultFailure(const QString &message)
{
    m_resultTextEdit->append("\n\nResult page load failed:\n" + message);
}

QString ProblemPage::currentLanguageLabel() const
{
    return m_languageComboBox->currentText();
}

void ProblemPage::setFavoriteEnabled(bool enabled)
{
    for (int i = 0; i < m_toolsListWidget->count(); ++i) {
        QListWidgetItem *item = m_toolsListWidget->item(i);
        if (item->data(Qt::UserRole).toString() == "favorite") {
            Qt::ItemFlags flags = item->flags();
            if (enabled) {
                item->setFlags(flags | Qt::ItemIsEnabled);
            } else {
                item->setFlags(flags & ~Qt::ItemIsEnabled);
            }
            break;
        }
    }
}

void ProblemPage::setSubmitEnabled(bool enabled)
{
    m_languageComboBox->setEnabled(enabled && m_languageComboBox->count() > 0);
    m_codeEdit->setEnabled(enabled && m_languageComboBox->count() > 0);
    m_submitButton->setEnabled(enabled && m_languageComboBox->count() > 0);
}

void ProblemPage::resetSubmitPanel()
{
    m_languageComboBox->clear();
    m_languageComboBox->setEnabled(false);
    m_codeEdit->setEnabled(false);
    m_submitButton->setEnabled(false);
    m_resultTextEdit->setPlainText("Preparing submit options...");
    m_lastSubmitPreview.clear();
}
