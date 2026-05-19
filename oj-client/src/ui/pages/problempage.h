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
    void showMissingLanguage();
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
    void submitRequested(const QString &language, const QString &sourceText);

private:
    void resetSubmitPanel();

    QLabel *m_titleLabel = nullptr;
    QListWidget *m_toolsListWidget = nullptr;
    QTextEdit *m_detailTextEdit = nullptr;
    QComboBox *m_languageComboBox = nullptr;
    QPlainTextEdit *m_codeEdit = nullptr;
    QPushButton *m_submitButton = nullptr;
    QTextEdit *m_resultTextEdit = nullptr;
    QString m_lastSubmitPreview;
};
