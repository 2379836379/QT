# DEVLOG

## 2026-06-25

### A1: base URL 规整（openjudgeclient）

- 背景：`m_baseUrl` / `m_judgerBaseUrl` 在多处以 `base + "/path"` 形式拼接，
  当配置（`appstate.toml` 的 `openjudge_base_url` / `judger_base_url`）带结尾 `/` 时会产生 `//path`。
- 改动：`oj-client/src/network/openjudgeclient.cpp` 匿名 namespace 新增 `normalizeBaseUrl()`
  （`trimmed()` + 去掉结尾所有 `/`）；`setBaseUrl` / `setJudgerBaseUrl` 改为存入规整值，
  `setJudgerBaseUrl` 保留"空则不覆盖"语义。
- 影响范围：仅本文件，调用方与默认值不变。
- 邮件验证服务用 `QUrl::resolved("/auth/...")`（绝对路径整体替换 path），不受尾斜杠影响，未改动。
- 验证：本机无 Qt/CMake 工具链，未做编译验证；逻辑为纯字符串规整。

### B1 + B2a: 看板拖拽 + 看板筛选排序

- 文件：`oj-client/src/ui/pages/taskboardpage.h` / `taskboardpage.cpp`。
- B1 拖拽：新增子类 `StatusListWidget`（继承 `QListWidget`，带 `status()` 与 `itemDropped` 信号，
  重写 `dropEvent`）。四列开启 drag/drop；drop 时 `emit itemDropped(url, targetStatus)`，
  并把 dropAction 设为 `IgnoreAction` 阻止 Qt 物理搬运 item——状态持久化与界面刷新统一走
  `statusChangeRequested → mainwindow 保存 → showTasks 重绘`，避免与 Qt 自带搬运冲突导致重复/乱序。
  右键菜单移动状态保留，两种方式并存。
- B2a 筛选排序：顶部加 标签 / 难度 / 排序 三个 `QComboBox`。`showTasks` 改为缓存 `m_allMeta`，
  `rebuildTagFilterOptions()` 动态重建标签项（用 `QSignalBlocker` 防递归），`renderTasks()`
  按筛选条件过滤 + 排序（最近更新/难度高→低/标题 localeAware）后填充四列。纯前端过滤，不改 SQL。
- 子类 `StatusListWidget` 含 `Q_OBJECT` 且声明在头文件，依赖 CMake AUTOMOC 处理（头文件已在 sources）。
- 验证：本机无 Qt/CMake，未编译验证。拖拽行为需在有 Qt 环境处实测确认。

### B2b: 错题本筛选排序（statspage）

- 文件：`oj-client/src/ui/pages/statspage.h` / `statspage.cpp`。
- 错题本区顶部加 标签 / 难度 两个 `QComboBox`；`showStats` 缓存 `m_reviewProblems`，
  `rebuildReviewTagOptions()` 动态重建标签项（`QSignalBlocker` 防递归），`renderReview()`
  按筛选渲染列表，标题显示为 `错题本 (显示数/总数)`。纯前端过滤。
- 验证：本机无 Qt/CMake，未编译验证。

### C: 间隔重复复习（SRS）

- 数据层 `repository/meta/problemmetarepository.*`：
  - `ProblemMeta` 加 `reviewInterval/reviewEase/nextReviewAt/lastReviewedAt/reviewCount`。
  - `ensureReviewColumns()`：用 `PRAGMA table_info` 检测后幂等 `ALTER TABLE ADD COLUMN`，对新旧库都适用。
  - `loadMeta/loadAllMeta/reviewProblems/dueReviewProblems` 的 SELECT 与 `readMeta` 同步读 13 列。
  - 新增 `dueReviewProblems(now)/dueReviewCount(now)/updateReviewSchedule(...)/ensureReviewScheduled(url,now)`。
    `updateReviewSchedule` 故意不改 `updated_at`，避免评分把题目顶到看板/统计列表最前。
  - 到期判定用 `Qt::ISODate` 本地时间字符串比较（同格式无时区，字典序=时间序）。
- 服务层 `service/meta/problemmetaservice.*`：
  - `gradeReview(url, grade)`：内部 `loadMeta` → 简化 SM-2（0=重来/1=困难/2=良好/3=简单，整数运算避免浮点）
    → `updateReviewSchedule`。`saveMeta` 末尾对 `reviewFlag` 题目调用 `ensureReviewScheduled` 做首次排程
    （best-effort，失败不阻断保存）。
- 新增页面 `ui/pages/reviewpage.*`：左到期列表 + 右详情（标题/难度/标签/笔记只读）+ 四个评分按钮 + 打开题目。
  评分 → `gradeRequested` → mainwindow 调 `gradeReview` 并重载 `showDue`。
- 入口 `homepage.*`：新增 `reviewRequested` 信号与 Review 按钮（展开/折叠），并入折叠可见性列表。
- 接线 `mainwindow.*`：新增 `m_reviewPage` 成员/创建/入栈/深色模式；首页 `reviewRequested` → `showDue(dueReviewProblems())`；
  复习页 home/theme/refresh/problemSelected/gradeRequested 全部接好。
- `CMakeLists.txt` 加入 `reviewpage.cpp/.h`。
- 与 PLAN 的偏差：PLAN 原计划首页显示"今日待复习 N"实时计数，本次先只放入口按钮，计数显示在复习页
  状态栏（`今日待复习 N 题`）。`dueReviewCount()` 已实现备用，后续若要首页实时计数再接线。
- 验证：本机无 Qt/CMake，未编译验证。SRS 排程/评分/到期过滤需在有 Qt 环境实测。

### A2: 数据落用户目录（DB / 缓存 / toml / 日志）

- 背景：旧实现把库、缓存、`config.toml`/`appstate.toml`、`startup.log` 都写在"项目/安装目录"
  （各处 `projectRootDir()` = exe 目录向上找 build）。Windows 装到 `Program Files` 时该目录只读，
  写库/写日志会失败（README 已知限制）。
- 新增 `src/config/apppaths.*`（已加入 `CMakeLists.txt`）：
  - `baseDir()` = `QStandardPaths::GenericDataLocation` + `/oj-client`；
    用 Generic 而非 `AppDataLocation` 是因为 `main.cpp` 故意把 `applicationName` 置为 `" "`，
    `AppDataLocation` 会拼出异常目录名，Generic + 固定 `oj-client` 与 appName 解耦更稳。
  - `dataDir()`=baseDir/`data`、`cacheDir()`=baseDir/`cache`、`configFilePath()`、`appStateFilePath()`，均自动 `mkpath`。
  - `migrateLegacyDataIfNeeded()`：首启一次性迁移（仅在目标缺失时拷贝，幂等）：
    - 关键库 `favorites.db`/`problemmeta.db`/`login_cache.db` 从旧 `data/` 拷到新 `dataDir`；缓存库不迁。
    - `config.toml`/`appstate.toml` 按旧候选路径（cwd、exe 目录及上级）查找后拷到 `baseDir`。
    - `writeDefaultAppStateIfMissing()`：若仍无 appstate 且安装目录存在内置铃声
      （`{app}/vedio/哈基米起床 - MyRingtone.mp3`），生成默认 `ring_path`（接替原 installer 行为）。
  - 在 `main.cpp` 于 `QApplication` 构造后、`MainWindow` 之前调用，确保早于任何库/toml 访问。
- 路径改造（统一改用 AppPaths，删除各文件本地 `projectRootDir()`）：
  - DB 仓库：`problemmetarepository`/`logincacherepository`/`favoriteproblemrepository` → `dataDir`。
  - 缓存仓库：`home/class/contest/problemcacherepository` → `cacheDir`（`cacheSizeBytes` 随 `databasePath` 自动对齐）。
  - 日志：8 个文件 9 处写 `data/startup.log` 的函数（`main`/`mainwindow`(含 clear)/`openjudgeclient`/`problempage`/
    `submitservice`/`submitrepository`/`aiservice`/`openaiclient`/`treesittersyntaxhighlighter`）改为 `dataDir/startup.log`。
  - `appconfig.cpp`：`candidateConfigPaths/configOutputPath/appStateOutputPath` 直接返回 `AppPaths` 路径
    （迁移已把旧 toml 搬到 baseDir，读写单一来源，去掉原多候选搜索）。
  - `applicationsizeservice.cpp`：总占用 = 安装目录 + `baseDir`（数据/缓存已外移，原口径会漏算）。
- `installer.iss`：删除 `[Code]` 段（不再由安装器写 `appstate.toml`），改由应用首启在用户目录生成；
  铃声仍随安装包放到 `{app}\vedio`。`[UninstallDelete]` 中 `{app}\data`/`{app}\cache`/`*.toml` 保留（无害）。
- fallback 说明（按约定记录）：`migrateLegacyDataIfNeeded` 与 `writeDefaultAppStateIfMissing` 属一次性迁移/兜底，
  均"仅在目标缺失时"执行，不覆盖用户现有数据；适用边界=旧版本升级首启或全新安装首启。
- 验证：本机无 Qt/CMake，未编译验证。需在有 Qt 环境验证：全新启动建目录、旧数据迁移、装到只读目录可读写、铃声默认路径生效。

### A3: Settings 页（服务地址）

- 范围决策：调研发现 PLAN 原列的 5 类设置中，闹钟开关/铃声路径已在 `StoragePage`、AI 配置已在 `AiConfigPage`，
  唯一无 UI 的是 `appstate.toml` 的三个服务地址。按 Simplicity/不扩大范围，仅新增"服务地址"设置页，
  不把已有控件复制进大一统页面（已与用户确认：urls_only + 即时生效）。
- `config/appconfig.*`：新增 `saveOpenJudgeBaseUrl/saveJudgerBaseUrl/saveEmailVerifyUrl`，
  匿名 namespace 抽出 `writeAppStateRootKey(key, value, err)`（复用既有 `updateOrAppendRootKey` + `quoteTomlValue`，
  写回 appstate.toml 根级键，逻辑与 `saveRingPath/saveAlarmEnabled` 一致）。补 `#include <QFileInfo>`。
- 新增页面 `ui/pages/settingspage.*`：仿 `AiConfigPage` 布局（顶栏 Home/Dark Mode、左侧 Back、右侧表单），
  三个 `QLineEdit`（openjudge/judger/email_verify）+ Save + 状态行；信号
  `backRequested/homeRequested/themeToggleRequested/saveRequested(3 url)`，方法 `setUrls/showSaveSucceeded/showSaveFailed/setDarkMode`。
- `homepage.*`：新增 `settingsRequested()` 信号与 "URLs" 按钮（展开）/"U"（折叠），并入折叠可见性列表与图标刷新。
- `mainwindow.*`：新增 `m_settingsPage` 成员/创建/入栈/深色模式同步；首页 `settingsRequested` → `setUrls(load*(默认值))` 入栈；
  保存 → 依次 `save*`，成功后即时注入 `m_client->setBaseUrl/setJudgerBaseUrl`、`m_emailVerifyService->setBaseUrl`
  （空字段回退到与启动相同的默认值），无需重启。
- `CMakeLists.txt` 加入 `settingspage.cpp/.h`。
- 验证：本机无 Qt/CMake，未编译验证。需实测：改地址保存后即时生效、空值回退默认、重启后从 toml 读回一致。
