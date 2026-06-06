#include "ui/pages/statspage.h"
#include "ui/lightmodeiconhelper.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QVBoxLayout>

#include <algorithm>

namespace
{
constexpr int ProblemUrlRole = Qt::UserRole;
constexpr int ProblemTitleRole = Qt::UserRole + 1;

struct StatusInfo
{
    QString key;
    QString label;
};

const QList<StatusInfo> &statusInfos()
{
    static const QList<StatusInfo> infos = {
        {"todo", "未开始"},
        {"doing", "进行中"},
        {"done", "已完成"},
        {"redo", "待重做"}};
    return infos;
}
}

StatsPage::StatsPage(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(18);

    auto *topFrame = new QFrame(this);
    topFrame->setObjectName("statsTopFrame");
    topFrame->setFixedHeight(80);
    auto *topLayout = new QHBoxLayout(topFrame);
    topLayout->setContentsMargins(24, 18, 24, 18);
    topLayout->setSpacing(16);

    auto *titleLabel = new QLabel("Statistics", topFrame);
    titleLabel->setObjectName("statsTitleLabel");
    auto *homeButton = new QPushButton("Home", topFrame);
    homeButton->setObjectName("statsTopActionButton");
    auto *themeButton = new QPushButton("Dark Mode", topFrame);
    themeButton->setObjectName("statsTopActionButton");
    auto *refreshButton = new QPushButton("Refresh", topFrame);
    refreshButton->setObjectName("statsTopActionButton");

    topLayout->addWidget(titleLabel, 1);
    topLayout->addWidget(homeButton);
    topLayout->addWidget(themeButton);
    topLayout->addWidget(refreshButton);

    auto *bottomLayout = new QHBoxLayout();
    bottomLayout->setSpacing(18);

    auto *summaryFrame = new QFrame(this);
    summaryFrame->setObjectName("statsSummaryFrame");
    auto *summaryOuter = new QVBoxLayout(summaryFrame);
    summaryOuter->setContentsMargins(20, 18, 20, 18);
    summaryOuter->setSpacing(12);
    auto *summaryTitle = new QLabel("Overview", summaryFrame);
    summaryTitle->setObjectName("statsSectionLabel");
    m_summaryLayout = new QVBoxLayout();
    m_summaryLayout->setContentsMargins(0, 0, 0, 0);
    m_summaryLayout->setSpacing(8);
    summaryOuter->addWidget(summaryTitle);
    summaryOuter->addLayout(m_summaryLayout);
    summaryOuter->addStretch();

    auto *reviewFrame = new QFrame(this);
    reviewFrame->setObjectName("statsReviewFrame");
    auto *reviewOuter = new QVBoxLayout(reviewFrame);
    reviewOuter->setContentsMargins(20, 18, 20, 18);
    reviewOuter->setSpacing(12);
    m_reviewLabel = new QLabel("错题本", reviewFrame);
    m_reviewLabel->setObjectName("statsSectionLabel");
    m_reviewList = new QListWidget(reviewFrame);
    m_reviewList->setObjectName("statsReviewList");
    m_reviewList->setMinimumHeight(360);
    reviewOuter->addWidget(m_reviewLabel);
    reviewOuter->addWidget(m_reviewList, 1);

    bottomLayout->addWidget(summaryFrame, 1);
    bottomLayout->addWidget(reviewFrame, 1);

    layout->addWidget(topFrame);
    layout->addLayout(bottomLayout, 1);

    homeButton->setToolTip("Home");
    themeButton->setToolTip("Dark Mode");
    refreshButton->setToolTip("Refresh");
    LightModeIconHelper::applyIcon(homeButton, "homepage.svg");
    LightModeIconHelper::applyIcon(themeButton, "dark-mode.png");
    LightModeIconHelper::applyIcon(refreshButton, "refresh.svg");

    setStyleSheet(
        "StatsPage { background: #f3f1eb; }"
        "#statsTopFrame, #statsSummaryFrame, #statsReviewFrame {"
        "  background: #fbfaf7;"
        "  border: 1px solid #ded8cc;"
        "  border-radius: 16px;"
        "}"
        "#statsTitleLabel {"
        "  font-size: 28px;"
        "  font-weight: 600;"
        "  color: #1f2328;"
        "}"
        "#statsSectionLabel {"
        "  font-size: 16px;"
        "  font-weight: 600;"
        "  color: #2f3a33;"
        "}"
        "#statsTopActionButton {"
        "  min-width: 36px;"
        "  padding: 6px;"
        "  border: none;"
        "  background: transparent;"
        "  color: #2f3a33;"
        "}"
        "#statRowLabel { color: #2f3a33; }"
        "#statRowValue { color: #5e675f; }"
        "#statBarHolder { background: transparent; }"
        "#statBarFill { background: #7fb8a6; border-radius: 4px; }"
        "#statBarRest { background: #e6e1d7; border-radius: 4px; }"
        "#statsReviewList {"
        "  background: transparent;"
        "  border: none;"
        "  outline: none;"
        "  color: #1f2328;"
        "}"
        "#statsReviewList::item {"
        "  padding: 10px 4px;"
        "  border-radius: 8px;"
        "  margin: 2px 0px;"
        "}"
        "#statsReviewList::item:selected { background: #dcefea; color: #12343b; }"
        "#statsReviewList::item:hover { background: #eef4ef; }");

    connect(homeButton, &QPushButton::clicked, this, &StatsPage::homeRequested);
    connect(refreshButton, &QPushButton::clicked, this, &StatsPage::refreshRequested);
    connect(themeButton, &QPushButton::clicked, this, [this]() {
        emit themeToggleRequested(!m_darkMode);
    });
    connect(m_reviewList, &QListWidget::itemClicked, this, [this](QListWidgetItem *item) {
        if (item == nullptr) {
            return;
        }
        emit problemSelected(item->data(ProblemTitleRole).toString(),
                             item->data(ProblemUrlRole).toString());
    });
}

void StatsPage::clearLayout(QVBoxLayout *layout)
{
    if (layout == nullptr) {
        return;
    }
    while (QLayoutItem *item = layout->takeAt(0)) {
        if (QWidget *widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }
}

void StatsPage::addStatRow(QVBoxLayout *layout,
                           const QString &label,
                           int value,
                           int maxValue)
{
    auto *row = new QWidget();
    auto *rowLayout = new QHBoxLayout(row);
    rowLayout->setContentsMargins(0, 0, 0, 0);
    rowLayout->setSpacing(10);

    auto *name = new QLabel(label, row);
    name->setObjectName("statRowLabel");
    name->setFixedWidth(150);

    auto *barHolder = new QFrame(row);
    barHolder->setObjectName("statBarHolder");
    barHolder->setFixedHeight(16);
    auto *barLayout = new QHBoxLayout(barHolder);
    barLayout->setContentsMargins(0, 0, 0, 0);
    barLayout->setSpacing(0);
    auto *fill = new QFrame(barHolder);
    fill->setObjectName("statBarFill");
    auto *rest = new QFrame(barHolder);
    rest->setObjectName("statBarRest");
    barLayout->addWidget(fill);
    barLayout->addWidget(rest);

    const int clampedValue = qMax(0, value);
    const int effectiveMax = qMax(maxValue, 1);
    barLayout->setStretch(0, clampedValue);
    barLayout->setStretch(1, qMax(0, effectiveMax - clampedValue));
    if (clampedValue == 0) {
        fill->hide();
    }

    auto *valueLabel = new QLabel(QString::number(value), row);
    valueLabel->setObjectName("statRowValue");
    valueLabel->setFixedWidth(48);

    rowLayout->addWidget(name);
    rowLayout->addWidget(barHolder, 1);
    rowLayout->addWidget(valueLabel);
    layout->addWidget(row);
}

void StatsPage::showStats(const QHash<QString, int> &statusCounts,
                          const QHash<QString, int> &tagCounts,
                          int notesCount,
                          const QList<FavoriteFolderInfo> &folders,
                          const QList<ProblemMeta> &reviewProblems)
{
    clearLayout(m_summaryLayout);

    auto *statusHeader = new QLabel("任务状态");
    statusHeader->setObjectName("statRowLabel");
    m_summaryLayout->addWidget(statusHeader);

    int maxStatus = 1;
    for (const StatusInfo &info : statusInfos()) {
        maxStatus = qMax(maxStatus, statusCounts.value(info.key, 0));
    }
    for (const StatusInfo &info : statusInfos()) {
        addStatRow(m_summaryLayout, info.label, statusCounts.value(info.key, 0), maxStatus);
    }

    auto *tagHeader = new QLabel("标签分布 (Top 8)");
    tagHeader->setObjectName("statRowLabel");
    m_summaryLayout->addWidget(tagHeader);

    QList<QPair<QString, int>> sortedTags;
    for (auto it = tagCounts.constBegin(); it != tagCounts.constEnd(); ++it) {
        sortedTags.append(qMakePair(it.key(), it.value()));
    }
    std::sort(sortedTags.begin(), sortedTags.end(),
              [](const QPair<QString, int> &lhs, const QPair<QString, int> &rhs) {
                  if (lhs.second != rhs.second) {
                      return lhs.second > rhs.second;
                  }
                  return lhs.first < rhs.first;
              });
    if (sortedTags.isEmpty()) {
        auto *empty = new QLabel("暂无标签");
        empty->setObjectName("statRowValue");
        m_summaryLayout->addWidget(empty);
    } else {
        const int maxTag = sortedTags.first().second;
        const int limit = qMin(8, static_cast<int>(sortedTags.size()));
        for (int i = 0; i < limit; ++i) {
            addStatRow(m_summaryLayout, sortedTags.at(i).first, sortedTags.at(i).second, maxTag);
        }
    }

    int favoriteTotal = 0;
    for (const FavoriteFolderInfo &folder : folders) {
        favoriteTotal += folder.problemCount;
    }

    auto *otherHeader = new QLabel("其它");
    otherHeader->setObjectName("statRowLabel");
    m_summaryLayout->addWidget(otherHeader);
    const int otherMax = qMax(1, qMax(notesCount, favoriteTotal));
    addStatRow(m_summaryLayout, "笔记数量", notesCount, otherMax);
    addStatRow(m_summaryLayout, "收藏题数", favoriteTotal, otherMax);

    m_reviewList->clear();
    for (const ProblemMeta &meta : reviewProblems) {
        const QString displayText = meta.title.isEmpty() ? meta.problemUrl : meta.title;
        auto *item = new QListWidgetItem(displayText, m_reviewList);
        item->setData(ProblemUrlRole, meta.problemUrl);
        item->setData(ProblemTitleRole, meta.title);
        if (!meta.tags.isEmpty()) {
            item->setToolTip(meta.tags.join(", "));
        }
    }
    m_reviewLabel->setText(QString("错题本 (%1)").arg(reviewProblems.size()));
}

void StatsPage::setDarkMode(bool dark)
{
    m_darkMode = dark;
    QString lightStyle = property("_lightStyleSheet").toString();
    if (lightStyle.isEmpty()) {
        lightStyle = styleSheet();
        setProperty("_lightStyleSheet", lightStyle);
    }

    const QString darkOverride =
        "StatsPage { background: #000000; }"
        "#statsTopFrame, #statsSummaryFrame, #statsReviewFrame {"
        "  background: #1b232c;"
        "  border: 1px solid #2c3844;"
        "}"
        "#statsTitleLabel, #statsSectionLabel { color: #d9e1e8; }"
        "#statsTopActionButton {"
        "  border: none;"
        "  background: transparent;"
        "  color: #e8edf2;"
        "}"
        "#statsTopActionButton:hover { background: #26313c; }"
        "#statRowLabel { color: #d9e1e8; }"
        "#statRowValue { color: #9ba8b6; }"
        "#statBarFill { background: #3d8d77; }"
        "#statBarRest { background: #2c3844; }"
        "#statsReviewList { color: #e8edf2; }"
        "#statsReviewList::item:selected { background: #234257; color: #eff8ff; }"
        "#statsReviewList::item:hover { background: #26313c; }";

    setStyleSheet(dark ? lightStyle + darkOverride : lightStyle);
}
