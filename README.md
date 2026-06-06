# oj-client

`oj-client` 是一个基于 Qt Widgets 开发的 OpenJudge 桌面客户端，集成了题目浏览、代码提交、本地测试、AI 辅助、收藏、缓存管理、截止提醒和 Windows 安装打包能力。  
邮件验证服务 [OJ-server](https://github.com/2379836379/OJ-server)  
程序测试服务 [OJ-judger](https://github.com/2379836379/OJ-judger)  

> 本轮新增功能的完整改动说明见 [实现变更文档.md](实现变更文档.md)。

## 项目目标

这个项目的目标是把 OpenJudge 上常见的做题流程放到一个本地桌面应用里完成：

- 登录课程 / 比赛账号
- 浏览课程、比赛和题目
- 查看题面并直接编写代码
- 运行本地测试
- 提交代码并查看判题结果
- 使用 AI 辅助解释、改代码、跑测试和提交
- 管理收藏、缓存和提醒

## 当前功能总览

### 1. 登录与账号缓存

- 支持账号密码登录 OpenJudge。
- 支持邮箱验证码相关流程。
- 支持记住最近登录账号和密码。
- 首次进入时会自动尝试恢复最近一次登录信息。
- 已缓存邮箱可跳过重复验证提示。

### 2. 首页与课程浏览

- 首页显示当前课程列表。
- 支持进入课程页查看课程下的 contest 列表。
- `Due Soon` 区域会显示临近截止的 contest。
- `Due Soon` 会按截止时间从近到远排序。
- 已完成的 contest 会从 `Due Soon` 中移除。
- `Due Soon` 中的 contest 显示截止日期。
- contest 加入 `Due Soon` 时会去重。

### 3. Contest 与题目列表

- 支持打开 contest 页面查看题目列表。
- 题目列表可显示做题状态。
- 已完成题目会在题目名称同一行右端显示 `finished` 标记。
- `finished` 在浅色 / 深色模式下使用高亮绿色。

### 4. Problem 页面

题目页目前是项目功能最集中的页面，包含：

- 题面展示
- 提交区
- 测试区
- AI 区
- Notes 侧栏（标签 / 笔记 / 任务状态 / 错题本）
- 收藏入口
- Home / Dark Mode 顶部按钮
- 左侧可折叠工具栏
- 题面顶栏 **Web** 按钮：在系统浏览器打开当前题目

题面展示能力包括：

- 显示时间限制、内存限制、描述、输入、输出、样例、提示
- 不显示题目 URL
- 不重复显示题目标题
- 支持白天 / 黑夜模式
- 深色模式下题面文字自动切换为浅色
- 样例输入 / 输出按普通文本展示

### 5. Starter Code 提取与自动填充

- 题目解析器会从题面中提取已有代码段。
- 对题面中多个 `pre` 代码块，支持提取中间已有模板代码作为 `starterCode`。
- 打开题目提交页时，会优先把 `starterCode` 填入代码编辑器。
- 如果当前题目已有本地草稿，则优先恢复草稿而不是覆盖成 `starterCode`。

### 6. 提交区

- 支持加载提交页面并解析可选语言列表。
- 支持保留当前题目的语言选择。
- 重新加载 submit 选项时，不会把用户手动选择的语言重置回默认语言。
- 支持代码编辑、提交和结果查看。
- 支持题目级代码草稿缓存；重新进入题目时不会丢失代码。

当前提交结果展示策略：

- 提交发送时显示 `Submitting...`
- 提交响应只显示 `Submit Status: ...`
- 判题结果只显示 `Judge Status: ...`
- 如果有编译错误 / 运行错误 / 详细报错，会直接显示原始错误标题和错误正文
- 不再显示请求 payload、HTTP 细节、URL、原始响应预览等低层调试信息

### 7. 本地测试 / Judge

- 支持调用独立 judge 服务运行当前代码。
- 根据提交语言自动映射文件名，例如：
  - C/C++ -> `main.cpp`
  - Python -> `main.py`
  - Rust -> `main.rs`
- 点击 `input` 后可对当前代码执行本地测试。
- 点击 `sample` 可一键将样例输入填入测试输入框。

当前测试结果展示策略：

- 测试进行中只显示 `Running test...`
- 成功时只显示 `stdout`
- 失败时只显示错误信息

### 8. 代码编辑器

提交区代码框已经做成轻量 IDE 风格，包括：

- 浅色 IDE 风格代码区
- 独立行号列
- 自定义字体和配色
- 自动补全成对括号
- 回车自动继承缩进
- 在 `{` 后回车自动生成块级缩进结构

### 9. 语法高亮

项目当前使用 Tree-sitter 做语法分析和高亮，不依赖 clangd / pylsp。

已接入：

- `tree-sitter-cpp`
- `tree-sitter-python`
- `tree-sitter` runtime

当前支持：

- C++ 高亮
- Python 高亮
- 根据 submit 当前语言自动切换高亮模式

当前没有启用：

- LSP 诊断
- 红色错误标记
- clangd / pylsp 相关能力

### 10. AI 辅助

项目内置 AI 对话区，支持配置 OpenAI 兼容接口。

AI 已实现的能力：

- 读取当前题面
- 读取当前语言
- 读取当前代码
- 读取测试输入
- 读取测试输出
- 替换代码
- 替换测试输入
- 触发本地测试
- 触发提交

AI 对话支持：

- 预设提示按钮：解释题意 / 写暴力解 / 优化复杂度
- 流式响应
- Markdown 渲染显示回答
- 多轮对话记录显示在响应区
- 工具调用失败时回传错误信息
- `/responses` 空结果时自动 fallback

当前 fallback 顺序：

1. 流式 `/responses`
2. 非流式 `/responses`
3. `/chat/completions`

其中：

- `/responses` 链路支持工具调用
- fallback 到 `/chat/completions` 时，目前主要用于避免空响应卡住

### 11. AI 翻译题面

Problem 页支持对题面部分内容做 AI 翻译。

当前翻译范围：

- Description
- Input
- Output
- Hint

翻译特性：

- `Translate` 按钮放在题面框顶部
- `Show Original` 按钮可切回原文
- 翻译结果会缓存
- 再次点击翻译时优先显示缓存结果，不重复请求
- 只翻译指定文本，不翻译代码、变量名、URL、样例 IO

### 12. 收藏系统

- 支持创建收藏夹
- 支持将当前题目保存到指定收藏夹
- 支持查看收藏夹中的题目
- 支持从收藏中重新打开题目
- 收藏数据本地持久化
- 收藏题目同时保存题面和相关信息，便于离线回看
- 支持导出 / 导入收藏夹为 JSON（合并导入）

### 13. 本地刷题管理（标签 / 笔记 / 看板 / 统计）

- **Notes 侧栏**：为每道题记录任务状态、难度、标签、个人笔记，可勾选加入错题本
- **任务看板**：首页入口 Task Board，四栏展示未开始 / 进行中 / 已完成 / 待重做，右键切换状态
- **统计与错题本**：首页入口 Stats，展示状态分布、标签 Top 8、笔记数、收藏数；错题本列表可点击打开
- 数据存储于 `data/problemmeta.db`，与收藏、提交等逻辑独立

### 14. 缓存与本地存储

项目当前维护多类本地缓存：

- 首页缓存
- 课程缓存
- contest 缓存
- 题目缓存
- 登录缓存
- 收藏数据库
- 题目元数据数据库（`problemmeta.db`：标签、笔记、任务状态、错题本）
- 题目代码草稿缓存
- 题目语言选择缓存
- AI 翻译缓存

Storage / Set 页面支持：

- 查看缓存大小
- 查看应用大小
- 一键清理缓存
- 显示当前状态信息

### 15. 截止提醒与闹钟

项目支持截止提醒和本地响铃。

当前能力：

- 解析课程 / contest 截止时间
- 在首页显示 `Due Soon`
- 支持开启 / 关闭 Alarm
- 可手动测试 alarm
- 可手动选择本地音频文件作为铃声
- 铃声路径会持久化
- 自动提醒和手动测试都会播放同一个铃声文件

提醒触发规则：

- 打开 alarm 开关时立即检查一次
- 之后每到整点自动检查一次
- 当 contest 距截止时间剩余 1 / 2 / 3 小时时各提醒一次
- 提醒时显示系统托盘通知并播放音频

### 16. 系统托盘

- 关闭主窗口时默认最小化到系统托盘
- 托盘图标支持右键菜单
- 支持从托盘恢复窗口
- 支持从托盘直接退出应用
- 首次关闭到托盘时会显示通知提示

### 17. 外观与交互

当前 UI 已支持：

- 全局字体显式设置为 `Microsoft YaHei UI`
- 白天 / 黑夜模式切换
- 顶部操作按钮图标化
- 多页面统一卡片式布局
- 左侧工具栏可折叠
- Home 按钮统一返回首页

已覆盖页面：

- LoginPage
- HomePage
- ClassPage
- ContestPage
- ProblemPage
- FavoritePage
- StoragePage
- AiConfigPage

### 18. 图标与资源

- 应用图标、任务栏图标、托盘图标统一使用项目资源图标
- 浅色 / 深色模式按钮图标分别来自：
  - `oj-client/images/light_mode`
  - `oj-client/images/dark_mode`

### 19. 测试与持续集成

- 解析器单元测试：`oj_parser_tests`（QtTest），覆盖 `ProblemParser`、`ContestParser`、`ParserCommon`
- GitHub Actions：`.github/workflows/build.yml`，三平台矩阵构建 + `ctest`
- CPack 打包：macOS `.dmg`、Linux `.tar.gz`、Windows `.zip`

### 20. 安装包与发布

项目已经提供 Windows 安装脚本：

- 安装脚本：`oj-client/installer.iss`
- 使用 Inno Setup 打包
- 安装包会带上：
  - `oj.exe`
  - Qt 运行库
  - 图标资源
  - 铃声音频文件

安装后初始化行为：

- 会生成 `appstate.toml`
- 默认铃声路径指向安装目录内置音频文件

当前说明：

- `config.toml` 只用于 AI 配置
- 其他运行状态保存在 `appstate.toml`
- 安装脚本不主动把项目源码目录里的 `cache` / `data` 打进安装包

## 项目结构

主要目录如下：

```text
oj-client/
├─ src/
│  ├─ config/        配置读写
│  ├─ network/       OpenJudge / OpenAI 网络访问
│  ├─ parser/        HTML / 结果解析
│  ├─ repository/    本地数据库与远端仓库封装
│  ├─ service/       业务逻辑层
│  └─ ui/            主窗口与各页面
├─ images/           UI 图标资源
├─ vedio/            铃声音频资源
├─ third_party/      tree-sitter 及语法库
├─ installer.iss     Inno Setup 安装脚本
├─ CMakeLists.txt    CMake 构建文件
├─ config.toml       AI 配置
└─ appstate.toml     运行时状态配置
```

## 技术栈

- C++
- Qt 6
  - Core
  - Widgets
  - Network
  - Sql
  - Multimedia
- SQLite
- Tree-sitter
- OpenAI-compatible API
- Inno Setup

## 构建方法

### 1. 依赖

需要准备：

- Qt 6.5 或更高版本
- CMake 3.19 或更高版本
- Windows 下可用的 MinGW 或其他兼容编译器

### 2. Debug / Release 构建

在 `E:\csqt\oj-client` 下执行：

```powershell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

构建成功后可执行文件位于：

```text
E:\csqt\oj-client\build\oj.exe
```

### 3. Windows 安装包

安装 Inno Setup 后，使用：

```text
oj-client/installer.iss
```

生成安装包。

### 4. macOS / Linux 构建

仓库根目录的 CMake 工程位于 `oj-client/` 子目录，可在 macOS / Linux 上直接构建：

```bash
# 在仓库根目录执行
cmake -S oj-client -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

构建产物：

- macOS：`build/oj.app`
- Linux：`build/oj`

依赖准备：

- macOS：安装 Qt 6.5+（建议用官方在线安装器或 `brew install qt`），需包含 `qtmultimedia` 模块
- Linux（Debian/Ubuntu）：`sudo apt-get install -y libgl1-mesa-dev libpulse-dev`，并安装 Qt 6.5+

### 5. 运行单元测试

解析器单元测试基于 QtTest，构建后用 `ctest` 运行：

```bash
cmake -S oj-client -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
cd build && ctest --output-on-failure
```

### 6. 跨平台打包（CPack）

构建完成后，可用 CPack 生成对应平台的分发包：

```bash
cd build
cpack            # macOS 生成 .dmg；Linux 生成 .tar.gz；Windows 生成 .zip
```

### 7. 持续集成（GitHub Actions）

`.github/workflows/build.yml` 会在 push / PR 时于 `ubuntu-latest`、`windows-latest`、
`macos-latest` 三个平台自动安装 Qt 6.5、构建并运行 `ctest`。

## 配置说明

### 1. `config.toml`

用于保存 AI 配置，例如：

- `base_url`
- `model`
- `api_key`
- `system_prompt`

### 2. `appstate.toml`

用于保存运行状态，例如：

- 闹钟开关
- 铃声路径
- `openjudge_base_url`：OpenJudge 主站地址
- `judger_base_url`：本地判题服务地址
- `email_verify_url`：邮箱验证服务地址
- 其他非 AI 配置

## 当前已知限制

- AI 工具调用能力依赖所接入的 OpenAI 兼容服务是否正确支持 `/responses`。
- judge / email / OpenJudge 地址可通过 `appstate.toml` 配置，未配置时使用内置默认值。
- 缓存和登录数据库目前仍默认写在项目 / 应用目录下，而不是统一放到用户目录。

## 后续可继续完善的方向

- 任务看板支持拖拽改状态
- 元数据导出 / 导入
- 增强题面解析兼容性(特指某些代码填空题)
- 继续完善 C++ / Python 语法高亮规则
- 补充自动化测试和 CI, 改进 AI 的工具调用能力(正在完成)

