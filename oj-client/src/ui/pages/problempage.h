#pragma once

#include "network/openjudgeclient.h"
#include "parser/problemparser.h"
#include "parser/resultparser.h"
#include "parser/submitparser.h"

#include <QWidget>

class QComboBox;
class QLabel;
class QPlainTextEdit;
class QPushButton;
class QTextEdit;
class QListWidget;
class QListWidgetItem;
class QFrame;
class QSplitter;
class QStackedWidget;
class QWidget;

class ProblemPage : public QWidget
{
    Q_OBJECT

public:
    explicit ProblemPage(QWidget *parent = nullptr);

    void openProblem(const QString &problemTitle = QString());
    void showProblemLoadedFromFavorites(const ProblemPageInfo &problemPageInfo);
    void showProblem(const ProblemPageInfo &problemPageInfo);
    void showProblemLoadFailed(const QString &message);
    void openSubmit(const ProblemPageInfo &problemPageInfo);
    void showLoadingSubmitOptions(bool loading);
    void showSubmitPageLoaded(const SubmitPageInfo &submitPageInfo,
                              const QString &defaultLanguage);
    void showSubmitting(bool submitting);
    void showTesting(bool testing);
    void showMissingLanguage();
    void showTestResult(const QString &text);
    void showTestFailed(const QString &message);
    void showSubmitPayloadBuilt(const QString &languageValue,
                                const QByteArray &payload);
    void showSubmitResult(const NetworkResult &result);
    void showSubmitFailed(const QString &message);
    void appendResultPageInfo(const ResultPageInfo &resultPageInfo);
    void appendResultFailure(const QString &message);
    QString currentLanguageLabel() const;
    void setFavoriteEnabled(bool enabled);
    void setSubmitEnabled(bool enabled);

signals:
    void backRequested();
    void favoriteRequested();
    void testRequested(const QString &languageLabel,
                       const QString &sourceText,
                       const QString &stdinText);
    void submitRequested(const QString &language, const QString &sourceText);

private:
    void setToolsExpanded(bool expanded);
    void setResultTab(bool showTestTab);
    void resetSubmitPanel();

    QLabel *m_titleLabel = nullptr;
    QFrame *m_toolsFrame = nullptr;
    QPushButton *m_toolsToggleButton = nullptr;
    QWidget *m_toolsPanel = nullptr;
    QWidget *m_collapsedToolsPanel = nullptr;
    QPushButton *m_backToolButton = nullptr;
    QPushButton *m_favoriteToolButton = nullptr;
    QPushButton *m_collapsedBackButton = nullptr;
    QPushButton *m_collapsedFavoriteButton = nullptr;
    QTextEdit *m_detailTextEdit = nullptr;
    QComboBox *m_languageComboBox = nullptr;
    QPlainTextEdit *m_codeEdit = nullptr;
    QPushButton *m_submitButton = nullptr;
    QPushButton *m_inputButton = nullptr;
    QPushButton *m_testTabButton = nullptr;
    QPushButton *m_submitTabButton = nullptr;
    QSplitter *m_submitPaneSplitter = nullptr;
    QStackedWidget *m_resultStack = nullptr;
    QSplitter *m_testPaneSplitter = nullptr;
    QTextEdit *m_testInputTextEdit = nullptr;
    QTextEdit *m_testResultTextEdit = nullptr;
    QTextEdit *m_submitResultTextEdit = nullptr;
    QString m_lastSubmitPreview;
    bool m_toolsExpanded = true;
    bool m_testing = false;
};
