#pragma once

#include "repository/meta/problemmetarepository.h"

#include <QList>
#include <QWidget>

class QLabel;
class QListWidget;
class QListWidgetItem;
class QPushButton;
class QFrame;

class TaskBoardPage : public QWidget
{
    Q_OBJECT

public:
    explicit TaskBoardPage(QWidget *parent = nullptr);
    void setDarkMode(bool dark);

    void showTasks(const QList<ProblemMeta> &allMeta);

signals:
    void homeRequested();
    void themeToggleRequested(bool dark);
    void refreshRequested();
    void problemSelected(const QString &title, const QString &url);
    void statusChangeRequested(const QString &url, const QString &newStatus);

private:
    QListWidget *listForStatus(const QString &status) const;
    void showStatusMenu(QListWidget *list, const QPoint &pos);

    QListWidget *m_todoList = nullptr;
    QListWidget *m_doingList = nullptr;
    QListWidget *m_doneList = nullptr;
    QListWidget *m_redoList = nullptr;
    QLabel *m_statusLabel = nullptr;
    bool m_darkMode = false;
};
