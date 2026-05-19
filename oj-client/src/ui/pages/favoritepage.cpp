#include "ui/pages/favoritepage.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QVBoxLayout>

FavoritePage::FavoritePage(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    auto *toolbarLayout = new QHBoxLayout();
    auto *backButton = new QPushButton("Back", this);
    auto *refreshButton = new QPushButton("Refresh", this);
    auto *titleLabel = new QLabel("Favorite Problems", this);

    m_favoriteListWidget = new QListWidget(this);

    toolbarLayout->addWidget(backButton);
    toolbarLayout->addWidget(titleLabel, 1);
    toolbarLayout->addWidget(refreshButton);

    layout->addLayout(toolbarLayout);
    layout->addWidget(m_favoriteListWidget, 1);

    connect(backButton, &QPushButton::clicked, this, &FavoritePage::backRequested);
    connect(refreshButton, &QPushButton::clicked, this, &FavoritePage::refreshRequested);
    connect(
        m_favoriteListWidget,
        &QListWidget::itemClicked,
        this,
        [this](QListWidgetItem *item) {
            emit favoriteSelected(item->text(), item->data(Qt::UserRole).toString());
        });
}

void FavoritePage::showFoldersUnavailable()
{
    m_favoriteListWidget->clear();
}

void FavoritePage::showRefreshUnavailable()
{
}

void FavoritePage::showFavoriteOperationFailed(const QString &message)
{
    m_favoriteListWidget->clear();
    auto *item = new QListWidgetItem(message, m_favoriteListWidget);
    item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
}

void FavoritePage::showFavorites(const QList<ProblemPageInfo> &favorites)
{
    m_favoriteListWidget->clear();
    for (const ProblemPageInfo &favorite : favorites) {
        const QString displayText = favorite.title.isEmpty()
            ? favorite.problemUrl
            : favorite.title;
        auto *item = new QListWidgetItem(displayText, m_favoriteListWidget);
        item->setData(Qt::UserRole, favorite.problemUrl);
    }
}
