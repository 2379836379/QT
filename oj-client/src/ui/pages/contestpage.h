#pragma once

#include "parser/contestparser.h"

#include <QWidget>

class QLabel;
class QListWidget;
class QListWidgetItem;

class ContestPage : public QWidget
{
    Q_OBJECT

public:
    explicit ContestPage(QWidget *parent = nullptr);

    void openContest(const QString &title, const QString &url);
    void showLoadingProblems();
    void showContestLoadFailed(const QString &message);
    void showProblems(const ContestPageInfo &contestPageInfo);

signals:
    void backRequested();
    void problemSelected(const QString &title, const QString &url);

private:
    QLabel *m_titleLabel = nullptr;
    QListWidget *m_toolsListWidget = nullptr;
    QListWidget *m_problemListWidget = nullptr;
    QString m_contestTitle;
    QString m_contestUrl;
};
