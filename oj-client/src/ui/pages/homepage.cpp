#include "ui/pages/homepage.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QUrl>
#include <QVBoxLayout>

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

    auto *bottomLayout = new QHBoxLayout();
    bottomLayout->setSpacing(18);

    auto *leftFrame = new QFrame(this);
    leftFrame->setObjectName("homeLeftFrame");
    auto *leftLayout = new QVBoxLayout(leftFrame);
    leftLayout->setContentsMargins(20, 18, 20, 18);
    leftLayout->setSpacing(12);

    auto *leftTitleLabel = new QLabel("Tools", leftFrame);
    leftTitleLabel->setObjectName("homeSectionLabel");

    m_toolsListWidget = new QListWidget(leftFrame);
    m_toolsListWidget->setObjectName("homeToolsList");
    auto *favoritesItem = new QListWidgetItem("Open Favorites", m_toolsListWidget);
    favoritesItem->setData(Qt::UserRole, "favorites");
    auto *logoutItem = new QListWidgetItem("Log Out", m_toolsListWidget);
    logoutItem->setData(Qt::UserRole, "logout");

    leftLayout->addWidget(leftTitleLabel);
    leftLayout->addWidget(m_toolsListWidget);
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

    bottomLayout->addWidget(leftFrame, 1);
    bottomLayout->addWidget(m_contentCard, 2);
    bottomLayout->addWidget(rightFrame, 2);

    layout->addWidget(topFrame);
    layout->addLayout(bottomLayout, 1);

    setStyleSheet(
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
        "#homeClassList {"
        "  background: transparent;"
        "  border: none;"
        "  border-radius: 0px;"
        "  padding: 0px;"
        "  outline: none;"
        "}"
        "#homeToolsList {"
        "  background: transparent;"
        "  border: none;"
        "  border-radius: 0px;"
        "  padding: 0px;"
        "  outline: none;"
        "}"
        "#homeReminderList {"
        "  background: transparent;"
        "  border: none;"
        "  border-radius: 0px;"
        "  padding: 0px;"
        "  outline: none;"
        "}"
        "#homeClassList::item, #homeToolsList::item, #homeReminderList::item {"
        "  padding: 12px 4px;"
        "  border-radius: 8px;"
        "  margin: 2px 0px;"
        "}"
        "#homeClassList::item:selected, #homeToolsList::item:selected, #homeReminderList::item:selected {"
        "  background: #dcefea;"
        "  color: #12343b;"
        "}"
        "#homeClassList::item:hover, #homeToolsList::item:hover, #homeReminderList::item:hover {"
        "  background: #eef4ef;"
        "}"
    );

    connect(
        m_classListWidget,
        &QListWidget::itemClicked,
        this,
        [this](QListWidgetItem *item) {
            emit classSelected(
                item->text(),
                item->data(Qt::UserRole).toString());
        });
    connect(
        m_toolsListWidget,
        &QListWidget::itemClicked,
        this,
        [this](QListWidgetItem *item) {
            const QString action = item->data(Qt::UserRole).toString();
            if (action == "favorites") {
                emit favoritesRequested();
            } else if (action == "logout") {
                emit logoutRequested();
            }
        });
    connect(
        m_reminderListWidget,
        &QListWidget::itemClicked,
        this,
        [this](QListWidgetItem *item) {
            const QString contestUrl = item->data(Qt::UserRole).toString();
            const QUrl parsedUrl(contestUrl);
            if (!contestUrl.isEmpty() && parsedUrl.isValid()
                && !parsedUrl.scheme().isEmpty()) {
                emit reminderSelected(item->text(), contestUrl);
            }
        });
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
    if (reminders.isEmpty()) {
        new QListWidgetItem("No contest deadlines within one week.", m_reminderListWidget);
        return;
    }

    for (const DeadlineReminder &reminder : reminders) {
        auto *item = new QListWidgetItem(reminder.contestTitle, m_reminderListWidget);
        item->setData(Qt::UserRole, reminder.contestUrl);
        item->setToolTip(
            QString("%1\n%2").arg(reminder.courseName, reminder.deadlineText));
    }
}
