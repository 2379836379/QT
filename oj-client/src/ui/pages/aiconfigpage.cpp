#include "ui/pages/aiconfigpage.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

AiConfigPage::AiConfigPage(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(18);

    auto *topFrame = new QFrame(this);
    topFrame->setObjectName("aiConfigTopFrame");
    topFrame->setFixedHeight(80);
    auto *topLayout = new QHBoxLayout(topFrame);
    topLayout->setContentsMargins(24, 18, 24, 18);
    topLayout->setSpacing(16);

    auto *backButton = new QPushButton("Back", topFrame);
    backButton->setObjectName("aiConfigBackButton");
    auto *titleLabel = new QLabel("AI Config", topFrame);
    titleLabel->setObjectName("aiConfigTitleLabel");
    topLayout->addWidget(backButton);
    topLayout->addWidget(titleLabel, 1);

    auto *contentFrame = new QFrame(this);
    contentFrame->setObjectName("aiConfigContentFrame");
    auto *contentLayout = new QVBoxLayout(contentFrame);
    contentLayout->setContentsMargins(24, 22, 24, 22);
    contentLayout->setSpacing(14);

    auto *pathTitle = new QLabel("Config File", contentFrame);
    pathTitle->setObjectName("aiConfigFieldLabel");
    m_pathLabel = new QLabel("--", contentFrame);
    m_pathLabel->setObjectName("aiConfigPathLabel");
    m_pathLabel->setWordWrap(true);

    auto *editorLabel = new QLabel("config.toml", contentFrame);
    editorLabel->setObjectName("aiConfigFieldLabel");
    m_configEdit = new QPlainTextEdit(contentFrame);
    m_configEdit->setObjectName("aiConfigPromptEdit");
    m_configEdit->setMinimumHeight(360);

    m_statusLabel = new QLabel(contentFrame);
    m_statusLabel->setObjectName("aiConfigStatusLabel");
    m_statusLabel->setWordWrap(true);

    auto *saveButton = new QPushButton("Save", contentFrame);
    saveButton->setObjectName("aiConfigSaveButton");

    contentLayout->addWidget(pathTitle);
    contentLayout->addWidget(m_pathLabel);
    contentLayout->addWidget(editorLabel);
    contentLayout->addWidget(m_configEdit, 1);
    contentLayout->addWidget(m_statusLabel);
    contentLayout->addWidget(saveButton, 0, Qt::AlignLeft);

    layout->addWidget(topFrame);
    layout->addWidget(contentFrame, 1);

    setStyleSheet(
        "AiConfigPage { background: #f3f1eb; }"
        "#aiConfigTopFrame, #aiConfigContentFrame {"
        "  background: #fbfaf7;"
        "  border: 1px solid #ded8cc;"
        "  border-radius: 16px;"
        "}"
        "#aiConfigBackButton, #aiConfigSaveButton {"
        "  padding: 10px 14px;"
        "  border: 1px solid #d7d2c7;"
        "  border-radius: 10px;"
        "  background: #f6f3ec;"
        "  color: #2f3a33;"
        "}"
        "#aiConfigBackButton:hover, #aiConfigSaveButton:hover {"
        "  background: #eef4ef;"
        "}"
        "#aiConfigTitleLabel { font-size: 26px; font-weight: 600; color: #1f2328; }"
        "#aiConfigFieldLabel { font-size: 15px; font-weight: 600; color: #2f3a33; }"
        "#aiConfigPathLabel, #aiConfigStatusLabel { color: #526056; }"
        "#aiConfigPromptEdit {"
        "  background: #fcfbf8;"
        "  border: 1px solid #d9d4c8;"
        "  border-radius: 12px;"
        "  color: #1f2328;"
        "  padding: 8px 10px;"
        "}"
    );

    connect(backButton, &QPushButton::clicked, this, &AiConfigPage::backRequested);
    connect(saveButton, &QPushButton::clicked, this, [this]() {
        emit saveRequested(currentConfigText());
    });
}

void AiConfigPage::setConfigText(const QString &path, const QString &content)
{
    m_pathLabel->setText(path.isEmpty() ? "config.toml" : path);
    m_configEdit->setPlainText(content);
    m_statusLabel->clear();
}

QString AiConfigPage::currentConfigText() const
{
    return m_configEdit->toPlainText();
}

void AiConfigPage::showSaveSucceeded(const QString &path)
{
    m_pathLabel->setText(path);
    m_statusLabel->setText("Saved.");
}

void AiConfigPage::showSaveFailed(const QString &message)
{
    m_statusLabel->setText(message);
}
