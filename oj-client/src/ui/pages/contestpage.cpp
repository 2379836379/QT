#include "ui/pages/contestpage.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QSizePolicy>
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

    m_titleLabel = new QLabel("Contest Set", topFrame);
    m_titleLabel->setObjectName("contestTitleLabel");

    topLayout->addWidget(m_titleLabel, 1);
    auto *homeButton = new QPushButton("Home", topFrame);
    homeButton->setObjectName("contestRefreshButton");
    topLayout->addWidget(homeButton, 0, Qt::AlignRight);
    auto *themeButton = new QPushButton("Dark Mode", topFrame);
    themeButton->setObjectName("contestRefreshButton");
    topLayout->addWidget(themeButton, 0, Qt::AlignRight);
    auto *refreshButton = new QPushButton("Refresh", topFrame);
    refreshButton->setObjectName("contestRefreshButton");
    topLayout->addWidget(refreshButton, 0, Qt::AlignRight);

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
        "#contestRefreshButton {"
        "  min-width: 88px;"
        "  padding: 8px 14px;"
        "  border: 1px solid #cdd7cf;"
        "  border-radius: 10px;"
        "  background: #f7f5ef;"
        "  color: #243029;"
        "}"
        "#contestRefreshButton:hover {"
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
        "#contestProblemTitleLabel {"
        "  color: #1f2328;"
        "}"
        "#contestProblemStatusLabel {"
        "  color: #5e675f;"
        "  font-weight: 600;"
        "}"
    );

    setToolsExpanded(true);

    connect(homeButton, &QPushButton::clicked, this, &ContestPage::homeRequested);
    connect(themeButton, &QPushButton::clicked, this, [this]() {
        emit themeToggleRequested(!m_darkMode);
    });
    connect(refreshButton, &QPushButton::clicked, this, &ContestPage::refreshRequested);
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
    m_titleLabel->setText(m_contestTitle.isEmpty() ? QString("Contest Set") : m_contestTitle);
    for (const ContestProblemInfo &problem : contestPageInfo.problems) {
        const QString text = QString("%1 %2").arg(problem.problemId, problem.title);
        auto *item = new QListWidgetItem(text, m_problemListWidget);
        item->setData(Qt::UserRole, problem.problemUrl);

        auto *rowWidget = new QWidget(m_problemListWidget);
        auto *rowLayout = new QHBoxLayout(rowWidget);
        rowLayout->setContentsMargins(0, 0, 0, 0);
        rowLayout->setSpacing(12);

        auto *titleLabel = new QLabel(text, rowWidget);
        titleLabel->setObjectName("contestProblemTitleLabel");
        titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

        rowLayout->addWidget(titleLabel, 1);
        rowLayout->addStretch();

        if (problem.solved) {
            auto *statusLabel = new QLabel("finished", rowWidget);
            statusLabel->setObjectName("contestProblemStatusLabel");
            statusLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
            rowLayout->addWidget(statusLabel, 0, Qt::AlignRight);
        }

        item->setSizeHint(rowWidget->sizeHint());
        m_problemListWidget->setItemWidget(item, rowWidget);
    }
}

void ContestPage::setDarkMode(bool dark)
{
    m_darkMode = dark;
    QString lightStyle = property("_lightStyleSheet").toString();
    if (lightStyle.isEmpty()) {
        lightStyle = styleSheet();
        setProperty("_lightStyleSheet", lightStyle);
    }

    const QString darkOverride =
        "ContestPage { background: #000000; }"
        "#contestTopFrame, #contestLeftFrame, #contestContentFrame {"
        "  background: #1b232c;"
        "  border: 1px solid #2c3844;"
        "}"
        "#contestTitleLabel, #contestSectionLabel, #contestToolsToggleButton, #contestToolButton, #contestToolIconButton {"
        "  color: #d9e1e8;"
        "}"
        "#contestRefreshButton {"
        "  border: 1px solid #3a4652;"
        "  background: #202a34;"
        "  color: #e8edf2;"
        "}"
        "#contestRefreshButton:hover, #contestToolButton:hover, #contestToolIconButton:hover {"
        "  background: #26313c;"
        "}"
        "#contestProblemList { color: #e8edf2; }"
        "#contestProblemList::item:selected {"
        "  background: #234257;"
        "  color: #eff8ff;"
        "}"
        "#contestProblemList::item:hover {"
        "  background: #26313c;"
        "}"
        "#contestProblemTitleLabel { color: #e8edf2; }"
        "#contestProblemStatusLabel {"
        "  color: #9fc4a7;"
        "  font-weight: 600;"
        "}";

    setStyleSheet(dark ? lightStyle + darkOverride : lightStyle);
}
