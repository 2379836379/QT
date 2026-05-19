#pragma once

#include "parser/loginparser.h"
#include "service/reminder/reminderservice.h"

#include <QWidget>

class QLabel;
class QListWidget;
class QListWidgetItem;
class QPushButton;
class QFrame;

class HomePage : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = nullptr);

    void showLoggingIn();
    void showLoginFailed(const QString &message);
    void showOpeningHome();
    void showLoadingClasses();
    void showHomeLoadFailed(const QString &message);
    void showClasses(const QList<JoinedClassInfo> &classes);
    void showReminderLoading();
    void showReminders(const QList<DeadlineReminder> &reminders);

signals:
    void classSelected(const QString &name, const QString &url);
    void favoritesRequested();
    void logoutRequested();
    void reminderSelected(const QString &title, const QString &url);

private:
    QFrame *m_contentCard = nullptr;
    QListWidget *m_classListWidget = nullptr;
    QListWidget *m_toolsListWidget = nullptr;
    QListWidget *m_reminderListWidget = nullptr;
};
