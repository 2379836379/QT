#pragma once

#include <QWidget>

class QLabel;
class QPushButton;
class QFrame;
class QWidget;
class QAbstractButton;

class StoragePage : public QWidget
{
    Q_OBJECT

public:
    explicit StoragePage(QWidget *parent = nullptr);
    void setDarkMode(bool dark);

    void showSizes(const QString &cacheSize, const QString &appSize);
    void showClearSucceeded(const QString &cacheSize, const QString &appSize);
    void showOperationFailed(const QString &message);
    void showClearing(bool clearing);

signals:
    void backRequested();
    void homeRequested();
    void themeToggleRequested(bool dark);
    void clearCacheRequested();

private:
    void setToolsExpanded(bool expanded);

    QFrame *m_toolsFrame = nullptr;
    QPushButton *m_toolsToggleButton = nullptr;
    QWidget *m_toolsPanel = nullptr;
    QWidget *m_collapsedToolsPanel = nullptr;
    QPushButton *m_backToolButton = nullptr;
    QPushButton *m_collapsedBackButton = nullptr;
    QLabel *m_cacheSizeValueLabel = nullptr;
    QLabel *m_appSizeValueLabel = nullptr;
    QAbstractButton *m_alarmToggleButton = nullptr;
    QAbstractButton *m_autoToggleButton = nullptr;
    QLabel *m_statusLabel = nullptr;
    QPushButton *m_clearCacheButton = nullptr;
    bool m_toolsExpanded = true;
    bool m_darkMode = false;
    bool m_alarmEnabled = false;
    bool m_autoEnabled = false;
};
