#pragma once

#include "parser/groupparser.h"

#include <QWidget>

class QLabel;
class QListWidget;
class QListWidgetItem;
class QPushButton;

class ClassPage : public QWidget
{
    Q_OBJECT

public:
    explicit ClassPage(QWidget *parent = nullptr);

    void openClass(const QString &name, const QString &url);
    void showLoadingContests();
    void showClassLoadFailed(const QString &message);
    void showContestSets(const GroupPageInfo &groupPageInfo);

signals:
    void backRequested();
    void contestSelected(const QString &title, const QString &url);

private:
    QLabel *m_titleLabel = nullptr;
    QListWidget *m_toolsListWidget = nullptr;
    QListWidget *m_contestListWidget = nullptr;
    QString m_className;
    QString m_classUrl;
};
