#include "config/appconfig.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QHash>
#include <QRegularExpression>
#include <QStringList>

namespace
{
QString unquoteTomlValue(const QString &value)
{
    QString trimmed = value.trimmed();
    if (trimmed.startsWith('"') && trimmed.endsWith('"') && trimmed.size() >= 2) {
        trimmed = trimmed.mid(1, trimmed.size() - 2);
    }
    trimmed.replace("\\n", "\n");
    trimmed.replace("\\\"", "\"");
    return trimmed;
}

QStringList candidateConfigPaths()
{
    const QString appDir = QCoreApplication::applicationDirPath();
    const QString cwd = QDir::currentPath();
    return {
        QDir(cwd).filePath("config.toml"),
        QDir(appDir).filePath("config.toml"),
        QDir(appDir).filePath("../config.toml"),
        QDir(appDir).filePath("../oj-client/config.toml"),
        QDir(appDir).filePath("../../oj-client/config.toml")
    };
}

QString configOutputPath()
{
    for (const QString &path : candidateConfigPaths()) {
        if (QFile::exists(path)) {
            return QDir::cleanPath(path);
        }
    }
    return QDir(QDir::currentPath()).filePath("config.toml");
}
}

OpenAiConfig AppConfig::loadOpenAiConfig()
{
    OpenAiConfig config;
    QString loadedPath;
    for (const QString &path : candidateConfigPaths()) {
        if (QFile::exists(path)) {
            loadedPath = QDir::cleanPath(path);
            break;
        }
    }
    if (loadedPath.isEmpty()) {
        return config;
    }

    QFile file(loadedPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return config;
    }

    const QString content = QString::fromUtf8(file.readAll());
    QString currentSection;
    QString modelProvider;
    QHash<QString, QString> providerBaseUrls;
    QHash<QString, QString> providerEnvKeys;
    const QRegularExpression sectionRegex("^\\s*\\[([^\\]]+)\\]\\s*$");
    const QRegularExpression keyValueRegex("^\\s*([A-Za-z0-9_\\-]+)\\s*=\\s*(.*?)\\s*$");

    const QStringList lines = content.split('\n');
    for (QString line : lines) {
        const int commentIndex = line.indexOf('#');
        if (commentIndex >= 0) {
            line = line.left(commentIndex);
        }
        if (line.trimmed().isEmpty()) {
            continue;
        }

        const QRegularExpressionMatch sectionMatch = sectionRegex.match(line);
        if (sectionMatch.hasMatch()) {
            currentSection = sectionMatch.captured(1).trimmed().toLower();
            continue;
        }

        const QRegularExpressionMatch keyValueMatch = keyValueRegex.match(line);
        if (!keyValueMatch.hasMatch()) {
            continue;
        }

        const QString key = keyValueMatch.captured(1).trimmed().toLower();
        const QString value = unquoteTomlValue(keyValueMatch.captured(2));

        if (currentSection.isEmpty()) {
            if (key == "model") {
                if (!value.isEmpty()) {
                    config.model = value;
                }
            } else if (key == "system_prompt") {
                config.systemPrompt = value;
            } else if (key == "api_key") {
                config.apiKey = value;
            } else if (key == "base_url") {
                const QUrl baseUrl(value);
                if (baseUrl.isValid() && !baseUrl.scheme().isEmpty()) {
                    config.baseUrl = baseUrl;
                }
            } else if (key == "model_provider") {
                modelProvider = value.trimmed().toLower();
            }
            continue;
        }

        if (currentSection == "openai") {
            if (key == "base_url") {
                const QUrl baseUrl(value);
                if (baseUrl.isValid() && !baseUrl.scheme().isEmpty()) {
                    config.baseUrl = baseUrl;
                }
            } else if (key == "model") {
                if (!value.isEmpty()) {
                    config.model = value;
                }
            } else if (key == "system_prompt") {
                config.systemPrompt = value;
            } else if (key == "api_key") {
                config.apiKey = value;
            }
            continue;
        }

        if (currentSection.startsWith("model_providers.")) {
            const QString providerName = currentSection.mid(QString("model_providers.").size());
            if (providerName.isEmpty()) {
                continue;
            }
            if (key == "base_url") {
                providerBaseUrls.insert(providerName, value);
            } else if (key == "env_key") {
                providerEnvKeys.insert(providerName, value);
            }
        }
    }

    if (!modelProvider.isEmpty()) {
        const QString providerName = modelProvider.toLower();
        const QString providerBaseUrl = providerBaseUrls.value(providerName);
        if (!providerBaseUrl.isEmpty()) {
            const QUrl baseUrl(providerBaseUrl);
            if (baseUrl.isValid() && !baseUrl.scheme().isEmpty()) {
                config.baseUrl = baseUrl;
            }
        }
        if (config.apiKey.trimmed().isEmpty()) {
            config.apiKey = providerEnvKeys.value(providerName);
        }
    }

    config.sourcePath = loadedPath;
    return config;
}

QString AppConfig::loadConfigText(QString *sourcePath)
{
    QString loadedPath;
    for (const QString &path : candidateConfigPaths()) {
        if (QFile::exists(path)) {
            loadedPath = QDir::cleanPath(path);
            break;
        }
    }
    if (sourcePath != nullptr) {
        *sourcePath = loadedPath.isEmpty() ? configOutputPath() : loadedPath;
    }
    if (loadedPath.isEmpty()) {
        return QString(
            "[openai]\n"
            "base_url = \"https://api.openai.com/v1\"\n"
            "model = \"gpt-5-mini\"\n"
            "system_prompt = \"You are an AI programming assistant for algorithm problems.\"\n"
            "api_key = \"\"\n");
    }

    QFile file(loadedPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QString();
    }
    return QString::fromUtf8(file.readAll());
}

bool AppConfig::saveOpenAiConfig(const OpenAiConfig &config, QString *errorMessage)
{
    const QString outputPath = configOutputPath();
    QDir().mkpath(QFileInfo(outputPath).absolutePath());

    QFile file(outputPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        if (errorMessage != nullptr) {
            *errorMessage = QString("Failed to write config.toml: %1").arg(file.errorString());
        }
        return false;
    }

    auto quote = [](QString text) {
        text.replace("\\", "\\\\");
        text.replace("\"", "\\\"");
        text.replace("\n", "\\n");
        return QString("\"%1\"").arg(text);
    };

    const QString content = QString(
                                "[openai]\n"
                                "base_url = %1\n"
                                "model = %2\n"
                                "system_prompt = %3\n"
                                "api_key = %4\n")
                                .arg(quote(config.baseUrl.toString()),
                                     quote(config.model),
                                     quote(config.systemPrompt),
                                     quote(config.apiKey));
    file.write(content.toUtf8());
    file.close();
    return true;
}

bool AppConfig::saveConfigText(const QString &content,
                               QString *savedPath,
                               QString *errorMessage)
{
    const QString outputPath = configOutputPath();
    QDir().mkpath(QFileInfo(outputPath).absolutePath());

    QFile file(outputPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        if (errorMessage != nullptr) {
            *errorMessage = QString("Failed to write config.toml: %1").arg(file.errorString());
        }
        return false;
    }
    file.write(content.toUtf8());
    file.close();
    if (savedPath != nullptr) {
        *savedPath = outputPath;
    }
    return true;
}
