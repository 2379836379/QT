#pragma once

#include <QWidget>

class QLabel;
class QPushButton;
class QPlainTextEdit;

class AiConfigPage : public QWidget
{
    Q_OBJECT

public:
    explicit AiConfigPage(QWidget *parent = nullptr);

    void setConfigText(const QString &path, const QString &content);
    QString currentConfigText() const;
    void showSaveSucceeded(const QString &path);
    void showSaveFailed(const QString &message);

signals:
    void backRequested();
    void saveRequested(const QString &content);

private:
    QLabel *m_pathLabel = nullptr;
    QPlainTextEdit *m_configEdit = nullptr;
    QLabel *m_statusLabel = nullptr;
};
