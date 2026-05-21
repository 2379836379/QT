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
#include <QSplitter>
#include <QStackedWidget>
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
    layout->setSpacing(4);

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
    bottomLayout->setSpacing(4);

    m_toolsFrame = new QFrame(this);
    m_toolsFrame->setObjectName("problemLeftFrame");
    auto *leftLayout = new QVBoxLayout(m_toolsFrame);
    leftLayout->setContentsMargins(20, 18, 20, 18);
    leftLayout->setSpacing(12);

    m_toolsToggleButton = new QPushButton(m_toolsFrame);
    m_toolsToggleButton->setObjectName("problemToolsToggleButton");

    m_toolsPanel = new QWidget(m_toolsFrame);
    auto *toolsPanelLayout = new QVBoxLayout(m_toolsPanel);
    toolsPanelLayout->setContentsMargins(0, 0, 0, 0);
    toolsPanelLayout->setSpacing(10);

    m_backToolButton = new QPushButton("Back", m_toolsPanel);
    m_backToolButton->setObjectName("problemToolButton");
    m_favoriteToolButton = new QPushButton("Favorite Current Problem", m_toolsPanel);
    m_favoriteToolButton->setObjectName("problemToolButton");
    m_aiToolButton = new QPushButton("AI", m_toolsPanel);
    m_aiToolButton->setObjectName("problemToolButton");
    toolsPanelLayout->addWidget(m_backToolButton);
    toolsPanelLayout->addWidget(m_favoriteToolButton);
    toolsPanelLayout->addWidget(m_aiToolButton);
    toolsPanelLayout->addStretch();

    m_collapsedToolsPanel = new QWidget(m_toolsFrame);
    m_collapsedToolsPanel->setObjectName("problemCollapsedToolsPanel");
    auto *collapsedLayout = new QVBoxLayout(m_collapsedToolsPanel);
    collapsedLayout->setContentsMargins(0, 0, 0, 0);
    collapsedLayout->setSpacing(10);

    m_collapsedBackButton = new QPushButton("B", m_collapsedToolsPanel);
    m_collapsedBackButton->setObjectName("problemToolIconButton");
    m_collapsedBackButton->setToolTip("Back");
    m_collapsedFavoriteButton = new QPushButton("F", m_collapsedToolsPanel);
    m_collapsedFavoriteButton->setObjectName("problemToolIconButton");
    m_collapsedFavoriteButton->setToolTip("Favorite Current Problem");
    m_collapsedAiButton = new QPushButton("A", m_collapsedToolsPanel);
    m_collapsedAiButton->setObjectName("problemToolIconButton");
    m_collapsedAiButton->setToolTip("AI");
    collapsedLayout->addWidget(m_collapsedBackButton);
    collapsedLayout->addWidget(m_collapsedFavoriteButton);
    collapsedLayout->addWidget(m_collapsedAiButton);
    collapsedLayout->addStretch();

    leftLayout->addWidget(m_toolsToggleButton);
    leftLayout->addWidget(m_toolsPanel);
    leftLayout->addWidget(m_collapsedToolsPanel);
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
    auto *resultTabLayout = new QHBoxLayout();
    resultTabLayout->setContentsMargins(0, 0, 0, 0);
    resultTabLayout->setSpacing(8);

    m_testTabButton = new QPushButton("test", submitFrame);
    m_testTabButton->setObjectName("problemResultTabButton");
    m_testTabButton->setCheckable(true);
    m_submitTabButton = new QPushButton("submit", submitFrame);
    m_submitTabButton->setObjectName("problemResultTabButton");
    m_submitTabButton->setCheckable(true);
    resultTabLayout->addWidget(m_testTabButton);
    resultTabLayout->addWidget(m_submitTabButton);

    m_inputButton = new QPushButton("input", submitFrame);
    m_inputButton->setObjectName("problemInputButton");
    m_submitButton = new QPushButton("Submit Code", submitFrame);
    m_submitButton->setObjectName("problemSubmitButton");
    resultTabLayout->addStretch();
    resultTabLayout->addWidget(m_inputButton, 0, Qt::AlignRight);
    resultTabLayout->addWidget(m_submitButton, 0, Qt::AlignRight);

    m_resultStack = new QStackedWidget(submitFrame);
    m_resultStack->setObjectName("problemResultStack");
    m_testPaneSplitter = new QSplitter(Qt::Horizontal, m_resultStack);
    m_testPaneSplitter->setObjectName("problemTestPaneSplitter");
    m_testPaneSplitter->setChildrenCollapsible(false);
    m_testPaneSplitter->setHandleWidth(8);

    m_testInputTextEdit = new QTextEdit(m_testPaneSplitter);
    m_testInputTextEdit->setObjectName("problemResultText");
    m_testInputTextEdit->setReadOnly(false);
    m_testInputTextEdit->setMinimumHeight(140);
    m_testInputTextEdit->setPlaceholderText("Write test input here.");

    m_testResultTextEdit = new QTextEdit(m_testPaneSplitter);
    m_testResultTextEdit->setObjectName("problemResultText");
    m_testResultTextEdit->setReadOnly(false);
    m_testResultTextEdit->setMinimumHeight(140);
    m_testResultTextEdit->setReadOnly(true);
    m_testResultTextEdit->setPlaceholderText("Test output will appear here.");
    m_testPaneSplitter->setStretchFactor(0, 1);
    m_testPaneSplitter->setStretchFactor(1, 1);
    m_testPaneSplitter->setSizes({320, 320});

    m_submitResultTextEdit = new QTextEdit(m_resultStack);
    m_submitResultTextEdit->setObjectName("problemResultText");
    m_submitResultTextEdit->setReadOnly(true);
    m_submitResultTextEdit->setMinimumHeight(140);
    m_resultStack->addWidget(m_testPaneSplitter);
    m_resultStack->addWidget(m_submitResultTextEdit);

    auto *resultPanel = new QWidget(submitFrame);
    auto *resultPanelLayout = new QVBoxLayout(resultPanel);
    resultPanelLayout->setContentsMargins(0, 0, 0, 0);
    resultPanelLayout->setSpacing(14);
    resultPanelLayout->addLayout(resultTabLayout);
    resultPanelLayout->addWidget(m_resultStack, 1);

    m_submitPaneSplitter = new QSplitter(Qt::Vertical, submitFrame);
    m_submitPaneSplitter->setObjectName("problemSubmitPaneSplitter");
    m_submitPaneSplitter->setChildrenCollapsible(false);
    m_submitPaneSplitter->setHandleWidth(8);
    m_submitPaneSplitter->addWidget(m_codeEdit);
    m_submitPaneSplitter->addWidget(resultPanel);
    m_submitPaneSplitter->setStretchFactor(0, 3);
    m_submitPaneSplitter->setStretchFactor(1, 2);
    m_submitPaneSplitter->setSizes({360, 240});

    submitLayout->addWidget(submitLabel);
    submitLayout->addWidget(m_languageComboBox);
    submitLayout->addWidget(m_submitPaneSplitter, 1);

    m_aiFrame = new QFrame(this);
    m_aiFrame->setObjectName("problemAiFrame");
    auto *aiLayout = new QVBoxLayout(m_aiFrame);
    aiLayout->setContentsMargins(20, 18, 20, 18);
    aiLayout->setSpacing(14);

    auto *aiLabel = new QLabel("AI", m_aiFrame);
    aiLabel->setObjectName("problemSectionLabel");
    m_aiConfigLabel = new QLabel("Config: using defaults", m_aiFrame);
    m_aiConfigLabel->setObjectName("problemAiMetaLabel");
    auto *aiPromptLabel = new QLabel("Prompt", m_aiFrame);
    aiPromptLabel->setObjectName("problemAiFieldLabel");
    m_aiPromptEdit = new QPlainTextEdit(m_aiFrame);
    m_aiPromptEdit->setObjectName("problemCodeEdit");
    m_aiPromptEdit->setPlaceholderText("Ask AI about the current problem, code, or test result.");
    m_aiPromptEdit->setMinimumHeight(120);
    m_aiAskButton = new QPushButton("Ask AI", m_aiFrame);
    m_aiAskButton->setObjectName("problemSubmitButton");
    auto *aiResponseLabel = new QLabel("Response", m_aiFrame);
    aiResponseLabel->setObjectName("problemAiFieldLabel");
    m_aiResponseTextEdit = new QTextEdit(m_aiFrame);
    m_aiResponseTextEdit->setObjectName("problemResultText");
    m_aiResponseTextEdit->setReadOnly(true);
    m_aiResponseTextEdit->setPlaceholderText("AI response will appear here.");
    aiLayout->addWidget(aiLabel);
    aiLayout->addWidget(m_aiConfigLabel);
    aiLayout->addWidget(aiPromptLabel);
    aiLayout->addWidget(m_aiPromptEdit);
    aiLayout->addWidget(m_aiAskButton, 0, Qt::AlignRight);
    aiLayout->addWidget(aiResponseLabel);
    aiLayout->addWidget(m_aiResponseTextEdit, 1);
    m_aiFrame->hide();

    auto *contentSplitter = new QSplitter(Qt::Horizontal, this);
    contentSplitter->setObjectName("problemContentSplitter");
    contentSplitter->setChildrenCollapsible(false);
    contentSplitter->setHandleWidth(4);
    contentSplitter->addWidget(problemFrame);
    m_workspaceSplitter = new QSplitter(Qt::Horizontal, contentSplitter);
    m_workspaceSplitter->setObjectName("problemWorkspaceSplitter");
    m_workspaceSplitter->setChildrenCollapsible(false);
    m_workspaceSplitter->setHandleWidth(4);
    m_workspaceSplitter->addWidget(submitFrame);
    m_workspaceSplitter->addWidget(m_aiFrame);
    m_workspaceSplitter->setStretchFactor(0, 4);
    m_workspaceSplitter->setStretchFactor(1, 2);
    m_workspaceSplitter->setSizes({720, 0});
    contentSplitter->addWidget(m_workspaceSplitter);
    contentSplitter->setStretchFactor(0, 1);
    contentSplitter->setStretchFactor(1, 1);
    contentSplitter->setSizes({640, 640});

    bottomLayout->addWidget(m_toolsFrame, 1);
    bottomLayout->addWidget(contentSplitter, 4);

    layout->addWidget(topFrame);
    layout->addLayout(bottomLayout, 1);

    setStyleSheet(
        "ProblemPage { background: #f3f1eb; }"
        "#problemTopFrame, #problemLeftFrame, #problemMiddleFrame, #problemRightFrame, #problemAiFrame {"
        "  background: #fbfaf7;"
        "  border: 1px solid #ded8cc;"
        "  border-radius: 16px;"
        "}"
        "#problemContentSplitter::handle {"
        "  background: transparent;"
        "}"
        "#problemContentSplitter::handle:hover {"
        "  background: rgba(18, 52, 59, 0.08);"
        "}"
        "#problemSubmitPaneSplitter::handle {"
        "  background: transparent;"
        "}"
        "#problemSubmitPaneSplitter::handle:hover {"
        "  background: rgba(18, 52, 59, 0.08);"
        "}"
        "#problemWorkspaceSplitter::handle {"
        "  background: transparent;"
        "}"
        "#problemWorkspaceSplitter::handle:hover {"
        "  background: rgba(18, 52, 59, 0.08);"
        "}"
        "#problemTestPaneSplitter::handle {"
        "  background: transparent;"
        "}"
        "#problemTestPaneSplitter::handle:hover {"
        "  background: rgba(18, 52, 59, 0.08);"
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
        "#problemAiMetaLabel, #problemAiFieldLabel {"
        "  color: #526056;"
        "}"
        "#problemToolsToggleButton {"
        "  background: transparent;"
        "  border: none;"
        "  padding: 0px;"
        "  text-align: left;"
        "  font-size: 16px;"
        "  font-weight: 600;"
        "  color: #2f3a33;"
        "}"
        "#problemToolsToggleButton:hover {"
        "  color: #12343b;"
        "}"
        "#problemBackButton, #problemSubmitButton, #problemInputButton {"
        "  min-width: 88px;"
        "  padding: 8px 14px;"
        "  border: 1px solid #cdd7cf;"
        "  border-radius: 10px;"
        "  background: #f7f5ef;"
        "  color: #243029;"
        "}"
        "#problemBackButton:hover, #problemSubmitButton:hover, #problemInputButton:hover {"
        "  background: #eef4ef;"
        "}"
        "#problemResultTabButton {"
        "  min-width: 72px;"
        "  padding: 6px 12px;"
        "  border: 1px solid #d9d4c8;"
        "  border-radius: 10px;"
        "  background: #f7f5ef;"
        "  color: #526056;"
        "}"
        "#problemResultTabButton:checked {"
        "  background: #e4efe7;"
        "  color: #1f2328;"
        "  border-color: #b9cabd;"
        "}"
        "#problemToolButton {"
        "  padding: 10px 12px;"
        "  border: none;"
        "  border-radius: 10px;"
        "  background: transparent;"
        "  color: #2f3a33;"
        "  text-align: left;"
        "}"
        "#problemToolButton:hover, #problemToolIconButton:hover {"
        "  background: #eef4ef;"
        "}"
        "#problemToolIconButton {"
        "  min-width: 36px;"
        "  max-width: 36px;"
        "  min-height: 36px;"
        "  max-height: 36px;"
        "  border: none;"
        "  border-radius: 10px;"
        "  background: transparent;"
        "  color: #2f3a33;"
        "  font-weight: 600;"
        "}"
        "#problemDetailText, #problemResultText, #problemCodeEdit, #problemLanguageCombo {"
        "  background: #fcfbf8;"
        "  border: 1px solid #d9d4c8;"
        "  border-radius: 12px;"
        "  color: #1f2328;"
        "}"
    );

    setToolsExpanded(true);

    connect(backButton, &QPushButton::clicked, this, &ProblemPage::backRequested);
    connect(m_backToolButton, &QPushButton::clicked, this, &ProblemPage::backRequested);
    connect(m_collapsedBackButton, &QPushButton::clicked, this, &ProblemPage::backRequested);
    connect(m_favoriteToolButton, &QPushButton::clicked, this, &ProblemPage::favoriteRequested);
    connect(m_collapsedFavoriteButton, &QPushButton::clicked, this, &ProblemPage::favoriteRequested);
    connect(m_aiToolButton, &QPushButton::clicked, this, [this]() {
        setAiPanelVisible(!m_aiPanelVisible);
    });
    connect(m_collapsedAiButton, &QPushButton::clicked, this, [this]() {
        setAiPanelVisible(!m_aiPanelVisible);
    });
    connect(
        m_toolsToggleButton,
        &QPushButton::clicked,
        this,
        [this]() {
            setToolsExpanded(!m_toolsExpanded);
        });
    connect(m_aiAskButton, &QPushButton::clicked, this, [this]() {
        const QString question = m_aiPromptEdit->toPlainText().trimmed();
        if (!question.isEmpty()) {
            appendAiTranscriptBlock("User", question);
            refreshAiResponseView();
        }
        emit aiAskRequested(question);
    });
    connect(
        m_inputButton,
        &QPushButton::clicked,
        this,
        [this]() {
            setResultTab(true);
            emit testRequested(currentLanguageLabel(),
                               m_codeEdit->toPlainText(),
                               m_testInputTextEdit->toPlainText());
        });
    connect(
        m_submitButton,
        &QPushButton::clicked,
        this,
        [this]() {
            setResultTab(false);
            emit submitRequested(
                m_languageComboBox->currentData().toString(),
                m_codeEdit->toPlainText());
        });
    connect(
        m_testTabButton,
        &QPushButton::clicked,
        this,
        [this]() {
            setResultTab(true);
        });
    connect(
        m_submitTabButton,
        &QPushButton::clicked,
        this,
        [this]() {
            setResultTab(false);
        });

    openProblem();
}

void ProblemPage::setToolsExpanded(bool expanded)
{
    m_toolsExpanded = expanded;
    if (m_toolsFrame != nullptr) {
        m_toolsFrame->setMinimumWidth(expanded ? 0 : 84);
        m_toolsFrame->setMaximumWidth(expanded ? QWIDGETSIZE_MAX : 84);
    }
    if (m_toolsPanel != nullptr) {
        m_toolsPanel->setVisible(expanded);
    }
    if (m_collapsedToolsPanel != nullptr) {
        m_collapsedToolsPanel->setVisible(!expanded);
    }
    const QList<QPushButton *> iconButtons = {m_collapsedBackButton,
                                              m_collapsedFavoriteButton,
                                              m_collapsedAiButton};
    for (QPushButton *button : iconButtons) {
        if (button != nullptr) {
            button->setVisible(!expanded);
        }
    }
    if (m_toolsToggleButton != nullptr) {
        m_toolsToggleButton->setText(expanded ? "Tools v" : ">");
    }
}

void ProblemPage::setAiPanelVisible(bool visible)
{
    m_aiPanelVisible = visible;
    if (m_workspaceSplitter == nullptr || m_workspaceSplitter->count() < 2
        || m_aiFrame == nullptr) {
        return;
    }

    QList<int> sizes = m_workspaceSplitter->sizes();
    if (sizes.size() < 2) {
        sizes = {720, 0};
    }

    if (visible) {
        m_aiFrame->show();
        if (sizes.at(1) <= 0) {
            sizes[0] = qMax(420, sizes.at(0));
            sizes[1] = 320;
        }
    } else {
        sizes[0] += sizes[1];
        sizes[1] = 0;
        m_workspaceSplitter->setSizes(sizes);
        m_aiFrame->hide();
        return;
    }
    m_workspaceSplitter->setSizes(sizes);
}

void ProblemPage::setResultTab(bool showTestTab)
{
    if (m_resultStack != nullptr) {
        m_resultStack->setCurrentIndex(showTestTab ? 0 : 1);
    }
    if (m_testTabButton != nullptr) {
        m_testTabButton->setChecked(showTestTab);
    }
    if (m_submitTabButton != nullptr) {
        m_submitTabButton->setChecked(!showTestTab);
    }
    if (m_submitButton != nullptr) {
        const bool canSubmit = m_languageComboBox != nullptr
                               && m_languageComboBox->count() > 0
                               && m_languageComboBox->isEnabled();
        m_submitButton->setVisible(!showTestTab);
        m_submitButton->setEnabled(!showTestTab && canSubmit);
    }
    if (m_inputButton != nullptr) {
        m_inputButton->setVisible(showTestTab);
        m_inputButton->setEnabled(showTestTab && !m_testing);
    }
}

void ProblemPage::openProblem(const QString &problemTitle)
{
    m_titleLabel->setText(problemTitle.isEmpty() ? "Problem" : problemTitle);
    m_detailTextEdit->setPlainText("Loading problem detail...");
    resetSubmitPanel();
    m_aiTranscript.clear();
    m_aiResponseBuffer.clear();
    refreshAiResponseView();
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
    m_submitButton->setEnabled(hasLanguages && m_submitTabButton->isChecked());
    m_submitResultTextEdit->setPlainText(
        QString("Submit page loaded.\nAction: %1")
            .arg(submitPageInfo.submitActionUrl));
}

void ProblemPage::showSubmitting(bool submitting)
{
    m_submitButton->setEnabled(
        !submitting && m_submitTabButton->isChecked() && m_languageComboBox->count() > 0
        && m_languageComboBox->isEnabled());
}

void ProblemPage::showTesting(bool testing)
{
    m_testing = testing;
    if (testing) {
        setResultTab(true);
        m_testResultTextEdit->setPlainText("Running test...");
    }
    if (m_inputButton != nullptr) {
        m_inputButton->setEnabled(m_testTabButton->isChecked() && !testing);
    }
}

void ProblemPage::showMissingLanguage()
{
    setResultTab(false);
    m_submitResultTextEdit->setPlainText("No language selected.");
}

void ProblemPage::showTestResult(const QString &text)
{
    setResultTab(true);
    m_testResultTextEdit->setPlainText(text);
}

void ProblemPage::showTestFailed(const QString &message)
{
    setResultTab(true);
    m_testResultTextEdit->setPlainText(message);
}

void ProblemPage::showSubmitPayloadBuilt(const QString &languageValue,
                                         const QByteArray &payload)
{
    m_lastSubmitPreview = formatSubmitPayloadPreview(
        currentLanguageLabel(),
        languageValue,
        m_codeEdit->toPlainText(),
        payload);
    setResultTab(false);
    m_submitResultTextEdit->setPlainText(m_lastSubmitPreview);
}

void ProblemPage::showSubmitResult(const NetworkResult &result)
{
    const QString resultText = formatSubmitResult(result);
    setResultTab(false);
    m_submitResultTextEdit->setPlainText(
        m_lastSubmitPreview.isEmpty()
            ? resultText
            : m_lastSubmitPreview + "\n\n" + resultText);
}

void ProblemPage::showSubmitFailed(const QString &message)
{
    setResultTab(false);
    m_submitResultTextEdit->setPlainText(message);
}

void ProblemPage::setAiConfigSummary(const QString &summary)
{
    if (m_aiConfigLabel != nullptr) {
        m_aiConfigLabel->setText(summary);
    }
}

void ProblemPage::showAiThinking(bool thinking)
{
    m_aiThinking = thinking;
    if (m_aiAskButton != nullptr) {
        m_aiAskButton->setEnabled(!thinking);
        m_aiAskButton->setText(thinking ? "Asking..." : "Ask AI");
    }
    if (thinking) {
        m_aiResponseBuffer.clear();
        appendAiTranscriptBlock("Assistant", "Waiting for OpenAI response...");
        refreshAiResponseView();
    }
}

void ProblemPage::appendAiResponse(const QString &delta)
{
    m_aiResponseBuffer += delta;
    const QString waitingBlock = "Assistant\nWaiting for OpenAI response...";
    if (m_aiTranscript.endsWith(waitingBlock)) {
        m_aiTranscript.chop(waitingBlock.size());
        m_aiTranscript += "Assistant\n" + m_aiResponseBuffer;
    } else {
        const QString assistantPrefix = "Assistant\n";
        const int assistantIndex = m_aiTranscript.lastIndexOf(assistantPrefix);
        if (assistantIndex >= 0) {
            m_aiTranscript = m_aiTranscript.left(assistantIndex) + assistantPrefix
                             + m_aiResponseBuffer;
        } else {
            appendAiTranscriptBlock("Assistant", m_aiResponseBuffer);
        }
    }
    refreshAiResponseView();
}

void ProblemPage::showAiResponse(const QString &text)
{
    m_aiResponseBuffer = text;
    const QString assistantPrefix = "Assistant\n";
    const int assistantIndex = m_aiTranscript.lastIndexOf(assistantPrefix);
    if (assistantIndex >= 0) {
        m_aiTranscript = m_aiTranscript.left(assistantIndex) + assistantPrefix + text;
    } else {
        appendAiTranscriptBlock("Assistant", text);
    }
    refreshAiResponseView();
}

void ProblemPage::showAiFailed(const QString &message)
{
    const QString assistantPrefix = "Assistant\n";
    const int assistantIndex = m_aiTranscript.lastIndexOf(assistantPrefix);
    if (assistantIndex >= 0) {
        m_aiTranscript = m_aiTranscript.left(assistantIndex) + assistantPrefix + message;
    } else {
        appendAiTranscriptBlock("Assistant", message);
    }
    refreshAiResponseView();
}

void ProblemPage::setSourceCodeText(const QString &text)
{
    if (m_codeEdit != nullptr) {
        m_codeEdit->setPlainText(text);
    }
}

void ProblemPage::setTestInputText(const QString &text)
{
    if (m_testInputTextEdit != nullptr) {
        m_testInputTextEdit->setPlainText(text);
    }
}

void ProblemPage::appendResultPageInfo(const ResultPageInfo &resultPageInfo)
{
    setResultTab(false);
    m_submitResultTextEdit->append("\n\n" + formatResultPageInfo(resultPageInfo));
}

void ProblemPage::appendResultFailure(const QString &message)
{
    setResultTab(false);
    m_submitResultTextEdit->append("\n\nResult page load failed:\n" + message);
}

QString ProblemPage::currentLanguageLabel() const
{
    return m_languageComboBox->currentText();
}

QString ProblemPage::currentLanguageValue() const
{
    return m_languageComboBox->currentData().toString();
}

QString ProblemPage::currentProblemDetailText() const
{
    return m_detailTextEdit == nullptr ? QString() : m_detailTextEdit->toPlainText();
}

QString ProblemPage::currentSourceCode() const
{
    return m_codeEdit == nullptr ? QString() : m_codeEdit->toPlainText();
}

QString ProblemPage::currentTestInput() const
{
    return m_testInputTextEdit == nullptr ? QString() : m_testInputTextEdit->toPlainText();
}

QString ProblemPage::currentTestOutput() const
{
    return m_testResultTextEdit == nullptr ? QString() : m_testResultTextEdit->toPlainText();
}

void ProblemPage::setFavoriteEnabled(bool enabled)
{
    if (m_favoriteToolButton != nullptr) {
        m_favoriteToolButton->setEnabled(enabled);
    }
    if (m_collapsedFavoriteButton != nullptr) {
        m_collapsedFavoriteButton->setEnabled(enabled);
    }
}

void ProblemPage::setSubmitEnabled(bool enabled)
{
    m_languageComboBox->setEnabled(enabled && m_languageComboBox->count() > 0);
    m_codeEdit->setEnabled(enabled && m_languageComboBox->count() > 0);
    m_submitButton->setEnabled(
        enabled && m_submitTabButton->isChecked() && m_languageComboBox->count() > 0);
    if (m_inputButton != nullptr) {
        m_inputButton->setEnabled(
            enabled && m_testTabButton->isChecked() && !m_testing
            && m_languageComboBox->count() > 0);
    }
}

void ProblemPage::resetSubmitPanel()
{
    m_testing = false;
    m_languageComboBox->clear();
    m_languageComboBox->setEnabled(false);
    m_codeEdit->setEnabled(false);
    m_submitButton->setEnabled(false);
    m_inputButton->setEnabled(false);
    m_testInputTextEdit->clear();
    m_testResultTextEdit->clear();
    m_submitResultTextEdit->setPlainText("Preparing submit options...");
    setResultTab(true);
    m_lastSubmitPreview.clear();
}

void ProblemPage::appendAiTranscriptBlock(const QString &title, const QString &body)
{
    if (!m_aiTranscript.isEmpty()) {
        m_aiTranscript += "\n\n";
    }
    m_aiTranscript += title + "\n" + body;
}

void ProblemPage::refreshAiResponseView()
{
    if (m_aiResponseTextEdit != nullptr) {
        if (m_aiTranscript.isEmpty()) {
            m_aiResponseTextEdit->clear();
        } else {
            m_aiResponseTextEdit->setPlainText(m_aiTranscript);
        }
    }
}
