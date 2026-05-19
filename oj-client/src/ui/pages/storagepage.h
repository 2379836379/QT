#pragma once

#include <QWidget>

class QLabel;
class QPushButton;

class StoragePage : public QWidget
{
    Q_OBJECT

public:
    explicit StoragePage(QWidget *parent = nullptr);

    void showSizes(const QString &cacheSize, const QString &appSize);
    void showClearSucceeded(const QString &cacheSize, const QString &appSize);
    void showOperationFailed(const QString &message);
    void showClearing(bool clearing);

signals:
    void backRequested();
    void clearCacheRequested();

private:
    QLabel *m_cacheSizeValueLabel = nullptr;
    QLabel *m_appSizeValueLabel = nullptr;
    QLabel *m_statusLabel = nullptr;
    QPushButton *m_clearCacheButton = nullptr;
};
