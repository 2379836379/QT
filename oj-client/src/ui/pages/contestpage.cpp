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

    auto *leftFrame = new QFrame(this);
    leftFrame->setObjectName("contestLeftFrame");
    auto *leftLayout = new QVBoxLayout(leftFrame);
    leftLayout->setContentsMargins(20, 18, 20, 18);
    leftLayout->setSpacing(12);

    auto *toolsLabel = new QLabel("Tools", leftFrame);
    toolsLabel->setObjectName("contestSectionLabel");

    m_toolsListWidget = new QListWidget(leftFrame);
    m_toolsListWidget->setObjectName("contestToolsList");
    auto *backItem = new QListWidgetItem("Back", m_toolsListWidget);
    backItem->setData(Qt::UserRole, "back");

    leftLayout->addWidget(toolsLabel);
    leftLayout->addWidget(m_toolsListWidget);
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

    bottomLayout->addWidget(leftFrame, 1);
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
        "#contestToolsList, #contestProblemList {"
        "  background: transparent;"
        "  border: none;"
        "  border-radius: 0px;"
        "  padding: 0px;"
        "  outline: none;"
        "}"
        "#contestToolsList::item, #contestProblemList::item {"
        "  padding: 12px 4px;"
        "  border-radius: 8px;"
        "  margin: 2px 0px;"
        "}"
        "#contestToolsList::item:selected, #contestProblemList::item:selected {"
        "  background: #dcefea;"
        "  color: #12343b;"
        "}"
        "#contestToolsList::item:hover, #contestProblemList::item:hover {"
        "  background: #eef4ef;"
        "}"
    );

    connect(backButton, &QPushButton::clicked, this, &ContestPage::backRequested);
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
        m_problemListWidget,
        &QListWidget::itemClicked,
        this,
        [this](QListWidgetItem *item) {
            emit problemSelected(
                item->text(),
                item->data(Qt::UserRole).toString());
        });
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
