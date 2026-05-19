#include "ui/pages/classpage.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QVBoxLayout>

ClassPage::ClassPage(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(18);

    auto *topFrame = new QFrame(this);
    topFrame->setObjectName("classTopFrame");
    topFrame->setFixedHeight(80);
    auto *topLayout = new QHBoxLayout(topFrame);
    topLayout->setContentsMargins(24, 18, 24, 18);
    topLayout->setSpacing(16);

    auto *backButton = new QPushButton("Back", topFrame);
    backButton->setObjectName("classBackButton");
    m_titleLabel = new QLabel("Course", topFrame);
    m_titleLabel->setObjectName("classTitleLabel");

    topLayout->addWidget(backButton);
    topLayout->addWidget(m_titleLabel, 1);

    auto *bottomLayout = new QHBoxLayout();
    bottomLayout->setSpacing(18);

    auto *leftFrame = new QFrame(this);
    leftFrame->setObjectName("classLeftFrame");
    auto *leftLayout = new QVBoxLayout(leftFrame);
    leftLayout->setContentsMargins(20, 18, 20, 18);
    leftLayout->setSpacing(12);

    auto *toolsLabel = new QLabel("Tools", leftFrame);
    toolsLabel->setObjectName("classSectionLabel");

    m_toolsListWidget = new QListWidget(leftFrame);
    m_toolsListWidget->setObjectName("classToolsList");
    auto *backItem = new QListWidgetItem("Back", m_toolsListWidget);
    backItem->setData(Qt::UserRole, "back");

    leftLayout->addWidget(toolsLabel);
    leftLayout->addWidget(m_toolsListWidget);
    leftLayout->addStretch();

    auto *contentFrame = new QFrame(this);
    contentFrame->setObjectName("classContentFrame");
    auto *contentLayout = new QVBoxLayout(contentFrame);
    contentLayout->setContentsMargins(20, 18, 20, 18);
    contentLayout->setSpacing(14);

    auto *contestLabel = new QLabel("Contest List", contentFrame);
    contestLabel->setObjectName("classSectionLabel");

    m_contestListWidget = new QListWidget(contentFrame);
    m_contestListWidget->setObjectName("classContestList");
    m_contestListWidget->setMinimumHeight(420);

    contentLayout->addWidget(contestLabel);
    contentLayout->addWidget(m_contestListWidget, 1);

    bottomLayout->addWidget(leftFrame, 1);
    bottomLayout->addWidget(contentFrame, 4);

    layout->addWidget(topFrame);
    layout->addLayout(bottomLayout, 1);

    setStyleSheet(
        "ClassPage { background: #f3f1eb; }"
        "#classTopFrame, #classLeftFrame, #classContentFrame {"
        "  background: #fbfaf7;"
        "  border: 1px solid #ded8cc;"
        "  border-radius: 16px;"
        "}"
        "#classTitleLabel {"
        "  font-size: 28px;"
        "  font-weight: 600;"
        "  color: #1f2328;"
        "}"
        "#classSectionLabel {"
        "  font-size: 16px;"
        "  font-weight: 600;"
        "  color: #2f3a33;"
        "}"
        "#classBackButton {"
        "  min-width: 88px;"
        "  padding: 8px 14px;"
        "  border: 1px solid #cdd7cf;"
        "  border-radius: 10px;"
        "  background: #f7f5ef;"
        "  color: #243029;"
        "}"
        "#classBackButton:hover {"
        "  background: #eef4ef;"
        "}"
        "#classToolsList, #classContestList {"
        "  background: transparent;"
        "  border: none;"
        "  border-radius: 0px;"
        "  padding: 0px;"
        "  outline: none;"
        "}"
        "#classToolsList::item, #classContestList::item {"
        "  padding: 12px 4px;"
        "  border-radius: 8px;"
        "  margin: 2px 0px;"
        "}"
        "#classToolsList::item:selected, #classContestList::item:selected {"
        "  background: #dcefea;"
        "  color: #12343b;"
        "}"
        "#classToolsList::item:hover, #classContestList::item:hover {"
        "  background: #eef4ef;"
        "}"
    );

    connect(backButton, &QPushButton::clicked, this, &ClassPage::backRequested);
    connect(
        m_toolsListWidget,
        &QListWidget::itemClicked,
        this,
        [this](QListWidgetItem *item) {
            if (item->data(Qt::UserRole).toString() == "back") {
                emit backRequested();
            }
        });
    connect(
        m_contestListWidget,
        &QListWidget::itemClicked,
        this,
        [this](QListWidgetItem *item) {
            emit contestSelected(
                item->text(),
                item->data(Qt::UserRole).toString());
        });
}

void ClassPage::openClass(const QString &name, const QString &url)
{
    m_className = name;
    m_classUrl = url;
    m_titleLabel->setText(name.isEmpty() ? QString("Course") : name);
    m_contestListWidget->clear();
}

void ClassPage::showLoadingContests()
{
}

void ClassPage::showClassLoadFailed(const QString &message)
{
    m_contestListWidget->clear();
    auto *item = new QListWidgetItem(message, m_contestListWidget);
    item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
}

void ClassPage::showContestSets(const GroupPageInfo &groupPageInfo)
{
    m_contestListWidget->clear();
    for (const ContestSetInfo &contestSet : groupPageInfo.contestSets) {
        QString text = contestSet.title;
        if (!contestSet.extraText.isEmpty()) {
            text += " | " + contestSet.extraText;
        }
        auto *item = new QListWidgetItem(text, m_contestListWidget);
        item->setData(Qt::UserRole, contestSet.url);
    }
}
