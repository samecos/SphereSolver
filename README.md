# SphereSolver

SphereSolver 是一个简单的 C++ 工具，用于从已知半径和球面采样点估计球心位置，并生成带噪声的球面点用于测试。

**特点**

- 基于梯度下降（Gradient Descent）的球心估计。
- 支持传入噪声方差作为优化时的权重提示（若已知）；也可自动从点云估计方差。
- 能生成任意数量的球面采样点并在坐标上加入高斯噪声。

**文件结构（关键）**

- `src/main.cpp`：CLI 界面，提供生成点与估计球心的交互。
- `src/SphereUtils.h` / `src/SphereUtils.cpp`：核心实现。

**API 说明（重要）**
函数签名（核心）：

```cpp
static Point3D SphereUtils::calculateSphereCenter(double radius, const std::vector<Point3D>& points, double inputVariance = -1.0, double* outVariance = nullptr);
```

- `radius`：已知半径。
- `points`：采样的球面点列表（每点为 `Point3D { x, y, z }`）。
- `inputVariance`：可选输入。若 > 0，则函数在优化中将使用该值作为噪声方差提示（用于按残差给点加权）；若 <= 0，则函数会在开始时根据点云自动估计初始方差。
- `outVariance`：可选输出指针，函数返回后会把最终的均方残差（mean squared residual）写入该位置，表示点到理想球面（distance - radius）的平方均值。

**构建（Windows, PowerShell, 使用 CMake）**
先在项目根目录（包含 `CMakeLists.txt`）执行：

```powershell
# 创建并进入 build 目录
mkdir build; cd build
# 生成 Visual Studio 解决方案（默认使用系统的 MSVC）
cmake ..
# 使用 Release 配置构建
cmake --build . --config Release
```

或者在已有 `build` 的情况下直接：

```powershell
cd build
cmake --build . --config Release
```

构建完成后，在 `build/` 或 `build/Release` 中会有可执行文件 `SphereSolver.exe`。

**运行示例**
运行程序后，你可以通过菜单选择：

- 生成带噪声的球面点：输入中心坐标、半径、噪声方差（噪声方差指的是坐标噪声的方差）和点数；程序会打印生成的点。
- 使用已知半径和若干点估计球心：程序会提示你输入“已知噪声方差（输入负值以自动估计）”，若你已知采样点的噪声强度，填入该方差能帮助优化更快且更鲁棒。最终会打印估计的球心和均方残差（用于衡量拟合质量）。

示例交互（伪）:

```
--- Sphere Solver ---
1. Calculate Sphere Center (from Radius + Points)
2. Generate Noisy Sphere Points
3. Exit
Select option: 2
Enter Center (x y z): 1 2 3
Enter Radius: 5
Enter Variance (Noise Power): 0.01
Enter number of points to generate: 100
# 程序输出 100 个点

# 之后用生成的点测试估计
Select option: 1
Enter known Radius: 5
Enter number of points: 100
Enter points (x y z):  # 粘贴上面生成的 100 行坐标
Enter known noise variance (enter negative to auto-estimate): 0.01
Calculated Center: (1.0001, 2.0003, 2.9998)
Estimated mean squared residual (variance): 0.0098
```

**调优建议**

- `inputVariance` 若准确可以显著提高鲁棒性；若不确定，可先用负值自动估计，再根据输出的 `outVariance` 调整。
- 如果估计不稳定，可尝试减少 `learningRate` 或增加 `iterations`（目前实现里使用了基于估计噪声的自适应学习率）。

**扩展/改进想法**

- 引入更成熟的优化器（如 Levenberg–Marquardt）替代简单的梯度下降以提高收敛速度与稳定性。
- 增加单元测试，自动化验证在不同噪声与点数下的估计误差。

如果你希望，我可以：

- 添加一个小的测试程序 `tests/`，生成点并自动验证球心估计误差；
- 把 `calculateSphereCenter` 的返回改为包含 variance 的结构体；
- 或者现在就为你运行一次构建与示例，并把输出截取给你。

---

`SphereSolver` — 简洁的球面拟合与数据生成工具，方便快速验证噪声对球心估计的影响。
