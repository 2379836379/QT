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

    m_toolsFrame = new QFrame(this);
    m_toolsFrame->setObjectName("classLeftFrame");
    auto *leftLayout = new QVBoxLayout(m_toolsFrame);
    leftLayout->setContentsMargins(20, 18, 20, 18);
    leftLayout->setSpacing(12);

    m_toolsToggleButton = new QPushButton(m_toolsFrame);
    m_toolsToggleButton->setObjectName("classToolsToggleButton");

    m_toolsPanel = new QWidget(m_toolsFrame);
    auto *toolsPanelLayout = new QVBoxLayout(m_toolsPanel);
    toolsPanelLayout->setContentsMargins(0, 0, 0, 0);
    toolsPanelLayout->setSpacing(10);

    m_backToolButton = new QPushButton("Back", m_toolsPanel);
    m_backToolButton->setObjectName("classToolButton");
    toolsPanelLayout->addWidget(m_backToolButton);
    toolsPanelLayout->addStretch();

    m_collapsedToolsPanel = new QWidget(m_toolsFrame);
    m_collapsedToolsPanel->setObjectName("classCollapsedToolsPanel");
    auto *collapsedLayout = new QVBoxLayout(m_collapsedToolsPanel);
    collapsedLayout->setContentsMargins(0, 0, 0, 0);
    collapsedLayout->setSpacing(10);

    m_collapsedBackButton = new QPushButton("B", m_collapsedToolsPanel);
    m_collapsedBackButton->setObjectName("classToolIconButton");
    m_collapsedBackButton->setToolTip("Back");
    collapsedLayout->addWidget(m_collapsedBackButton);
    collapsedLayout->addStretch();

    leftLayout->addWidget(m_toolsToggleButton);
    leftLayout->addWidget(m_toolsPanel);
    leftLayout->addWidget(m_collapsedToolsPanel);
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

    bottomLayout->addWidget(m_toolsFrame, 1);
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
        "#classToolsToggleButton {"
        "  background: transparent;"
        "  border: none;"
        "  padding: 0px;"
        "  text-align: left;"
        "  font-size: 16px;"
        "  font-weight: 600;"
        "  color: #2f3a33;"
        "}"
        "#classToolsToggleButton:hover {"
        "  color: #12343b;"
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
        "#classToolButton {"
        "  padding: 10px 12px;"
        "  border: none;"
        "  border-radius: 10px;"
        "  background: transparent;"
        "  color: #2f3a33;"
        "  text-align: left;"
        "}"
        "#classToolButton:hover, #classToolIconButton:hover {"
        "  background: #eef4ef;"
        "}"
        "#classToolIconButton {"
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
        "#classContestList {"
        "  background: transparent;"
        "  border: none;"
        "  border-radius: 0px;"
        "  padding: 0px;"
        "  outline: none;"
        "}"
        "#classContestList::item {"
        "  padding: 12px 4px;"
        "  border-radius: 8px;"
        "  margin: 2px 0px;"
        "}"
        "#classContestList::item:selected {"
        "  background: #dcefea;"
        "  color: #12343b;"
        "}"
        "#classContestList::item:hover {"
        "  background: #eef4ef;"
        "}"
    );

    setToolsExpanded(true);

    connect(backButton, &QPushButton::clicked, this, &ClassPage::backRequested);
    connect(m_backToolButton, &QPushButton::clicked, this, &ClassPage::backRequested);
    connect(m_collapsedBackButton, &QPushButton::clicked, this, &ClassPage::backRequested);
    connect(
        m_toolsToggleButton,
        &QPushButton::clicked,
        this,
        [this]() {
            setToolsExpanded(!m_toolsExpanded);
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

void ClassPage::setToolsExpanded(bool expanded)
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
    if (m_collapsedBackButton != nullptr) {
        m_collapsedBackButton->setVisible(!expanded);
    }
    if (m_toolsToggleButton != nullptr) {
        m_toolsToggleButton->setText(expanded ? "Tools v" : ">");
    }
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
