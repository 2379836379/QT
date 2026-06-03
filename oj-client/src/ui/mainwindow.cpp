#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "config/appconfig.h"
#include "network/cookiestore.h"
#include "network/openaiclient.h"
#include "network/openjudgeclient.h"
#include "parser/loginparser.h"
#include "repository/cache/classcacherepository.h"
#include "repository/cache/contestcacherepository.h"
#include "repository/cache/homecacherepository.h"
#include "repository/cache/problemcacherepository.h"
#include "repository/favorite/favoriteproblemrepository.h"
#include "repository/login/logincacherepository.h"
#include "repository/submit/classrepository.h"
#include "repository/submit/contestrepository.h"
#include "repository/submit/homerepository.h"
#include "repository/submit/problemrepository.h"
#include "repository/submit/resultrepository.h"
#include "repository/submit/submitrepository.h"
#include "service/browse/classservice.h"
#include "service/browse/contestservice.h"
#include "service/browse/homeservice.h"
#include "service/browse/problemservice.h"
#include "service/cache/cacheservice.h"
#include "service/favorite/favoriteproblemservice.h"
#include "service/app/applicationsizeservice.h"
#include "service/ai/aiservice.h"
#include "service/login/loginservice.h"
#include "service/login/emailverifyservice.h"
#include "service/login/logincacheservice.h"
#include "service/reminder/reminderservice.h"
#include "service/submit/resultservice.h"
#include "service/submit/submitservice.h"
#include "ui/pages/classpage.h"
#include "ui/pages/contestpage.h"
#include "ui/pages/aiconfigpage.h"
#include "ui/pages/favoritepage.h"
#include "ui/pages/homepage.h"
#include "ui/pages/loginpage.h"
#include "ui/pages/problempage.h"
#include "ui/pages/storagepage.h"

#include <QAction>
#include <QApplication>
#include <QDir>
#include <QInputDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMenu>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QStyle>
#include <QSystemTrayIcon>
#include <QWidget>

namespace
{
QString normalizeJudgeLanguage(const QString &languageLabel)
{
    const QString text = languageLabel.trimmed().toLower();
    if (text.contains("python")) {
        return "python";
    }
    if (text.contains("rust") || text == "rs") {
        return "rust";
    }
    if (text == "gcc" || text.startsWith("gcc(") || text.contains("gcc ")
        || text.contains("g++")
        || text.contains("c++") || text.contains("clang++")) {
        return "cpp";
    }
    if (text == "cc" || text == "cxx") {
        return "cpp";
    }
    return text;
}

QString judgeFileNameForLanguage(const QString &language)
{
    if (language == "python" || language == "python3" || language == "py") {
        return "main.py";
    }
    if (language == "rust" || language == "rs") {
        return "main.rs";
    }
    return "main.cpp";
}

QString truncatedText(const QString &text, int limit = 2000)
{
    if (text.size() <= limit) {
        return text;
    }
    return text.left(limit) + "\n...[truncated]";
}

QString jsonValueToText(const QJsonValue &value)
{
    if (value.isString()) {
        return value.toString();
    }
    if (value.isBool()) {
        return value.toBool() ? "true" : "false";
    }
    if (value.isDouble()) {
        return QString::number(value.toDouble());
    }
    if (value.isNull() || value.isUndefined()) {
        return "<none>";
    }
    if (value.isArray()) {
        return QString::fromUtf8(
            QJsonDocument(value.toArray()).toJson(QJsonDocument::Indented));
    }
    return QString::fromUtf8(
        QJsonDocument(value.toObject()).toJson(QJsonDocument::Indented));
}

QString formatJudgeResult(const NetworkResult &result)
{
    const QJsonDocument document = QJsonDocument::fromJson(result.body);
    if (!document.isObject()) {
        return QString(
                   "Judge Result\n"
                   "HTTP status: %1\n"
                   "Network OK: %2\n\n"
                   "%3")
            .arg(QString::number(result.statusCode),
                 result.ok ? "true" : "false",
                 QString::fromUtf8(result.body));
    }

    const QJsonObject object = document.object();
    QStringList sections;
    sections << QString("Judge Result\nHTTP status: %1\nNetwork OK: %2")
                    .arg(QString::number(result.statusCode), result.ok ? "true" : "false");

    const QStringList summaryKeys = {"status",
                                     "message",
                                     "exit_code",
                                     "time_ms",
                                     "memory_mb",
                                     "compile_exit_code"};
    QStringList summaryLines;
    for (const QString &key : summaryKeys) {
        if (object.contains(key)) {
            summaryLines << QString("%1: %2").arg(key, jsonValueToText(object.value(key)));
        }
    }
    if (!summaryLines.isEmpty()) {
        sections << summaryLines.join('\n');
    }

    const QStringList outputKeys = {"stdout", "stderr", "compile_stdout", "compile_stderr"};
    for (const QString &key : outputKeys) {
        if (!object.contains(key)) {
            continue;
        }
        const QString value = jsonValueToText(object.value(key));
        if (value.isEmpty() || value == "<none>") {
            continue;
        }
        sections << QString("%1:\n%2").arg(key, value);
    }

    sections << QString("Raw JSON:\n%1")
                    .arg(QString::fromUtf8(document.toJson(QJsonDocument::Indented)));
    return sections.join("\n\n");
}

QString formatJudgeRequest(const QString &language,
                           const QString &fileName,
                           const QString &sourceText,
                           const QString &stdinText)
{
    return QString(
               "Judge Request\n"
               "POST http://10.129.240.62:18080/judge\n"
               "language: %1\n"
               "file: %2\n"
               "time_limit_ms: 2000\n"
               "memory_limit_mb: 256\n"
               "source_length: %3\n"
               "stdin_length: %4\n\n"
               "stdin:\n%5\n\n"
               "source:\n%6")
        .arg(language,
             fileName,
             QString::number(sourceText.size()),
             QString::number(stdinText.size()),
             truncatedText(stdinText),
             truncatedText(sourceText));
}

QString formatSubmitResponse(const NetworkResult &result)
{
    const QJsonDocument document = QJsonDocument::fromJson(result.body);
    QString redirectUrl;
    if (document.isObject()) {
        redirectUrl = document.object().value("redirect").toString();
    }

    QString text = QString(
                       "Submit Response\n"
                       "HTTP status: %1\n"
                       "Network OK: %2\n"
                       "Request URL: %3\n"
                       "Final URL: %4")
                       .arg(QString::number(result.statusCode),
                            result.ok ? "true" : "false",
                            result.requestUrl.toString(),
                            result.finalUrl.toString());
    if (!redirectUrl.isEmpty()) {
        text += "\nRedirect: " + redirectUrl;
    }
    if (!result.body.trimmed().isEmpty()) {
        text += "\n\nRaw JSON:\n"
                + QString::fromUtf8(document.isObject()
                                        ? document.toJson(QJsonDocument::Indented)
                                        : result.body);
    }
    return text;
}

void writeStartupLog(const QString &message)
{
    QDir dir(QCoreApplication::applicationDirPath());
    if (dir.dirName().compare("build", Qt::CaseInsensitive) == 0) {
        dir.cdUp();
    }
    dir.mkpath("data");

    QFile file(dir.filePath("data/startup.log"));
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        return;
    }

    QTextStream stream(&file);
    stream << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz")
           << " | " << message << '\n';
}
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    writeStartupLog("MainWindow: constructor begin");
    ui->setupUi(this);
    writeStartupLog("MainWindow: ui setup complete");

    m_client = new OpenJudgeClient(this);
    m_openAiClient = new OpenAiClient(this);
    m_cookieStore = new CookieStore(m_client);
    m_client->setCookieStore(m_cookieStore);
    writeStartupLog("MainWindow: core clients created");

    m_homeRepository = new HomeRepository(m_client, this);
    m_homeCacheRepository = new HomeCacheRepository();
    m_classRepository = new ClassRepository(m_client, this);
    m_classCacheRepository = new ClassCacheRepository();
    m_contestRepository = new ContestRepository(m_client, this);
    m_contestCacheRepository = new ContestCacheRepository();
    m_problemRepository = new ProblemRepository(m_client, this);
    m_problemCacheRepository = new ProblemCacheRepository();
    m_loginCacheRepository = new LoginCacheRepository();
    m_resultRepository = new ResultRepository(m_client, this);
    m_submitRepository = new SubmitRepository(m_client, this);
    m_reminderClassRepository = new ClassRepository(m_client, this);
    writeStartupLog("MainWindow: repositories created");

    m_cacheService = new CacheService(m_homeCacheRepository,
                                      m_classCacheRepository,
                                      m_contestCacheRepository,
                                      m_problemCacheRepository,
                                      this);
    m_loginService = new LoginService(m_client, this);
    m_loginCacheService = new LoginCacheService(m_loginCacheRepository, this);
    m_emailVerifyService = new EmailVerifyService(this);
    m_applicationSizeService = new ApplicationSizeService(this);
    writeStartupLog("MainWindow: app services created");
    const OpenAiConfig openAiConfig = AppConfig::loadOpenAiConfig();
    writeStartupLog("MainWindow: openai config loaded");
    m_openAiClient->setBaseUrl(openAiConfig.baseUrl);
    m_aiService = new AiService(m_openAiClient, this);
    m_aiService->setConfig(openAiConfig);
    m_aiService->setToolContext(AiToolContext{
        [this]() { return m_problemPage->currentProblemDetailText(); },
        [this]() {
            const QString label = m_problemPage->currentLanguageLabel();
            const QString value = m_problemPage->currentLanguageValue();
            if (label.isEmpty() && value.isEmpty()) {
                return QString();
            }
            return QString("label: %1\nvalue: %2")
                .arg(label.isEmpty() ? "<none>" : label,
                     value.isEmpty() ? "<none>" : value);
        },
        [this]() { return m_problemPage->currentSourceCode(); },
        [this]() { return m_problemPage->currentTestInput(); },
        [this]() { return m_problemPage->currentTestOutput(); },
        [this](const QString &text) { m_problemPage->setSourceCodeText(text); },
        [this](const QString &text) { m_problemPage->setTestInputText(text); },
        [this](const QString &callId, const QString &stdinText) {
            m_pendingAiRunTestCallId = callId;
            const QString language =
                normalizeJudgeLanguage(m_problemPage->currentLanguageLabel());
            const QString sourceText = m_problemPage->currentSourceCode();
            if (language.isEmpty()) {
                m_aiService->failToolCall(callId, "No language selected.");
                return;
            }
            if (sourceText.trimmed().isEmpty()) {
                m_aiService->failToolCall(callId, "Source code is empty.");
                return;
            }

            m_problemPage->setTestInputText(stdinText);
            m_lastTestRequestLog = formatJudgeRequest(
                language,
                judgeFileNameForLanguage(language),
                sourceText,
                stdinText);
            m_problemPage->showTesting(true);
            m_problemPage->showTestResult(m_lastTestRequestLog + "\n\nRunning test...");
            m_client->judgeSource(language,
                                  judgeFileNameForLanguage(language),
                                  sourceText.toUtf8(),
                                  stdinText.toUtf8());
        },
        [this](const QString &callId) {
            m_pendingAiSubmitCallId = callId;
            const QString languageValue = m_problemPage->currentLanguageValue();
            const QString sourceText = m_problemPage->currentSourceCode();
            if (languageValue.trimmed().isEmpty()) {
                m_aiService->failToolCall(callId, "No language selected.");
                return;
            }
            if (sourceText.trimmed().isEmpty()) {
                m_aiService->failToolCall(callId, "Source code is empty.");
                return;
            }
            m_submitService->submitSolution(languageValue, sourceText);
        }});
    writeStartupLog("MainWindow: ai service configured");
    m_aiConfigSummary = QString("Config: %1 | model: %2")
                            .arg(openAiConfig.sourcePath.isEmpty()
                                     ? QString("defaults")
                                     : openAiConfig.sourcePath,
                                 openAiConfig.model);
    m_homeService = new HomeService(m_homeRepository, m_homeCacheRepository, this);
    m_classService = new ClassService(
        m_classRepository, m_classCacheRepository, this);
    m_contestService = new ContestService(
        m_contestRepository, m_contestCacheRepository, this);
    m_problemService = new ProblemService(
        m_problemRepository, m_problemCacheRepository, this);
    m_favoriteProblemRepository = new FavoriteProblemRepository();
    m_favoriteProblemService = new FavoriteProblemService(
        m_favoriteProblemRepository, this);
    m_resultService = new ResultService(m_resultRepository, this);
    m_submitService = new SubmitService(m_submitRepository, this);
    m_reminderService = new ReminderService(
        m_reminderClassRepository,
        m_classCacheRepository,
        this);
    writeStartupLog("MainWindow: domain services created");

    setupUiState();
    writeStartupLog("MainWindow: setupUiState complete");
    setupTrayIcon();
    writeStartupLog("MainWindow: setupTrayIcon complete");
    connectSignals();
    writeStartupLog("MainWindow: connectSignals complete");
    ensureStartupVisible();
    writeStartupLog("MainWindow: ensureStartupVisible complete");
    QTimer::singleShot(0, this, [this]() {
        writeStartupLog("MainWindow: deferred initialization begin");
        if (m_loginCacheService->initialize()) {
            CachedLoginInfo loginInfo;
            if (m_loginCacheService->loadLastLogin(&loginInfo)) {
                m_loginPage->setCredentials(loginInfo.email, loginInfo.password);
            }
        }
        applyLoginCacheState(m_loginPage->email());

        if (!m_favoriteProblemService->initialize()) {
        }
        writeStartupLog("MainWindow: deferred initialization complete");
    });
    writeStartupLog("MainWindow: constructor end");
}

MainWindow::~MainWindow()
{
    delete m_homeCacheRepository;
    delete m_classCacheRepository;
    delete m_contestCacheRepository;
    delete m_problemCacheRepository;
    delete m_loginCacheRepository;
    delete m_favoriteProblemRepository;
    delete ui;
}

void MainWindow::setupUiState()
{
    writeStartupLog("MainWindow: setupUiState begin");
    m_loginPage = new LoginPage(this);
    writeStartupLog("MainWindow: login page created");
    m_homePage = new HomePage(this);
    writeStartupLog("MainWindow: home page created");
    m_aiConfigPage = new AiConfigPage(this);
    writeStartupLog("MainWindow: ai config page created");
    m_classPage = new ClassPage(this);
    writeStartupLog("MainWindow: class page created");
    m_contestPage = new ContestPage(this);
    writeStartupLog("MainWindow: contest page created");
    m_problemPage = new ProblemPage(this);
    writeStartupLog("MainWindow: problem page created");
    m_favoritePage = new FavoritePage(this);
    writeStartupLog("MainWindow: favorite page created");
    m_storagePage = new StoragePage(this);
    writeStartupLog("MainWindow: storage page created");

    ui->pageStack->addWidget(m_loginPage);
    ui->pageStack->addWidget(m_homePage);
    ui->pageStack->addWidget(m_aiConfigPage);
    ui->pageStack->addWidget(m_classPage);
    ui->pageStack->addWidget(m_contestPage);
    ui->pageStack->addWidget(m_problemPage);
    ui->pageStack->addWidget(m_favoritePage);
    ui->pageStack->addWidget(m_storagePage);
    writeStartupLog("MainWindow: pages added to stack");

    m_problemPage->setAiConfigSummary(m_aiConfigSummary);
    writeStartupLog("MainWindow: problem page ai summary set");
    QString configPath;
    m_aiConfigPage->setConfigText(configPath = m_aiService->config().sourcePath,
                                  AppConfig::loadConfigText(&configPath));
    writeStartupLog("MainWindow: ai config text loaded");

    showRootPage(m_loginPage);
    writeStartupLog("MainWindow: setupUiState end");
}

void MainWindow::setupTrayIcon()
{
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        return;
    }

    m_trayIcon = new QSystemTrayIcon(this);
    QIcon trayIcon = windowIcon();
    if (trayIcon.isNull()) {
        trayIcon = style()->standardIcon(QStyle::SP_MessageBoxInformation);
    }
    m_trayIcon->setIcon(trayIcon);
    m_trayIcon->setToolTip("oj-client");

    m_trayMenu = new QMenu(this);
    m_restoreTrayAction = m_trayMenu->addAction("Restore");
    m_exitTrayAction = m_trayMenu->addAction("Exit");
    m_trayIcon->setContextMenu(m_trayMenu);

    connect(m_restoreTrayAction, &QAction::triggered, this, [this]() {
        restoreFromTray();
    });
    connect(m_exitTrayAction, &QAction::triggered, this, [this]() {
        m_allowClose = true;
        if (m_trayIcon != nullptr) {
            m_trayIcon->hide();
        }
        qApp->quit();
    });
    connect(
        m_trayIcon,
        &QSystemTrayIcon::activated,
        this,
        [this](QSystemTrayIcon::ActivationReason reason) {
            if (reason == QSystemTrayIcon::Trigger
                || reason == QSystemTrayIcon::DoubleClick) {
                restoreFromTray();
            }
        });

    m_trayIcon->show();
}

void MainWindow::ensureStartupVisible()
{
    setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    showNormal();
    raise();
    activateWindow();

    QTimer::singleShot(0, this, [this]() {
        setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
        showNormal();
        raise();
        activateWindow();
    });
}

void MainWindow::connectSignals()
{
    connect(
        m_loginPage,
        &LoginPage::loginRequested,
        this,
        [this](const QString &email, const QString &password) {
            if (email.isEmpty() || password.isEmpty()) {
                m_loginPage->showLoginFailed("Email and password are required.");
                return;
            }

            if (requiresEmailVerification(email)) {
                const QString code = m_loginPage->verificationCode();
                if (code.isEmpty()) {
                    m_loginPage->showLoginFailed(
                        "Verification code is required for a new email.");
                    return;
                }

                m_pendingLoginEmail = email;
                m_pendingLoginPassword = password;
                m_emailVerifyService->verifyCode(email, code);
                return;
            }

            m_loginService->login(email, password);
        });
    connect(
        m_loginPage,
        &LoginPage::emailEdited,
        this,
        [this](const QString &email) {
            applyLoginCacheState(email);
        });
    connect(
        m_loginPage,
        &LoginPage::verificationRequested,
        this,
        [this](const QString &email) {
            if (email.isEmpty()) {
                m_loginPage->showLoginFailed("Email is required.");
                return;
            }

            if (!requiresEmailVerification(email)) {
                m_loginPage->showVerificationMessage(
                    "This email is already cached. Verification is not required.");
                return;
            }

            m_emailVerifyService->sendCode(email);
        });
    connect(
        m_loginService,
        &LoginService::loggingChanged,
        this,
        [this](bool logging) {
            if (logging) {
                m_loginPage->showLoggingIn();
            } else if (ui->pageStack->currentWidget() == m_loginPage) {
                m_loginPage->showIdle();
            }
        });
    connect(
        m_loginService,
        &LoginService::failed,
        this,
        [this](const QString &message) {
            showRootPage(m_loginPage);
            m_loginPage->showLoginFailed(message);
        });
    connect(
        m_loginService,
        &LoginService::loginSucceeded,
        this,
        [this](const QUrl &personalHomeUrl) {
            m_loginCacheService->saveLogin(
                m_loginPage->email(),
                m_loginPage->password());
            m_verifiedEmail = m_loginPage->email();
            showRootPage(m_homePage);
            m_homePage->showOpeningHome();
            m_homeService->openHome(personalHomeUrl);
        });
    connect(
        m_loginCacheService,
        &LoginCacheService::failed,
        this,
        [this](const QString &message) {
            if (ui->pageStack->currentWidget() == m_loginPage) {
                m_loginPage->showLoginFailed(message);
            }
        });
    connect(
        m_emailVerifyService,
        &EmailVerifyService::sendingChanged,
        this,
        [this](bool sending) {
            m_loginPage->showSendingVerification(sending);
            if (!sending) {
                m_loginPage->showIdle();
            }
        });
    connect(
        m_emailVerifyService,
        &EmailVerifyService::verifyingChanged,
        this,
        [this](bool verifying) {
            m_loginPage->showVerifyingCode(verifying);
            if (!verifying) {
                m_loginPage->showIdle();
            }
        });
    connect(
        m_emailVerifyService,
        &EmailVerifyService::codeSent,
        this,
        [this](const QString &message) {
            m_loginPage->showVerificationMessage(message);
        });
    connect(
        m_emailVerifyService,
        &EmailVerifyService::verified,
        this,
        [this](const QString &email) {
            m_verifiedEmail = email.trimmed();
            m_loginPage->showVerificationMessage(
                "Verification succeeded. Logging in...");
            m_loginService->login(m_pendingLoginEmail, m_pendingLoginPassword);
        });
    connect(
        m_emailVerifyService,
        &EmailVerifyService::failed,
        this,
        [this](const QString &message) {
            m_loginPage->showLoginFailed(message);
            m_pendingLoginEmail.clear();
            m_pendingLoginPassword.clear();
        });

    connect(
        m_homePage,
        &HomePage::themeToggleRequested,
        this,
        [this](bool dark) {
            applyDarkMode(dark);
        });
    connect(m_classPage, &ClassPage::themeToggleRequested, this, [this](bool dark) {
        applyDarkMode(dark);
    });
    connect(m_contestPage, &ContestPage::themeToggleRequested, this, [this](bool dark) {
        applyDarkMode(dark);
    });
    connect(m_problemPage, &ProblemPage::themeToggleRequested, this, [this](bool dark) {
        applyDarkMode(dark);
    });
    connect(m_favoritePage, &FavoritePage::themeToggleRequested, this, [this](bool dark) {
        applyDarkMode(dark);
    });
    connect(m_storagePage, &StoragePage::themeToggleRequested, this, [this](bool dark) {
        applyDarkMode(dark);
    });
    connect(m_aiConfigPage, &AiConfigPage::themeToggleRequested, this, [this](bool dark) {
        applyDarkMode(dark);
    });
    connect(
        m_homePage,
        &HomePage::classSelected,
        this,
        [this](const QString &name, const QString &url) {
            m_classPage->openClass(name, url);
            pushPage(m_classPage);
            m_classService->openClass(QUrl(url));
        });
    connect(
        m_homePage,
        &HomePage::favoritesRequested,
        this,
        [this]() {
            m_favoritePage->showFoldersUnavailable();
            m_favoritePage->showFolders(
                m_favoriteProblemService->loadFolders());
            pushPage(m_favoritePage);
        });
    connect(
        m_homePage,
        &HomePage::refreshRequested,
        this,
        [this]() {
            const QUrl homeUrl = m_homeService->currentHomeUrl();
            if (!homeUrl.isValid() || homeUrl.scheme().isEmpty()) {
                return;
            }
            m_homePage->showOpeningHome();
            m_homeService->openHome(homeUrl);
        });
    connect(
        m_homePage,
        &HomePage::storageRequested,
        this,
        [this]() {
            m_storagePage->showSizes(
                m_cacheService->formattedTotalCacheSize(),
                m_applicationSizeService->formattedTotalApplicationSize());
            pushPage(m_storagePage);
        });
    connect(
        m_homePage,
        &HomePage::aiConfigRequested,
        this,
        [this]() {
            QString configPath;
            const QString configText = AppConfig::loadConfigText(&configPath);
            m_aiConfigPage->setConfigText(configPath, configText);
            pushPage(m_aiConfigPage);
        });
    connect(
        m_homePage,
        &HomePage::logoutRequested,
        this,
        [this]() {
            m_client->logout();
            showRootPage(m_loginPage);
            m_loginPage->showIdle();
        });
    connect(
        m_homePage,
        &HomePage::reminderSelected,
        this,
        [this](const QString &title, const QString &url) {
            const QUrl contestUrl(url);
            if (!contestUrl.isValid() || contestUrl.scheme().isEmpty()) {
                return;
            }

            m_contestPage->openContest(title, contestUrl.toString());
            pushPage(m_contestPage);
            m_contestService->openContest(contestUrl);
        });

    connect(m_classPage, &ClassPage::backRequested, this, &MainWindow::popPage);
    connect(m_classPage, &ClassPage::homeRequested, this, [this]() {
        showRootPage(m_homePage);
    });
    connect(
        m_classPage,
        &ClassPage::contestSelected,
        this,
        [this](const QString &title, const QString &url) {
            m_contestPage->openContest(title, url);
            pushPage(m_contestPage);
            m_contestService->openContest(QUrl(url));
        });

    connect(m_contestPage, &ContestPage::backRequested, this, &MainWindow::popPage);
    connect(m_contestPage, &ContestPage::homeRequested, this, [this]() {
        showRootPage(m_homePage);
    });
    connect(
        m_contestPage,
        &ContestPage::problemSelected,
        this,
        [this](const QString &title, const QString &url) {
            openProblemPage(url, title, true);
        });

    connect(m_problemPage, &ProblemPage::backRequested, this, &MainWindow::popPage);
    connect(m_problemPage, &ProblemPage::homeRequested, this, [this]() {
        showRootPage(m_homePage);
    });
    connect(
        m_problemPage,
        &ProblemPage::refreshRequested,
        this,
        [this]() {
            const QUrl problemUrl = m_problemService->currentProblemUrl();
            if (!problemUrl.isValid() || problemUrl.scheme().isEmpty()) {
                return;
            }
            m_problemPage->openProblem(
                m_currentProblemInfo.title.isEmpty()
                    ? m_problemPage->windowTitle()
                    : m_currentProblemInfo.title);
            m_problemService->openProblem(problemUrl);
        });
    connect(
        m_problemPage,
        &ProblemPage::favoriteRequested,
        this,
        &MainWindow::saveCurrentProblemToFavorites);
    connect(
        m_problemPage,
        &ProblemPage::aiAskRequested,
        this,
        [this](const QString &question) {
            m_aiService->ask(question,
                             m_problemPage->currentProblemDetailText(),
                             m_problemPage->currentSourceCode(),
                             m_problemPage->currentTestInput(),
                             m_problemPage->currentTestOutput());
        });
    connect(
        m_problemPage,
        &ProblemPage::testRequested,
        this,
        [this](const QString &languageLabel,
               const QString &sourceText,
               const QString &stdinText) {
            const QString language = normalizeJudgeLanguage(languageLabel);
            if (language.isEmpty()) {
                m_problemPage->showTestFailed("No language selected.");
                return;
            }
            if (sourceText.trimmed().isEmpty()) {
                m_problemPage->showTestFailed("Source code is empty.");
                return;
            }

            m_lastTestRequestLog = formatJudgeRequest(
                language,
                judgeFileNameForLanguage(language),
                sourceText,
                stdinText);
            m_problemPage->showTesting(true);
            m_problemPage->showTestResult(m_lastTestRequestLog + "\n\nRunning test...");
            m_client->judgeSource(language,
                                  judgeFileNameForLanguage(language),
                                  sourceText.toUtf8(),
                                  stdinText.toUtf8());
        });
    connect(
        m_problemPage,
        &ProblemPage::submitRequested,
        this,
        [this](const QString &language, const QString &sourceText) {
            if (language.isEmpty()) {
                m_problemPage->showMissingLanguage();
                return;
            }
            m_submitService->submitSolution(language, sourceText);
        });

    connect(m_favoritePage, &FavoritePage::backRequested, this, &MainWindow::popPage);
    connect(m_favoritePage, &FavoritePage::homeRequested, this, [this]() {
        showRootPage(m_homePage);
    });
    connect(
        m_favoritePage,
        &FavoritePage::refreshRequested,
        this,
        [this]() {
            m_favoritePage->showFolders(
                m_favoriteProblemService->loadFolders());
        });
    connect(
        m_favoritePage,
        &FavoritePage::createFolderRequested,
        this,
        [this](const QString &folderName) {
            if (!m_favoriteProblemService->createFolder(folderName)) {
                m_favoritePage->showFavoriteOperationFailed(
                    m_favoriteProblemService->lastError());
                return;
            }

            m_favoritePage->showFolders(
                m_favoriteProblemService->loadFolders());
        });
    connect(
        m_favoritePage,
        &FavoritePage::folderRemoveRequested,
        this,
        [this](qint64 folderId, const QString &) {
            if (!m_favoriteProblemService->removeFolder(folderId)) {
                m_favoritePage->showFavoriteOperationFailed(
                    m_favoriteProblemService->lastError());
                return;
            }

            m_favoritePage->showFolders(
                m_favoriteProblemService->loadFolders());
        });
    connect(
        m_favoritePage,
        &FavoritePage::folderSelected,
        this,
        [this](qint64 folderId, const QString &folderName) {
            m_favoritePage->showFavorites(
                folderId,
                folderName,
                m_favoriteProblemService->loadFavoritesInFolder(folderId));
        });
    connect(
        m_favoritePage,
        &FavoritePage::favoriteSelected,
        this,
        [this](const QString &title, const QString &url) {
            openProblemPage(url, title, true);
        });
    connect(
        m_favoritePage,
        &FavoritePage::favoriteRemoveRequested,
        this,
        [this](qint64 folderId,
               const QString &folderName,
               const QString &problemUrl) {
            if (!m_favoriteProblemService->removeFavoriteFromFolder(problemUrl, folderId)) {
                m_favoritePage->showFavoriteOperationFailed(
                    m_favoriteProblemService->lastError());
                return;
            }

            m_favoritePage->showFavorites(
                folderId,
                folderName,
                m_favoriteProblemService->loadFavoritesInFolder(folderId));
        });
    connect(m_storagePage, &StoragePage::backRequested, this, &MainWindow::popPage);
    connect(m_storagePage, &StoragePage::homeRequested, this, [this]() {
        showRootPage(m_homePage);
    });
    connect(m_aiConfigPage, &AiConfigPage::backRequested, this, &MainWindow::popPage);
    connect(m_aiConfigPage, &AiConfigPage::homeRequested, this, [this]() {
        showRootPage(m_homePage);
    });
    connect(
        m_aiConfigPage,
        &AiConfigPage::saveRequested,
        this,
        [this](const QString &content) {
            QString errorMessage;
            QString savedPath;
            if (!AppConfig::saveConfigText(content, &savedPath, &errorMessage)) {
                m_aiConfigPage->showSaveFailed(errorMessage);
                return;
            }
            OpenAiConfig savedConfig = AppConfig::loadOpenAiConfig();
            m_openAiClient->setBaseUrl(savedConfig.baseUrl);
            m_aiService->setConfig(savedConfig);
            m_aiConfigSummary = QString("Config: %1 | model: %2")
                                    .arg(savedConfig.sourcePath.isEmpty()
                                             ? QString("defaults")
                                             : savedConfig.sourcePath,
                                         savedConfig.model);
            m_problemPage->setAiConfigSummary(m_aiConfigSummary);
            m_aiConfigPage->showSaveSucceeded(savedPath);
        });
    connect(
        m_storagePage,
        &StoragePage::clearCacheRequested,
        this,
        [this]() {
            m_storagePage->showClearing(true);
            if (!m_cacheService->clearAllCaches()) {
                m_storagePage->showOperationFailed(m_cacheService->lastError());
                return;
            }

            m_storagePage->showClearSucceeded(
                m_cacheService->formattedTotalCacheSize(),
                m_applicationSizeService->formattedTotalApplicationSize());
        });

    connect(
        m_aiService,
        &AiService::responseDelta,
        this,
        [this](const QString &delta) {
            m_problemPage->appendAiResponse(delta);
        });
    connect(
        m_aiService,
        &AiService::thinkingChanged,
        this,
        [this](bool thinking) {
            m_problemPage->showAiThinking(thinking);
        });
    connect(
        m_aiService,
        &AiService::responseReady,
        this,
        [this](const QString &text) {
            m_problemPage->showAiResponse(text);
        });
    connect(
        m_aiService,
        &AiService::failed,
        this,
        [this](const QString &message) {
            m_problemPage->showAiFailed(message);
        });
    connect(
        m_client,
        &OpenJudgeClient::judgeFinished,
        this,
        [this](const NetworkResult &result) {
            m_problemPage->showTesting(false);
            const QString toolResultText = m_lastTestRequestLog + "\n\n" + formatJudgeResult(result);
            if (!result.ok) {
                const QString failureText =
                    m_lastTestRequestLog
                    + QString("\n\nJudge Response\nHTTP status: %1\nNetwork error: %2")
                          .arg(QString::number(result.statusCode), result.errorString);
                m_problemPage->showTestFailed(failureText);
                if (!m_pendingAiRunTestCallId.isEmpty()) {
                    m_aiService->failToolCall(m_pendingAiRunTestCallId, failureText);
                    m_pendingAiRunTestCallId.clear();
                }
                return;
            }
            m_problemPage->showTestResult(toolResultText);
            if (!m_pendingAiRunTestCallId.isEmpty()) {
                m_aiService->completeToolCall(m_pendingAiRunTestCallId, toolResultText);
                m_pendingAiRunTestCallId.clear();
            }
        });
    connect(
        m_favoriteProblemService,
        &FavoriteProblemService::favoriteLoaded,
        this,
        [this](const ProblemPageInfo &problemPageInfo) {
            setCurrentProblem(problemPageInfo);
            m_problemPage->showProblemLoadedFromFavorites(problemPageInfo);
            if (!problemPageInfo.submitUrl.isEmpty()) {
                m_problemPage->openSubmit(problemPageInfo);
                m_submitService->openSubmit(QUrl(problemPageInfo.submitUrl));
            }
        });
    connect(
        m_favoriteProblemService,
        &FavoriteProblemService::failed,
        this,
        [this](const QString &message) {
            m_favoritePage->showFavoriteOperationFailed(message);
            if (ui->pageStack->currentWidget() == m_problemPage) {
                m_problemPage->showProblemLoadFailed(message);
            }
        });
    connect(
        m_resultService,
        &ResultService::loadingChanged,
        this,
        [this](bool loading) {
            Q_UNUSED(loading);
        });
    connect(
        m_resultService,
        &ResultService::resultLoaded,
        this,
        [this](const ResultPageInfo &resultPageInfo) {
            m_problemPage->appendResultPageInfo(resultPageInfo);
        });
    connect(
        m_resultService,
        &ResultService::failed,
        this,
        [this](const QString &message) {
            m_problemPage->appendResultFailure(message);
        });

    connect(
        m_submitService,
        &SubmitService::loadingChanged,
        this,
        [this](bool loading) {
            m_problemPage->showLoadingSubmitOptions(loading);
        });
    connect(
        m_submitService,
        &SubmitService::submittingChanged,
        this,
        [this](bool submitting) {
            m_problemPage->showSubmitting(submitting);
        });
    connect(
        m_submitService,
        &SubmitService::submitPageLoaded,
        this,
        [this](const SubmitPageInfo &submitPageInfo) {
            writeStartupLog("MainWindow: submitPageLoaded signal received");
            m_problemPage->showSubmitPageLoaded(
                submitPageInfo, m_submitService->defaultLanguage());
            writeStartupLog("MainWindow: problem page showSubmitPageLoaded returned");
        });
    connect(
        m_submitService,
        &SubmitService::submitPayloadBuilt,
        this,
        [this](const QString &languageValue,
               const QString &,
               const QByteArray &payload) {
            m_problemPage->showSubmitPayloadBuilt(languageValue, payload);
        });
    connect(
        m_submitService,
        &SubmitService::solutionSubmitted,
        this,
        [this](const NetworkResult &result) {
            QString redirectUrl;
            const QJsonDocument jsonDocument = QJsonDocument::fromJson(result.body);
            if (jsonDocument.isObject()) {
                redirectUrl = jsonDocument.object().value("redirect").toString();
            }

            m_problemPage->showSubmitResult(result);
            if (!m_pendingAiSubmitCallId.isEmpty()) {
                m_aiService->completeToolCall(
                    m_pendingAiSubmitCallId,
                    formatSubmitResponse(result));
                m_pendingAiSubmitCallId.clear();
            }
            if (!redirectUrl.isEmpty()) {
                m_resultService->openResult(QUrl(redirectUrl));
            }
        });
    connect(
        m_submitService,
        &SubmitService::failed,
        this,
        [this](const QString &message) {
            m_problemPage->showSubmitFailed(message);
            if (!m_pendingAiSubmitCallId.isEmpty()) {
                m_aiService->failToolCall(
                    m_pendingAiSubmitCallId,
                    QString("Submit failed.\n%1").arg(message));
                m_pendingAiSubmitCallId.clear();
            }
        });

    connect(
        m_homeService,
        &HomeService::loadingChanged,
        this,
        [this](bool loading) {
            if (loading) {
                m_homePage->showLoadingClasses();
            }
        });
    connect(
        m_homeService,
        &HomeService::failed,
        this,
        [this](const QString &message) {
            m_homePage->showHomeLoadFailed(message);
        });
    connect(
        m_homeService,
        &HomeService::homeLoaded,
        this,
        [this](const QList<JoinedClassInfo> &classes) {
            showRootPage(m_homePage);
            m_homePage->showClasses(classes);
            m_homePage->showReminderLoading();
            m_reminderService->refreshReminders(classes);
        });
    connect(
        m_reminderService,
        &ReminderService::remindersUpdated,
        this,
        [this](const QList<DeadlineReminder> &reminders) {
            m_homePage->showReminders(reminders);
        });

    connect(
        m_classService,
        &ClassService::loadingChanged,
        this,
        [this](bool loading) {
            if (loading) {
                m_classPage->showLoadingContests();
            }
        });
    connect(
        m_classPage,
        &ClassPage::refreshRequested,
        this,
        [this]() {
            const QUrl classUrl = m_classService->currentClassUrl();
            if (!classUrl.isValid() || classUrl.scheme().isEmpty()) {
                return;
            }
            m_classPage->showLoadingContests();
            m_classService->openClass(classUrl);
        });
    connect(
        m_classService,
        &ClassService::failed,
        this,
        [this](const QString &message) {
            m_classPage->showClassLoadFailed(message);
        });
    connect(
        m_classService,
        &ClassService::classLoaded,
        this,
        [this](const ClassPageInfo &, const GroupPageInfo &groupPageInfo) {
            m_classPage->showContestSets(groupPageInfo);
        });

    connect(
        m_contestService,
        &ContestService::loadingChanged,
        this,
        [this](bool loading) {
            if (loading) {
                m_contestPage->showLoadingProblems();
            }
        });
    connect(
        m_contestPage,
        &ContestPage::refreshRequested,
        this,
        [this]() {
            const QUrl contestUrl = m_contestService->currentContestUrl();
            if (!contestUrl.isValid() || contestUrl.scheme().isEmpty()) {
                return;
            }
            m_contestPage->showLoadingProblems();
            m_contestService->openContest(contestUrl);
        });
    connect(
        m_contestService,
        &ContestService::failed,
        this,
        [this](const QString &message) {
            m_contestPage->showContestLoadFailed(message);
        });
    connect(
        m_contestService,
        &ContestService::contestLoaded,
        this,
        [this](const ContestPageInfo &contestPageInfo) {
            m_contestPage->showProblems(contestPageInfo);
        });

    connect(
        m_problemService,
        &ProblemService::loadingChanged,
        this,
        [this](bool loading) {
            Q_UNUSED(loading);
        });
    connect(
        m_problemService,
        &ProblemService::failed,
        this,
        [this](const QString &message) {
            m_problemPage->showProblemLoadFailed(message);
        });
    connect(
        m_problemService,
        &ProblemService::problemLoaded,
        this,
        [this](const ProblemPageInfo &problemPageInfo) {
            writeStartupLog("MainWindow: problemLoaded signal received");
            setCurrentProblem(problemPageInfo);
            writeStartupLog("MainWindow: current problem set");
            m_problemPage->showProblem(problemPageInfo);
            writeStartupLog("MainWindow: problem page content shown");
            if (!problemPageInfo.submitUrl.isEmpty()) {
                writeStartupLog("MainWindow: openSubmit begin");
                m_problemPage->openSubmit(problemPageInfo);
                writeStartupLog("MainWindow: problem page openSubmit returned");
                m_submitService->openSubmit(QUrl(problemPageInfo.submitUrl));
                writeStartupLog("MainWindow: submitService openSubmit called");
            }
        });
}

void MainWindow::showRootPage(QWidget *page)
{
    m_navigationHistory.clear();
    const int index = ui->pageStack->indexOf(page);
    if (index >= 0) {
        ui->pageStack->setCurrentIndex(index);
    }
}

void MainWindow::pushPage(QWidget *page)
{
    const int currentIndex = ui->pageStack->currentIndex();
    const int nextIndex = ui->pageStack->indexOf(page);
    if (nextIndex < 0 || currentIndex == nextIndex) {
        return;
    }

    m_navigationHistory.append(currentIndex);
    ui->pageStack->setCurrentIndex(nextIndex);
}

void MainWindow::popPage()
{
    if (m_navigationHistory.isEmpty()) {
        showRootPage(m_homePage);
        return;
    }

    const int index = m_navigationHistory.takeLast();
    ui->pageStack->setCurrentIndex(index);
}

void MainWindow::setCurrentProblem(const ProblemPageInfo &problemPageInfo)
{
    m_currentProblemInfo = problemPageInfo;
    m_hasCurrentProblem = !problemPageInfo.problemUrl.isEmpty();
    m_problemPage->setFavoriteEnabled(m_hasCurrentProblem);
    m_problemPage->setSubmitEnabled(
        m_hasCurrentProblem && !problemPageInfo.submitUrl.isEmpty());
}

void MainWindow::applyDarkMode(bool dark)
{
    m_darkMode = dark;
    if (ui != nullptr) {
        const QString rootStyle = dark
            ? QString(
                  "#centralwidget, #pageStack {"
                  "  background: #000000;"
                  "}")
            : QString(
                  "#centralwidget, #pageStack {"
                  "  background: #ffffff;"
                  "}");
        ui->centralwidget->setStyleSheet(rootStyle);
        ui->pageStack->setStyleSheet(rootStyle);
    }
    if (m_loginPage != nullptr) {
        m_loginPage->setDarkMode(dark);
    }
    if (m_homePage != nullptr) {
        m_homePage->setDarkMode(dark);
    }
    if (m_classPage != nullptr) {
        m_classPage->setDarkMode(dark);
    }
    if (m_contestPage != nullptr) {
        m_contestPage->setDarkMode(dark);
    }
    if (m_problemPage != nullptr) {
        m_problemPage->setDarkMode(dark);
    }
    if (m_favoritePage != nullptr) {
        m_favoritePage->setDarkMode(dark);
    }
    if (m_storagePage != nullptr) {
        m_storagePage->setDarkMode(dark);
    }
    if (m_aiConfigPage != nullptr) {
        m_aiConfigPage->setDarkMode(dark);
    }
}

void MainWindow::openProblemPage(const QString &problemUrl,
                                 const QString &problemTitle,
                                 bool preferFavoriteCache)
{
    m_problemPage->openProblem(problemTitle);
    pushPage(m_problemPage);

    if (preferFavoriteCache
        && m_favoriteProblemService->tryLoadFavorite(problemUrl)) {
        return;
    }

    m_problemService->openProblem(QUrl(problemUrl));
}

void MainWindow::saveCurrentProblemToFavorites()
{
    if (!m_hasCurrentProblem) {
        return;
    }

    const QList<FavoriteFolderInfo> folders = m_favoriteProblemService->loadFolders();
    if (folders.isEmpty()) {
        const QString newFolderName = QInputDialog::getText(
            this,
            "Create Favorite Folder",
            "No favorite folders yet. Create one:");
        if (newFolderName.trimmed().isEmpty()) {
            return;
        }

        if (!m_favoriteProblemService->createFolder(newFolderName.trimmed())) {
            m_problemPage->showProblemLoadFailed(m_favoriteProblemService->lastError());
            return;
        }
    }

    const QList<FavoriteFolderInfo> selectableFolders = m_favoriteProblemService->loadFolders();
    if (selectableFolders.isEmpty()) {
        m_problemPage->showProblemLoadFailed("No favorite folder available.");
        return;
    }

    QStringList folderNames;
    for (const FavoriteFolderInfo &folder : selectableFolders) {
        folderNames.append(folder.name);
    }

    bool ok = false;
    const QString selectedFolderName = QInputDialog::getItem(
        this,
        "Select Favorite Folder",
        "Save current problem to:",
        folderNames,
        0,
        false,
        &ok);
    if (!ok || selectedFolderName.isEmpty()) {
        return;
    }

    qint64 folderId = -1;
    for (const FavoriteFolderInfo &folder : selectableFolders) {
        if (folder.name == selectedFolderName) {
            folderId = folder.id;
            break;
        }
    }
    if (folderId < 0) {
        m_problemPage->showProblemLoadFailed("Selected favorite folder not found.");
        return;
    }

    if (!m_favoriteProblemService->saveFavoriteToFolder(m_currentProblemInfo, folderId)) {
        m_problemPage->showProblemLoadFailed(m_favoriteProblemService->lastError());
        return;
    }

    if (ui->pageStack->currentWidget() == m_favoritePage) {
        m_favoritePage->showFavorites(
            folderId,
            selectedFolderName,
            m_favoriteProblemService->loadFavoritesInFolder(folderId));
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_allowClose || m_trayIcon == nullptr || !m_trayIcon->isVisible()) {
        QMainWindow::closeEvent(event);
        return;
    }

    hide();
    event->ignore();
    if (!m_trayNoticeShown) {
        m_trayIcon->showMessage("oj-client",
                                "Application is still running in the system tray.",
                                QSystemTrayIcon::Information,
                                3000);
        m_trayNoticeShown = true;
    }
}

void MainWindow::restoreFromTray()
{
    showNormal();
    raise();
    activateWindow();
}

bool MainWindow::requiresEmailVerification(const QString &email) const
{
    const QString normalizedEmail = email.trimmed();
    if (normalizedEmail.isEmpty()) {
        return false;
    }

    if (normalizedEmail == m_verifiedEmail) {
        return false;
    }

    CachedLoginInfo loginInfo;
    return !m_loginCacheService->tryLoadLoginByEmail(normalizedEmail, &loginInfo);
}

void MainWindow::applyLoginCacheState(const QString &email)
{
    const QString normalizedEmail = email.trimmed();
    if (normalizedEmail != m_verifiedEmail) {
        m_verifiedEmail.clear();
    }

    CachedLoginInfo loginInfo;
    if (m_loginCacheService->tryLoadLoginByEmail(normalizedEmail, &loginInfo)) {
        m_loginPage->setPassword(loginInfo.password);
        m_loginPage->setVerificationRequired(false);
        return;
    }

    if (!normalizedEmail.isEmpty()) {
        m_loginPage->setVerificationRequired(true);
    } else {
        m_loginPage->setVerificationRequired(false);
    }
}
