#include "ui/pages/loginpage.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

LoginPage::LoginPage(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    auto *shell = new QFrame(this);
    shell->setObjectName("loginShell");
    auto *shellLayout = new QVBoxLayout(shell);
    shellLayout->setContentsMargins(0, 0, 0, 0);
    shellLayout->addStretch();

    auto *rowLayout = new QHBoxLayout();
    rowLayout->addStretch();

    auto *card = new QFrame(shell);
    card->setObjectName("loginCard");
    card->setFixedWidth(420);
    auto *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(28, 28, 28, 28);
    cardLayout->setSpacing(14);

    auto *titleLabel = new QLabel("OpenJudge Login", card);
    titleLabel->setObjectName("loginTitleLabel");

    auto *emailLabel = new QLabel("Email", card);
    auto *passwordLabel = new QLabel("Password", card);

    m_emailEdit = new QLineEdit(card);
    m_emailEdit->setPlaceholderText("name@example.com");
    m_passwordEdit = new QLineEdit(card);
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setPlaceholderText("Password");

    m_statusLabel = new QLabel(card);
    m_statusLabel->setObjectName("loginStatusLabel");
    m_statusLabel->setWordWrap(true);

    m_loginButton = new QPushButton("Login", card);
    m_loginButton->setObjectName("loginButton");

    cardLayout->addWidget(titleLabel);
    cardLayout->addWidget(emailLabel);
    cardLayout->addWidget(m_emailEdit);
    cardLayout->addWidget(passwordLabel);
    cardLayout->addWidget(m_passwordEdit);
    cardLayout->addWidget(m_statusLabel);
    cardLayout->addWidget(m_loginButton);

    rowLayout->addWidget(card);
    rowLayout->addStretch();
    shellLayout->addLayout(rowLayout);
    shellLayout->addStretch();

    layout->addWidget(shell);

    setStyleSheet(
        "LoginPage { background: #efe9df; }"
        "#loginCard {"
        "  background: #fbfaf7;"
        "  border: 1px solid #ded8cc;"
        "  border-radius: 18px;"
        "}"
        "#loginTitleLabel {"
        "  font-size: 28px;"
        "  font-weight: 600;"
        "  color: #1f2328;"
        "  margin-bottom: 8px;"
        "}"
        "QLineEdit {"
        "  padding: 10px 12px;"
        "  border: 1px solid #d4cec2;"
        "  border-radius: 10px;"
        "  background: #fcfbf8;"
        "}"
        "#loginButton {"
        "  padding: 10px 14px;"
        "  border: 1px solid #cdd7cf;"
        "  border-radius: 10px;"
        "  background: #dcefea;"
        "  color: #12343b;"
        "  font-weight: 600;"
        "}"
        "#loginButton:hover {"
        "  background: #cfe6de;"
        "}"
        "#loginStatusLabel {"
        "  color: #7a4b36;"
        "  min-height: 20px;"
        "}"
    );

    connect(
        m_loginButton,
        &QPushButton::clicked,
        this,
        [this]() {
            emit loginRequested(
                m_emailEdit->text().trimmed(),
                m_passwordEdit->text());
        });
    connect(
        m_passwordEdit,
        &QLineEdit::returnPressed,
        this,
        [this]() {
            emit loginRequested(
                m_emailEdit->text().trimmed(),
                m_passwordEdit->text());
        });

    showIdle();
}

void LoginPage::showIdle()
{
    m_emailEdit->setEnabled(true);
    m_passwordEdit->setEnabled(true);
    m_loginButton->setEnabled(true);
    m_statusLabel->clear();
}

void LoginPage::showLoggingIn()
{
    m_emailEdit->setEnabled(false);
    m_passwordEdit->setEnabled(false);
    m_loginButton->setEnabled(false);
    m_statusLabel->setText("Logging in...");
}

void LoginPage::showLoginFailed(const QString &message)
{
    m_emailEdit->setEnabled(true);
    m_passwordEdit->setEnabled(true);
    m_loginButton->setEnabled(true);
    m_statusLabel->setText(message);
}
