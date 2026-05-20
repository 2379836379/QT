#include "ui/pages/contestpage.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QVBoxLayout>

ContestPage::ContestPage(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(18);

    auto *topFrame = new QFrame(this);
    topFrame->setObjectName("contestTopFrame");
    topFrame->setFixedHeight(80);
    auto *topLayout = new QHBoxLayout(topFrame);
    topLayout->setContentsMargins(24, 18, 24, 18);
    topLayout->setSpacing(16);

    auto *backButton = new QPushButton("Back", topFrame);
    backButton->setObjectName("contestBackButton");
    m_titleLabel = new QLabel("Contest Set", topFrame);
    m_titleLabel->setObjectName("contestTitleLabel");

    topLayout->addWidget(backButton);
    topLayout->addWidget(m_titleLabel, 1);

    auto *bottomLayout = new QHBoxLayout();
    bottomLayout->setSpacing(18);

    m_toolsFrame = new QFrame(this);
    m_toolsFrame->setObjectName("contestLeftFrame");
    auto *leftLayout = new QVBoxLayout(m_toolsFrame);
    leftLayout->setContentsMargins(20, 18, 20, 18);
    leftLayout->setSpacing(12);

    m_toolsToggleButton = new QPushButton(m_toolsFrame);
    m_toolsToggleButton->setObjectName("contestToolsToggleButton");

    m_toolsPanel = new QWidget(m_toolsFrame);
    auto *toolsPanelLayout = new QVBoxLayout(m_toolsPanel);
    toolsPanelLayout->setContentsMargins(0, 0, 0, 0);
    toolsPanelLayout->setSpacing(10);

    m_backToolButton = new QPushButton("Back", m_toolsPanel);
    m_backToolButton->setObjectName("contestToolButton");
    toolsPanelLayout->addWidget(m_backToolButton);
    toolsPanelLayout->addStretch();

    m_collapsedToolsPanel = new QWidget(m_toolsFrame);
    m_collapsedToolsPanel->setObjectName("contestCollapsedToolsPanel");
    auto *collapsedLayout = new QVBoxLayout(m_collapsedToolsPanel);
    collapsedLayout->setContentsMargins(0, 0, 0, 0);
    collapsedLayout->setSpacing(10);

    m_collapsedBackButton = new QPushButton("B", m_collapsedToolsPanel);
    m_collapsedBackButton->setObjectName("contestToolIconButton");
    m_collapsedBackButton->setToolTip("Back");
    collapsedLayout->addWidget(m_collapsedBackButton);
    collapsedLayout->addStretch();

    leftLayout->addWidget(m_toolsToggleButton);
    leftLayout->addWidget(m_toolsPanel);
    leftLayout->addWidget(m_collapsedToolsPanel);
    leftLayout->addStretch();

    auto *contentFrame = new QFrame(this);
    contentFrame->setObjectName("contestContentFrame");
    auto *contentLayout = new QVBoxLayout(contentFrame);
    contentLayout->setContentsMargins(20, 18, 20, 18);
    contentLayout->setSpacing(14);

    auto *problemLabel = new QLabel("Problem List", contentFrame);
    problemLabel->setObjectName("contestSectionLabel");

    m_problemListWidget = new QListWidget(contentFrame);
    m_problemListWidget->setObjectName("contestProblemList");
    m_problemListWidget->setMinimumHeight(420);

    contentLayout->addWidget(problemLabel);
    contentLayout->addWidget(m_problemListWidget, 1);

    bottomLayout->addWidget(m_toolsFrame, 1);
    bottomLayout->addWidget(contentFrame, 4);

    layout->addWidget(topFrame);
    layout->addLayout(bottomLayout, 1);

    setStyleSheet(
        "ContestPage { background: #f3f1eb; }"
        "#contestTopFrame, #contestLeftFrame, #contestContentFrame {"
        "  background: #fbfaf7;"
        "  border: 1px solid #ded8cc;"
        "  border-radius: 16px;"
        "}"
        "#contestTitleLabel {"
        "  font-size: 28px;"
        "  font-weight: 600;"
        "  color: #1f2328;"
        "}"
        "#contestSectionLabel {"
        "  font-size: 16px;"
        "  font-weight: 600;"
        "  color: #2f3a33;"
        "}"
        "#contestToolsToggleButton {"
        "  background: transparent;"
        "  border: none;"
        "  padding: 0px;"
        "  text-align: left;"
        "  font-size: 16px;"
        "  font-weight: 600;"
        "  color: #2f3a33;"
        "}"
        "#contestToolsToggleButton:hover {"
        "  color: #12343b;"
        "}"
        "#contestBackButton {"
        "  min-width: 88px;"
        "  padding: 8px 14px;"
        "  border: 1px solid #cdd7cf;"
        "  border-radius: 10px;"
        "  background: #f7f5ef;"
        "  color: #243029;"
        "}"
        "#contestBackButton:hover {"
        "  background: #eef4ef;"
        "}"
        "#contestToolButton {"
        "  padding: 10px 12px;"
        "  border: none;"
        "  border-radius: 10px;"
        "  background: transparent;"
        "  color: #2f3a33;"
        "  text-align: left;"
        "}"
        "#contestToolButton:hover, #contestToolIconButton:hover {"
        "  background: #eef4ef;"
        "}"
        "#contestToolIconButton {"
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
        "#contestProblemList {"
        "  background: transparent;"
        "  border: none;"
        "  border-radius: 0px;"
        "  padding: 0px;"
        "  outline: none;"
        "}"
        "#contestProblemList::item {"
        "  padding: 12px 4px;"
        "  border-radius: 8px;"
        "  margin: 2px 0px;"
        "}"
        "#contestProblemList::item:selected {"
        "  background: #dcefea;"
        "  color: #12343b;"
        "}"
        "#contestProblemList::item:hover {"
        "  background: #eef4ef;"
        "}"
    );

    setToolsExpanded(true);

    connect(backButton, &QPushButton::clicked, this, &ContestPage::backRequested);
    connect(m_backToolButton, &QPushButton::clicked, this, &ContestPage::backRequested);
    connect(m_collapsedBackButton, &QPushButton::clicked, this, &ContestPage::backRequested);
    connect(
        m_toolsToggleButton,
        &QPushButton::clicked,
        this,
        [this]() {
            setToolsExpanded(!m_toolsExpanded);
        });
    connect(
        m_problemListWidget,
        &QListWidget::itemClicked,
        this,
        [this](QListWidgetItem *item) {
            emit problemSelected(
                item->text(),
                item->data(Qt::UserRole).toString());
        });
}

void ContestPage::setToolsExpanded(bool expanded)
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

void ContestPage::openContest(const QString &title, const QString &url)
{
    m_contestTitle = title;
    m_contestUrl = url;
    m_titleLabel->setText(title.isEmpty() ? QString("Contest Set") : title);
    m_problemListWidget->clear();
}

void ContestPage::showLoadingProblems()
{
}

void ContestPage::showContestLoadFailed(const QString &message)
{
    m_problemListWidget->clear();
    auto *item = new QListWidgetItem(message, m_problemListWidget);
    item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
}

void ContestPage::showProblems(const ContestPageInfo &contestPageInfo)
{
    m_problemListWidget->clear();
    for (const ContestProblemInfo &problem : contestPageInfo.problems) {
        const QString text = QString("%1 %2").arg(problem.problemId, problem.title);
        auto *item = new QListWidgetItem(text, m_problemListWidget);
        item->setData(Qt::UserRole, problem.problemUrl);
    }
}
