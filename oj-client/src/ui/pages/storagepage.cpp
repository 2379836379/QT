#include "ui/pages/storagepage.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

StoragePage::StoragePage(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(18);

    auto *topFrame = new QFrame(this);
    topFrame->setObjectName("storageTopFrame");
    topFrame->setFixedHeight(80);
    auto *topLayout = new QHBoxLayout(topFrame);
    topLayout->setContentsMargins(24, 18, 24, 18);
    topLayout->setSpacing(16);

    auto *backButton = new QPushButton("Back", topFrame);
    backButton->setObjectName("storageBackButton");
    auto *titleLabel = new QLabel("Storage", topFrame);
    titleLabel->setObjectName("storageTitleLabel");

    topLayout->addWidget(backButton);
    topLayout->addWidget(titleLabel, 1);

    auto *contentFrame = new QFrame(this);
    contentFrame->setObjectName("storageContentFrame");
    auto *contentLayout = new QVBoxLayout(contentFrame);
    contentLayout->setContentsMargins(24, 22, 24, 22);
    contentLayout->setSpacing(18);

    auto *cacheRow = new QHBoxLayout();
    auto *cacheLabel = new QLabel("Cache Size", contentFrame);
    cacheLabel->setObjectName("storageLabel");
    m_cacheSizeValueLabel = new QLabel("--", contentFrame);
    m_cacheSizeValueLabel->setObjectName("storageValueLabel");
    cacheRow->addWidget(cacheLabel);
    cacheRow->addStretch();
    cacheRow->addWidget(m_cacheSizeValueLabel);

    auto *appRow = new QHBoxLayout();
    auto *appLabel = new QLabel("Application Size", contentFrame);
    appLabel->setObjectName("storageLabel");
    m_appSizeValueLabel = new QLabel("--", contentFrame);
    m_appSizeValueLabel->setObjectName("storageValueLabel");
    appRow->addWidget(appLabel);
    appRow->addStretch();
    appRow->addWidget(m_appSizeValueLabel);

    m_statusLabel = new QLabel(contentFrame);
    m_statusLabel->setObjectName("storageStatusLabel");
    m_statusLabel->setWordWrap(true);

    m_clearCacheButton = new QPushButton("Clear Cache", contentFrame);
    m_clearCacheButton->setObjectName("storageClearButton");

    contentLayout->addLayout(cacheRow);
    contentLayout->addLayout(appRow);
    contentLayout->addWidget(m_statusLabel);
    contentLayout->addWidget(m_clearCacheButton, 0, Qt::AlignLeft);
    contentLayout->addStretch();

    layout->addWidget(topFrame);
    layout->addWidget(contentFrame, 1);

    setStyleSheet(
        "StoragePage { background: #f3f1eb; }"
        "#storageTopFrame, #storageContentFrame {"
        "  background: #fbfaf7;"
        "  border: 1px solid #ded8cc;"
        "  border-radius: 16px;"
        "}"
        "#storageBackButton, #storageClearButton {"
        "  padding: 10px 14px;"
        "  border: 1px solid #d7d2c7;"
        "  border-radius: 10px;"
        "  background: #f6f3ec;"
        "  color: #2f3a33;"
        "}"
        "#storageBackButton:hover, #storageClearButton:hover {"
        "  background: #eef4ef;"
        "}"
        "#storageTitleLabel {"
        "  font-size: 26px;"
        "  font-weight: 600;"
        "  color: #1f2328;"
        "}"
        "#storageLabel {"
        "  font-size: 16px;"
        "  font-weight: 600;"
        "  color: #2f3a33;"
        "}"
        "#storageValueLabel {"
        "  font-size: 16px;"
        "  color: #1f2328;"
        "}"
        "#storageStatusLabel {"
        "  min-height: 20px;"
        "  color: #7a4b36;"
        "}"
    );

    connect(backButton, &QPushButton::clicked, this, &StoragePage::backRequested);
    connect(m_clearCacheButton, &QPushButton::clicked,
            this, &StoragePage::clearCacheRequested);
}

void StoragePage::showSizes(const QString &cacheSize, const QString &appSize)
{
    m_cacheSizeValueLabel->setText(cacheSize);
    m_appSizeValueLabel->setText(appSize);
    m_clearCacheButton->setEnabled(true);
    m_statusLabel->clear();
}

void StoragePage::showClearSucceeded(const QString &cacheSize, const QString &appSize)
{
    showSizes(cacheSize, appSize);
    m_statusLabel->setText("Cache cleared.");
}

void StoragePage::showOperationFailed(const QString &message)
{
    m_clearCacheButton->setEnabled(true);
    m_statusLabel->setText(message);
}

void StoragePage::showClearing(bool clearing)
{
    m_clearCacheButton->setEnabled(!clearing);
    if (clearing) {
        m_statusLabel->setText("Clearing cache...");
    }
}
