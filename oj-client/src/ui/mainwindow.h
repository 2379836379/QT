#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "parser/problemparser.h"

#include <QMainWindow>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class OpenJudgeClient;
class CookieStore;
class HomeRepository;
class HomeCacheRepository;
class ClassRepository;
class ClassCacheRepository;
class ContestRepository;
class ContestCacheRepository;
class ProblemRepository;
class ProblemCacheRepository;
class FavoriteProblemRepository;
class LoginCacheRepository;
class ResultRepository;
class SubmitRepository;
class CacheService;
class LoginService;
class LoginCacheService;
class EmailVerifyService;
class ApplicationSizeService;
class HomeService;
class ClassService;
class ContestService;
class ProblemService;
class FavoriteProblemService;
class ResultService;
class SubmitService;
class ReminderService;
class LoginPage;
class HomePage;
class ClassPage;
class ContestPage;
class ProblemPage;
class FavoritePage;
class StoragePage;
class QWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    void setupUiState();
    void connectSignals();
    void showRootPage(QWidget *page);
    void pushPage(QWidget *page);
    void popPage();
    void setCurrentProblem(const ProblemPageInfo &problemPageInfo);
    void openProblemPage(const QString &problemUrl,
                         const QString &problemTitle,
                         bool preferFavoriteCache);
    void saveCurrentProblemToFavorites();
    bool requiresEmailVerification(const QString &email) const;
    void applyLoginCacheState(const QString &email);

private:
    Ui::MainWindow *ui;
    OpenJudgeClient *m_client = nullptr;
    CookieStore *m_cookieStore = nullptr;
    HomeRepository *m_homeRepository = nullptr;
    HomeCacheRepository *m_homeCacheRepository = nullptr;
    ClassRepository *m_classRepository = nullptr;
    ClassCacheRepository *m_classCacheRepository = nullptr;
    ContestRepository *m_contestRepository = nullptr;
    ContestCacheRepository *m_contestCacheRepository = nullptr;
    ProblemRepository *m_problemRepository = nullptr;
    ProblemCacheRepository *m_problemCacheRepository = nullptr;
    LoginCacheRepository *m_loginCacheRepository = nullptr;
    ResultRepository *m_resultRepository = nullptr;
    SubmitRepository *m_submitRepository = nullptr;
    CacheService *m_cacheService = nullptr;
    LoginService *m_loginService = nullptr;
    LoginCacheService *m_loginCacheService = nullptr;
    EmailVerifyService *m_emailVerifyService = nullptr;
    ApplicationSizeService *m_applicationSizeService = nullptr;
    HomeService *m_homeService = nullptr;
    ClassService *m_classService = nullptr;
    ContestService *m_contestService = nullptr;
    ProblemService *m_problemService = nullptr;
    FavoriteProblemRepository *m_favoriteProblemRepository = nullptr;
    FavoriteProblemService *m_favoriteProblemService = nullptr;
    ResultService *m_resultService = nullptr;
    SubmitService *m_submitService = nullptr;
    ClassRepository *m_reminderClassRepository = nullptr;
    ReminderService *m_reminderService = nullptr;
    LoginPage *m_loginPage = nullptr;
    HomePage *m_homePage = nullptr;
    ClassPage *m_classPage = nullptr;
    ContestPage *m_contestPage = nullptr;
    ProblemPage *m_problemPage = nullptr;
    FavoritePage *m_favoritePage = nullptr;
    StoragePage *m_storagePage = nullptr;
    QVector<int> m_navigationHistory;
    ProblemPageInfo m_currentProblemInfo;
    bool m_hasCurrentProblem = false;
    QString m_verifiedEmail;
    QString m_pendingLoginEmail;
    QString m_pendingLoginPassword;
};

#endif // MAINWINDOW_H
