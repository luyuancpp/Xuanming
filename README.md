# Xuanming（玄冥）

一个基于 Unreal Engine 5.7 的战术 FPS 多人对战项目，支持 Dedicated Server + Client 架构。

---

## 项目结构

```
Xuanming/
├── Xuanming.uproject          # 项目入口
├── Source/
│   ├── Xuanming.Target.cs           # Game target（默认游戏）
│   ├── XuanmingEditor.Target.cs     # Editor target（编辑器）
│   ├── XuanmingServer.Target.cs     # Dedicated Server target
│   ├── XuanmingClient.Target.cs     # Client target
│   └── Xuanming/                    # 主 C++ 模块
│       ├── Xuanming.Build.cs
│       ├── Xuanming.h / .cpp
│       ├── Public/                  # 头文件
│       │   ├── XuanmingGameMode.h
│       │   ├── XuanmingGameState.h
│       │   ├── XuanmingPlayerController.h
│       │   ├── XuanmingPlayerState.h
│       │   └── XuanmingCharacter.h
│       └── Private/                 # 实现文件
│           ├── XuanmingGameMode.cpp
│           ├── XuanmingGameState.cpp
│           ├── XuanmingPlayerController.cpp
│           ├── XuanmingPlayerState.cpp
│           └── XuanmingCharacter.cpp
├── Config/
│   ├── DefaultEngine.ini      # 引擎/网络配置
│   ├── DefaultGame.ini        # 项目元数据
│   ├── DefaultInput.ini       # 输入配置（EnhancedInput）
│   └── DefaultEditor.ini      # 编辑器配置
├── Content/                   # 资源（蓝图/贴图/模型，由编辑器管理）
├── GenerateProjectFiles.bat   # 生成 VS 工程
├── BuildEditor.bat            # 编译 Editor
├── BuildServer.bat            # 编译 DS
├── BuildClient.bat            # 编译 Client
├── LaunchServer.bat           # 启动本地 DS
└── LaunchClient.bat           # 启动客户端连接本地 DS
```

---

## 首次使用

### 环境准备

1. **Visual Studio 2022**（必须勾选"使用 C++ 的游戏开发"工作负载）
2. **.NET SDK 8.0+**
3. **Git** 2.x
4. **Git LFS**（必装！UE 资源全靠它）
   ```cmd
   winget install GitHub.GitLFS
   git lfs install
   ```
5. **源码版 Unreal Engine 5.7.4**，路径 `F:\work\UnrealEngine`
   - 引擎必须先编译完成（已确认 `UnrealEditor.exe` 存在）

### 步骤

```cmd
REM 1. 生成 VS 工程文件
GenerateProjectFiles.bat

REM 2. 用 VS 打开 Xuanming.sln，或直接：
BuildEditor.bat

REM 3. 启动编辑器（双击 Xuanming.uproject 即可）
```

---

## 本地联调（DS + Client）

**两步**：

```cmd
REM 终端 1：启动 Dedicated Server（端口 7777）
LaunchServer.bat

REM 终端 2：启动客户端，自动连接 127.0.0.1:7777
LaunchClient.bat
```

要起多个客户端，多双击 `LaunchClient.bat` 即可。

---

## 打包 Dedicated Server（生产部署）

### Win64 Server

```cmd
BuildServer.bat
```

生成的二进制在 `Binaries\Win64\XuanmingServer.exe`。

### Linux Server（推荐生产环境）

```cmd
REM 需要先安装 Linux 交叉编译工具链
REM 见 https://dev.epicgames.com/documentation/zh-cn/unreal-engine/linux-development-requirements-for-unreal-engine

F:\work\UnrealEngine\Engine\Build\BatchFiles\RunUAT.bat ^
    BuildCookRun ^
    -project="%CD%\Xuanming.uproject" ^
    -noP4 -platform=Linux -clientconfig=Development -serverconfig=Development ^
    -cook -allmaps -build -stage -pak -archive ^
    -archivedirectory="%CD%\Build\LinuxServer" ^
    -server -noclient
```

部署到 Linux：
```bash
chmod +x XuanmingServer
./XuanmingServer -log -port=7777
```

---

## 客户端打包

```cmd
F:\work\UnrealEngine\Engine\Build\BatchFiles\RunUAT.bat ^
    BuildCookRun ^
    -project="%CD%\Xuanming.uproject" ^
    -noP4 -platform=Win64 -clientconfig=Development ^
    -cook -allmaps -build -stage -pak -archive ^
    -archivedirectory="%CD%\Build\WindowsClient"
```

---

## 网络架构（当前骨架）

```
┌─────────────────┐         ┌─────────────────┐
│   Client 1      │◄───────►│                 │
└─────────────────┘   UDP   │ Dedicated       │
┌─────────────────┐  :7777  │ Server          │
│   Client 2      │◄───────►│ (Authoritative) │
└─────────────────┘         │                 │
┌─────────────────┐         │  GameMode       │
│   Client N      │◄───────►│  GameState      │
└─────────────────┘         └─────────────────┘
```

**类层级（C++）：**

| 类 | 存在位置 | 职责 |
|---|---|---|
| `AXuanmingGameMode` | 只在 Server | 游戏规则、登入登出、胜负判定 |
| `AXuanmingGameState` | Server + 所有 Client | 比赛全局状态（计时、阶段） |
| `AXuanmingPlayerController` | 本地 Client + Server | 输入、UI、本玩家专属 |
| `AXuanmingPlayerState` | Server + 所有 Client | 玩家数据（K/D、队伍） |
| `AXuanmingCharacter` | Server + 所有相关 Client | 角色实体、血量、相机 |

**服务器权威原则**：伤害、移动校验、武器开火都必须在 Server 上判定，客户端只发请求和做预测。

---

## 下一步开发路线（建议优先级）

### Phase 1 - 基础玩法（2-4 周）
- [ ] EnhancedInput 接入（WASD 移动、鼠标视角、跳跃、蹲伏）
- [ ] 武器基类 `AXuanmingWeapon` + 服务器权威射击
- [ ] 命中检测（Hitscan + 线性追踪）
- [ ] 简单 HUD（血量、弹药）
- [ ] 重生流程

### Phase 2 - 多人对战（1-2 个月）
- [ ] 大厅/匹配（先 OnlineSubsystemNULL，后接 EOS 或自研）
- [ ] 队伍系统（红蓝队、友军伤害开关）
- [ ] 记分板 UI
- [ ] 战斗结算

### Phase 3 - 上线准备（3-6 个月+）
- [ ] 反外挂（商业方案，如 ACE / EasyAntiCheat）
- [ ] 账号 / 后端服务（独立后端团队）
- [ ] 服务器调度（K8s + 战斗服动态分配）
- [ ] 性能优化、移动端适配
- [ ] 商城 / 支付 / 通行证

> ⚠️ **现实提醒**：做到 Phase 3 这种规模通常需要 30+ 人团队、2 年以上时间、千万级预算。这个骨架只是技术起点。

---

## 故障排查

### "CoreMinimal.h not found" 红色波浪线
clang 静态分析器看不到 UE 头文件路径，**不影响实际编译**。跑 `GenerateProjectFiles.bat` 后用 VS 打开 `.sln` 编译即可。

### .bat 双击执行报 "'al' 不是内部或外部命令" + 乱码
原因：.bat 是 UTF-8 编码但 Windows CMD 默认 GBK，注释里的中文被当成命令执行。
**已修复**：所有 .bat 顶部加了 `chcp 65001 > nul`，注释也都换成英文。

### GenerateProjectFiles 报 "error CS1525: Invalid expression term '['"
原因：UE 5.7 的 `.Build.cs` 用了 C# 12 集合表达式 `[...]`，需要 **.NET 8 SDK**。
你拉下来的源码版引擎自带的 `UnrealBuildTool.exe` 可能是 net6.0 编译的旧版本。

**解决方案**：
1. 安装 .NET 8 SDK：`winget install Microsoft.DotNet.SDK.8`
2. 在引擎根目录（`F:/work/UnrealEngine`）放 `global.json`：
   ```json
   {
     "sdk": {
       "version": "8.0.100",
       "rollForward": "latestFeature"
     }
   }
   ```
3. 跑 `Setup.bat` 拉依赖（必做，否则 EpicGames.Horde 编不过）
4. 用引擎根目录的 `GenerateProjectFiles.bat`（不是项目里的）跑一次，强制重新编译 UBT
5. 之后再跑项目的 `GenerateProjectFiles.bat`

### "EpicGames.Horde.dll" 找不到 / `Protos\horde\log_rpc.proto` 缺失
原因：Setup.bat 没跑过，二进制依赖没下载。
**解决**：在引擎根跑 `Setup.bat` 或直接调用 `Engine/Binaries/DotNET/GitDependencies/win-x64/GitDependencies.exe --force`

### "Engine association not found"
`.uproject` 里的 `EngineAssociation` 指向 `F:/work/UnrealEngine`。如果引擎路径变了，手动修改这一行。

### Git LFS 没装
项目可以先跑，但**不要提交美术资源**，否则仓库会爆炸。先装 LFS。

### 服务器没人连
检查防火墙是否放行 UDP 7777 端口。

---

## 许可

私有项目，未经授权不得使用。
