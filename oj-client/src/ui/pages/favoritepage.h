#pragma once

#include "parser/problemparser.h"

#include <QWidget>

class QLabel;
class QListWidget;
class QListWidgetItem;

class FavoritePage : public QWidget
{
    Q_OBJECT

public:
    explicit FavoritePage(QWidget *parent = nullptr);

    void showFoldersUnavailable();
    void showRefreshUnavailable();
    void showFavoriteOperationFailed(const QString &message);
    void showFavorites(const QList<ProblemPageInfo> &favorites);

signals:
    void backRequested();
    void refreshRequested();
    void favoriteSelected(const QString &title, const QString &url);

private:
    QListWidget *m_favoriteListWidget = nullptr;
};
