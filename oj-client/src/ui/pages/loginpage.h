#pragma once

#include <QWidget>

class QLabel;
class QLineEdit;
class QPushButton;

class LoginPage : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);

    void showIdle();
    void showLoggingIn();
    void showLoginFailed(const QString &message);

signals:
    void loginRequested(const QString &email, const QString &password);

private:
    QLabel *m_statusLabel = nullptr;
    QLineEdit *m_emailEdit = nullptr;
    QLineEdit *m_passwordEdit = nullptr;
    QPushButton *m_loginButton = nullptr;
};
