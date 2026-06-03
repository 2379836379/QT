#include "ui/pages/homepage.h"

#include "repository/cache/contestcacherepository.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QUrl>
#include <QVBoxLayout>

namespace
{
QString buildHomePageStyle(bool dark)
{
    if (!dark) {
        return QString(
            "HomePage { background: #f3f1eb; }"
            "#homeTopFrame, #homeLeftFrame, #homeMiddleFrame, #homeRightFrame {"
            "  background: #fbfaf7;"
            "  border: 1px solid #ded8cc;"
            "  border-radius: 16px;"
            "}"
            "#homeTitleLabel {"
            "  font-size: 28px;"
            "  font-weight: 600;"
            "  color: #1f2328;"
            "}"
            "#homeSubtitleLabel {"
            "  font-size: 13px;"
            "  color: #6b7280;"
            "}"
            "#homeInfoLabel {"
            "  font-size: 13px;"
            "  color: #5e675f;"
            "  line-height: 1.4;"
            "}"
            "#homeSectionLabel {"
            "  font-size: 16px;"
            "  font-weight: 600;"
            "  color: #2f3a33;"
            "}"
            "#homeToolsToggleButton {"
            "  background: transparent;"
            "  border: none;"
            "  padding: 0px;"
            "  text-align: left;"
            "  font-size: 16px;"
            "  font-weight: 600;"
            "  color: #2f3a33;"
            "}"
            "#homeToolsToggleButton:hover {"
            "  color: #12343b;"
            "}"
            "#homeToolButton {"
            "  padding: 10px 12px;"
            "  border: none;"
            "  border-radius: 10px;"
            "  background: transparent;"
            "  color: #2f3a33;"
            "  text-align: left;"
            "}"
            "#homeToolButton:hover, #homeToolIconButton:hover {"
            "  background: #eef4ef;"
            "}"
            "#homeRefreshButton {"
            "  min-width: 88px;"
            "  padding: 8px 14px;"
            "  border: 1px solid #cdd7cf;"
            "  border-radius: 10px;"
            "  background: #f7f5ef;"
            "  color: #243029;"
            "}"
            "#homeRefreshButton:hover {"
            "  background: #eef4ef;"
            "}"
            "#homeToolIconButton {"
            "  min-width: 36px;"
            "  max-width: 36px;"
            "  min-height: 36px;"
            "  max-height: 36px;"
            "  border: none;"
            "  border-radius: 10px;"
            "  background: transparent;"
            "  color: #2f3a33;"
            "  font-weight: 600;"
            "}"
            "#homeClassList, #homeReminderList {"
            "  background: transparent;"
            "  border: none;"
            "  border-radius: 0px;"
            "  padding: 0px;"
            "  outline: none;"
            "  color: #1f2328;"
            "}"
            "#homeClassList::item, #homeReminderList::item {"
            "  padding: 12px 4px;"
            "  border-radius: 8px;"
            "  margin: 2px 0px;"
            "}"
            "#homeClassList::item:selected, #homeReminderList::item:selected {"
            "  background: #dcefea;"
            "  color: #12343b;"
            "}"
            "#homeClassList::item:hover, #homeReminderList::item:hover {"
            "  background: #eef4ef;"
            "}"
        );
    }

    return QString(
        "HomePage { background: #000000; }"
        "#homeTopFrame, #homeLeftFrame, #homeMiddleFrame, #homeRightFrame {"
        "  background: #1b232c;"
        "  border: 1px solid #2c3844;"
        "  border-radius: 16px;"
        "}"
        "#homeTitleLabel {"
        "  font-size: 28px;"
        "  font-weight: 600;"
        "  color: #e8edf2;"
        "}"
        "#homeSubtitleLabel {"
        "  font-size: 13px;"
        "  color: #9ba8b6;"
        "}"
        "#homeInfoLabel {"
        "  font-size: 13px;"
        "  color: #b5c0cb;"
        "  line-height: 1.4;"
        "}"
        "#homeSectionLabel {"
        "  font-size: 16px;"
        "  font-weight: 600;"
        "  color: #d9e1e8;"
        "}"
        "#homeToolsToggleButton {"
        "  background: transparent;"
        "  border: none;"
        "  padding: 0px;"
        "  text-align: left;"
        "  font-size: 16px;"
        "  font-weight: 600;"
        "  color: #d9e1e8;"
        "}"
        "#homeToolsToggleButton:hover {"
        "  color: #ffffff;"
        "}"
        "#homeToolButton {"
        "  padding: 10px 12px;"
        "  border: none;"
        "  border-radius: 10px;"
        "  background: transparent;"
        "  color: #d9e1e8;"
        "  text-align: left;"
        "}"
        "#homeToolButton:hover, #homeToolIconButton:hover {"
        "  background: #26313c;"
        "}"
        "#homeRefreshButton {"
        "  min-width: 88px;"
        "  padding: 8px 14px;"
        "  border: 1px solid #3a4652;"
        "  border-radius: 10px;"
        "  background: #202a34;"
        "  color: #e8edf2;"
        "}"
        "#homeRefreshButton:hover {"
        "  background: #293542;"
        "}"
        "#homeToolIconButton {"
        "  min-width: 36px;"
        "  max-width: 36px;"
        "  min-height: 36px;"
        "  max-height: 36px;"
        "  border: none;"
        "  border-radius: 10px;"
        "  background: transparent;"
        "  color: #d9e1e8;"
        "  font-weight: 600;"
        "}"
        "#homeClassList, #homeReminderList {"
        "  background: transparent;"
        "  border: none;"
        "  border-radius: 0px;"
        "  padding: 0px;"
        "  outline: none;"
        "  color: #e8edf2;"
        "}"
        "#homeClassList::item, #homeReminderList::item {"
        "  padding: 12px 4px;"
        "  border-radius: 8px;"
        "  margin: 2px 0px;"
        "}"
        "#homeClassList::item:selected, #homeReminderList::item:selected {"
        "  background: #234257;"
        "  color: #eff8ff;"
        "}"
        "#homeClassList::item:hover, #homeReminderList::item:hover {"
        "  background: #26313c;"
        "}"
    );
}

QString formatReminderContestTitle(const DeadlineReminder &reminder)
{
    QString text = reminder.contestTitle;
    ContestCacheRepository cacheRepository;
    ContestPageInfo contestPageInfo;
    if (cacheRepository.loadContest(reminder.contestUrl, &contestPageInfo)
        && contestPageInfo.totalProblems > 0) {
        text += QString(" (%1/%2)")
                    .arg(QString::number(contestPageInfo.solvedProblems),
                         QString::number(contestPageInfo.totalProblems));
    }
    if (!reminder.deadlineText.isEmpty()) {
        text += " | " + reminder.deadlineText;
    }
    return text;
}

bool shouldHideCompletedReminder(const DeadlineReminder &reminder)
{
    ContestCacheRepository cacheRepository;
    ContestPageInfo contestPageInfo;
    if (!cacheRepository.loadContest(reminder.contestUrl, &contestPageInfo)) {
        return false;
    }
    return contestPageInfo.totalProblems > 0
        && contestPageInfo.solvedProblems >= contestPageInfo.totalProblems;
}
}

HomePage::HomePage(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(18);

    // 上框
    auto *topFrame = new QFrame(this);
    topFrame->setObjectName("homeTopFrame");
    auto *topLayout = new QHBoxLayout(topFrame);
    topFrame->setFixedHeight(80);
    topLayout->setContentsMargins(24, 18, 24, 18);
    topLayout->setSpacing(16);

    auto *titleBlock = new QVBoxLayout();
    titleBlock->setSpacing(6);

    auto *titleLabel = new QLabel("OJ", topFrame);
    titleLabel->setObjectName("homeTitleLabel");

    titleBlock->addWidget(titleLabel);

    topLayout->addLayout(titleBlock, 1);
    auto *homeButton = new QPushButton("Home", topFrame);
    homeButton->setObjectName("homeRefreshButton");
    topLayout->addWidget(homeButton, 0, Qt::AlignRight);
    m_themeButton = new QPushButton("Dark Mode", topFrame);
    m_themeButton->setObjectName("homeRefreshButton");
    topLayout->addWidget(m_themeButton, 0, Qt::AlignRight);
    auto *refreshButton = new QPushButton("Refresh", topFrame);
    refreshButton->setObjectName("homeRefreshButton");
    topLayout->addWidget(refreshButton, 0, Qt::AlignRight);

    auto *bottomLayout = new QHBoxLayout();
    bottomLayout->setSpacing(18);

    m_toolsFrame = new QFrame(this);
    m_toolsFrame->setObjectName("homeLeftFrame");
    auto *leftLayout = new QVBoxLayout(m_toolsFrame);
    leftLayout->setContentsMargins(20, 18, 20, 18);
    leftLayout->setSpacing(12);

    m_toolsToggleButton = new QPushButton(m_toolsFrame);
    m_toolsToggleButton->setObjectName("homeToolsToggleButton");

    m_toolsPanel = new QWidget(m_toolsFrame);
    auto *toolsPanelLayout = new QVBoxLayout(m_toolsPanel);
    toolsPanelLayout->setContentsMargins(0, 0, 0, 0);
    toolsPanelLayout->setSpacing(10);

    m_favoritesButton = new QPushButton("Open Favorites", m_toolsPanel);
    m_favoritesButton->setObjectName("homeToolButton");
    m_storageButton = new QPushButton("Storage", m_toolsPanel);
    m_storageButton->setObjectName("homeToolButton");
    m_aiConfigButton = new QPushButton("AI Config", m_toolsPanel);
    m_aiConfigButton->setObjectName("homeToolButton");
    m_logoutButton = new QPushButton("Log Out", m_toolsPanel);
    m_logoutButton->setObjectName("homeToolButton");

    m_collapsedToolsPanel = new QWidget(m_toolsFrame);
    m_collapsedToolsPanel->setObjectName("homeCollapsedToolsPanel");
    auto *collapsedLayout = new QVBoxLayout(m_collapsedToolsPanel);
    collapsedLayout->setContentsMargins(0, 0, 0, 0);
    collapsedLayout->setSpacing(10);

    m_collapsedFavoritesButton = new QPushButton("F", m_collapsedToolsPanel);
    m_collapsedFavoritesButton->setObjectName("homeToolIconButton");
    m_collapsedFavoritesButton->setToolTip("Open Favorites");
    m_collapsedStorageButton = new QPushButton("S", m_collapsedToolsPanel);
    m_collapsedStorageButton->setObjectName("homeToolIconButton");
    m_collapsedStorageButton->setToolTip("Storage");
    m_collapsedAiConfigButton = new QPushButton("A", m_collapsedToolsPanel);
    m_collapsedAiConfigButton->setObjectName("homeToolIconButton");
    m_collapsedAiConfigButton->setToolTip("AI Config");
    m_collapsedLogoutButton = new QPushButton("L", m_collapsedToolsPanel);
    m_collapsedLogoutButton->setObjectName("homeToolIconButton");
    m_collapsedLogoutButton->setToolTip("Log Out");

    collapsedLayout->addWidget(m_collapsedFavoritesButton);
    collapsedLayout->addWidget(m_collapsedStorageButton);
    collapsedLayout->addWidget(m_collapsedAiConfigButton);
    collapsedLayout->addWidget(m_collapsedLogoutButton);
    collapsedLayout->addStretch();

    toolsPanelLayout->addWidget(m_favoritesButton);
    toolsPanelLayout->addWidget(m_storageButton);
    toolsPanelLayout->addWidget(m_aiConfigButton);
    toolsPanelLayout->addWidget(m_logoutButton);
    toolsPanelLayout->addStretch();

    leftLayout->addWidget(m_toolsToggleButton);
    leftLayout->addWidget(m_toolsPanel);
    leftLayout->addWidget(m_collapsedToolsPanel);
    leftLayout->addStretch();
   
    m_contentCard = new QFrame(this);
    m_contentCard->setObjectName("homeMiddleFrame");
    auto *cardLayout = new QVBoxLayout(m_contentCard);
    cardLayout->setContentsMargins(20, 18, 20, 18);
    cardLayout->setSpacing(14);

    auto *sectionLabel = new QLabel("Course List", m_contentCard);
    sectionLabel->setObjectName("homeSectionLabel");

    m_classListWidget = new QListWidget(m_contentCard);
    m_classListWidget->setObjectName("homeClassList");
    m_classListWidget->setMinimumHeight(420);

    cardLayout->addWidget(sectionLabel);
    cardLayout->addWidget(m_classListWidget, 1);

    auto *rightFrame = new QFrame(this);
    rightFrame->setObjectName("homeRightFrame");
    auto *rightLayout = new QVBoxLayout(rightFrame);
    rightLayout->setContentsMargins(20, 18, 20, 18);
    rightLayout->setSpacing(12);

    auto *rightTitleLabel = new QLabel("Panel", rightFrame);
    rightTitleLabel->setObjectName("homeSectionLabel");
    rightTitleLabel->setText("Due Soon");

    m_reminderListWidget = new QListWidget(rightFrame);
    m_reminderListWidget->setObjectName("homeReminderList");

    rightLayout->addWidget(rightTitleLabel);
    rightLayout->addWidget(m_reminderListWidget, 1);

    bottomLayout->addWidget(m_toolsFrame, 1);
    bottomLayout->addWidget(m_contentCard, 2);
    bottomLayout->addWidget(rightFrame, 2);

    layout->addWidget(topFrame);
    layout->addLayout(bottomLayout, 1);

    setDarkMode(false);

    setToolsExpanded(true);

    connect(
        m_classListWidget,
        &QListWidget::itemClicked,
        this,
        [this](QListWidgetItem *item) {
            emit classSelected(
                item->text(),
                item->data(Qt::UserRole).toString());
        });
    connect(m_favoritesButton, &QPushButton::clicked, this, &HomePage::favoritesRequested);
    connect(m_storageButton, &QPushButton::clicked, this, &HomePage::storageRequested);
    connect(m_aiConfigButton, &QPushButton::clicked, this, &HomePage::aiConfigRequested);
    connect(m_logoutButton, &QPushButton::clicked, this, &HomePage::logoutRequested);
    connect(homeButton, &QPushButton::clicked, this, [this]() {
        Q_UNUSED(this);
    });
    connect(refreshButton, &QPushButton::clicked, this, &HomePage::refreshRequested);
    connect(
        m_themeButton,
        &QPushButton::clicked,
        this,
        [this]() {
            emit themeToggleRequested(!m_darkMode);
        });
    connect(m_collapsedFavoritesButton, &QPushButton::clicked, this, &HomePage::favoritesRequested);
    connect(m_collapsedStorageButton, &QPushButton::clicked, this, &HomePage::storageRequested);
    connect(m_collapsedAiConfigButton, &QPushButton::clicked, this, &HomePage::aiConfigRequested);
    connect(m_collapsedLogoutButton, &QPushButton::clicked, this, &HomePage::logoutRequested);
    connect(
        m_toolsToggleButton,
        &QPushButton::clicked,
        this,
        [this]() {
            setToolsExpanded(!m_toolsExpanded);
        });
    connect(
        m_reminderListWidget,
        &QListWidget::itemClicked,
        this,
        [this](QListWidgetItem *item) {
            const QString contestUrl = item->data(Qt::UserRole).toString();
            const QString contestTitle = item->data(Qt::UserRole + 1).toString();
            const QUrl parsedUrl(contestUrl);
            if (!contestUrl.isEmpty() && parsedUrl.isValid()
                && !parsedUrl.scheme().isEmpty()) {
                emit reminderSelected(
                    contestTitle.isEmpty() ? item->text() : contestTitle,
                    contestUrl);
            }
        });
}

void HomePage::setDarkMode(bool dark)
{
    m_darkMode = dark;
    setStyleSheet(buildHomePageStyle(dark));
    if (m_themeButton != nullptr) {
        m_themeButton->setText(dark ? "Light Mode" : "Dark Mode");
    }
}

void HomePage::setToolsExpanded(bool expanded)
{
    m_toolsExpanded = expanded;
    if (m_toolsFrame != nullptr) {
        m_toolsFrame->setMinimumWidth(expanded ? 0 : 84);
        m_toolsFrame->setMaximumWidth(expanded ? QWIDGETSIZE_MAX : 84);
    }
    if (m_toolsPanel != nullptr) {
        m_toolsPanel->setVisible(expanded);
    }
    if (m_collapsedToolsPanel != nullptr) {
        m_collapsedToolsPanel->setVisible(!expanded);
    }
    const QList<QPushButton *> iconButtons = {m_collapsedFavoritesButton,
                                              m_collapsedStorageButton,
                                              m_collapsedAiConfigButton,
                                              m_collapsedLogoutButton};
    for (QPushButton *button : iconButtons) {
        if (button != nullptr) {
            button->setVisible(!expanded);
        }
    }
    if (m_toolsToggleButton != nullptr) {
        m_toolsToggleButton->setText(expanded ? "Tools v" : ">");
    }
}

void HomePage::showLoggingIn()
{
    showReminderLoading();
}

void HomePage::showLoginFailed(const QString &message)
{
    Q_UNUSED(message);
}

void HomePage::showOpeningHome()
{
    showReminderLoading();
}

void HomePage::showLoadingClasses()
{
    showReminderLoading();
}

void HomePage::showHomeLoadFailed(const QString &message)
{
    Q_UNUSED(message);
}

void HomePage::showClasses(const QList<JoinedClassInfo> &classes)
{
    m_classListWidget->clear();
    for (const JoinedClassInfo &joinedClass : classes) {
        const QString displayText = joinedClass.name.isEmpty()
            ? QStringLiteral("Unnamed class")
            : joinedClass.name;
        auto *item = new QListWidgetItem(displayText, m_classListWidget);
        item->setData(Qt::UserRole, joinedClass.url);
    }
}

void HomePage::showReminderLoading()
{
    if (m_reminderListWidget == nullptr) {
        return;
    }

    m_reminderListWidget->clear();
    new QListWidgetItem("Checking course deadlines...", m_reminderListWidget);
}

void HomePage::showReminders(const QList<DeadlineReminder> &reminders)
{
    if (m_reminderListWidget == nullptr) {
        return;
    }

    m_reminderListWidget->clear();
    QList<DeadlineReminder> visibleReminders;
    for (const DeadlineReminder &reminder : reminders) {
        if (!shouldHideCompletedReminder(reminder)) {
            visibleReminders.append(reminder);
        }
    }

    if (visibleReminders.isEmpty()) {
        new QListWidgetItem("No contest deadlines within one week.", m_reminderListWidget);
        return;
    }

    for (const DeadlineReminder &reminder : visibleReminders) {
        auto *item = new QListWidgetItem(
            formatReminderContestTitle(reminder),
            m_reminderListWidget);
        item->setData(Qt::UserRole, reminder.contestUrl);
        item->setData(Qt::UserRole + 1, reminder.contestTitle);
        item->setToolTip(
            QString("%1\n%2").arg(reminder.courseName, reminder.deadlineText));
    }
}
