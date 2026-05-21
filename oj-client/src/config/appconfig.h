#pragma once

#include <QString>
#include <QUrl>

struct OpenAiConfig
{
    QUrl baseUrl = QUrl("https://api.openai.com/v1");
    QString model = "gpt-5-mini";
    QString systemPrompt;
    QString apiKey;
    QString sourcePath;
};

class AppConfig
{
public:
    static OpenAiConfig loadOpenAiConfig();
    static QString loadConfigText(QString *sourcePath = nullptr);
    static bool saveOpenAiConfig(const OpenAiConfig &config, QString *errorMessage = nullptr);
    static bool saveConfigText(const QString &content,
                               QString *savedPath = nullptr,
                               QString *errorMessage = nullptr);
};
