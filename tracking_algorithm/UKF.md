## 无迹卡尔曼滤波 (Unscented Kalman Filter)

### UKF解决非线性模型

UKF从原始高斯分布中提取包括均值点在内的一系列代表点，并将代表点带入到非线性等式中，围绕这些点进行近似，从而获得更好的近似结果。

UKF不会对**非线性函数**进行**线性化**处理，它用所谓的 $sigma$ 点来近似概率分布。其优势在于在很多情况下, $sigma$ 点近似非线性转换的效果比线性化更好，此外它还可以省去计算复杂的雅可比矩阵的过程

## UKF $sigma$ 点选取

UKF代表点被称为 $sigma$ 点，它们分布在状态**均值**的周围，并且和每个状态维度的标准偏差(即 $σ$ )有关，因此被叫做 $sigma$ 点。它们代表了整个分布。

## 无损变换

当**高斯分布**经过**非线性函数变换**后，它**不再是高斯分布**，而无迹卡尔曼滤波通过无迹转换将非线性函数变换的结果近似成高斯分布。

以下是无迹变换执行的步骤：

- 计算 $sigma$ 点集合

- 为每个 $sigma$ 点分配权重

- 把所有单个 $sigma$ 点**代入到非线性函数**

- 对经过上述加权和转换的点**近似新的高斯分布**

- 计算新高斯分布的**均值和方差**

### 计算 $sigma$ 点集

选择合适的 $sigma$ 点数量 $n$ 取决于系统的状态维度N：$n=2N+1$

```math
\chi ^{[0]} = \mu
\\

\\
\chi ^{[i]} = \mu +(\sqrt{(n+\lambda)\sum})_i  \  ,i=1,...,n

\\

\\
\chi ^{[i]} = \mu - (\sqrt{(n+\lambda) \sum})_{i-n}  \ ,i =n+1, ..., 2n
```

-  $\chi$ 表示 $sigma$ 点矩阵， 矩阵中每一列表示一组 $sigma$ 点。 每个维度有 $n=2N+1＝5$ 个 $sigma$ 点。可以看到，同一列的 $sigma$ 点有两个对称的计算公式，这是由于同一列的上下两个 $sigma$ 点在原始高斯分布中围绕均值点呈对称分布。

- $\lambda$ 是比例因子， 它是告诉我们选择距离均值多元的 $sigma$ 点。 经验值为 $3-n$

- $\sum$ 是协方差矩阵， 对 $\sum$ 求平方需要找到一个满足一下条件的矩阵:

$$S: \sum = S.S \  or \   \sum = S.S^T, S= \sqrt{\sum}$$

### 计算 $sigma$ 点的权重

在我们挑选出合适的 $sigma$ 点之后，我们需要为每个 $sigma$ 点赋予合适的权重:

```math
\omega_m^{[0]} = {{\lambda} \over {n+\lambda}} \\

\\

\omega_c^{[0]} = \omega_m^{[0]} + {1 - \alpha ^2 + \beta} \\

\\

\omega_m^{[i]} = \omega_c^{[0]} = {1 \over {2(n+\lambda)}} \ ,i=1,...,2n

```

注意的是计算均值权重的公式与计算其余sigma 点权重的公式也是不同的，而且所有代表点权重之和等于1。

### 无迹转换参数

$sigma$ 点的选择和权重计算公式主要有以下四个可调节参数，这些参数被称作 $UT Parameters$ , 我们可以使用这些参数对无迹变换进行缩放：

```math
\kappa \geq 0 \\

\alpha \in (0, 1] \\

\lambda = \alpha ^2 (n+\kappa) -n \\

\beta = 2
```

其中最重要的是 $λ$ 扩散参数，数学研究表明，$λ$ 的最佳值为 $3-n$ 。如果λ已经给出，其余 $UT$ 参数可以考虑省略，从而简化公式，如在有的资料中计算 $sigma$ 点权重的公式为:

```math
\omega_m^{[0]} = {{\lambda} \over {n+\lambda}} \\

\omega_m^{[i]} = {1 \over {2(n+\lambda)}} \ ,i=1,...,2n
```

### 计算近似高斯分布的均值和协方差

现在我们已经得到了带有权重的，经过非线性等式转换后的 $sigma$ 点，我们可以通过这些 $sigma$ 点近似新的高斯分布，并计算其均值及协方差。

$$
\mu ^{'} = \sum _{i=0}^{2n} \omega_m^{[i]} g(\chi ^{[i]})
$$

$$
\sum {'} = \sum _{i=0}^{2n} \omega_c^{[i]} (g(\chi ^{[i]}) - \mu^{'})(g(\chi^{[i]})-\mu^{'})^T
$$

$\mu ^{'}$ 是预测的均值， $\sum{'}$ 是预测的协方差矩阵

这样我们就完成了无迹变换的全部过程，现在让我在预测更新和测量更新中融入无迹卡尔曼滤波。

### UFK 流程

预测

1. **计算 $sigma$ 点**。
2. **计算 $sigma$ 点权重**。
3. **转换 $sigma$ 点并计算新的均值和协方差。** 再增加外部的过程白噪声 $R$。

$$
\mu ^{'} = \sum _{i=0}^{2n} \omega_m^{[i]} g(\chi ^{[i]})
$$

$$
\sum {'} = \sum _{i=0}^{2n} \omega_c^{[i]} (g(\chi ^{[i]}) - \mu^{'})(g(\chi^{[i]})-\mu^{'})^T
$$

观察

在预测更新后我们得到了预测值的均值和协方差。 当我们得到来自传感器的测量值后，如果我们想获得预测值和测量值之间的差值，我们得首先从预测的状态空间转换到测量状态空间。

如果**状态空间的转换是非线性**的，则就**需要像预测步骤一样进行无迹转换**。

通过公式 $h(x)$ 我们可以把预测状态空间转换为测量状态空间。

$$
Z = h(\chi)
$$

$$
\hat {z} = \sum _{i=0}^{2n} \omega^{[i]} Z ^{[i]}
$$

```math
S = \sum _{i=0}^{2n} \omega^{[i]} (Z^{[i]} - \hat z)(Z^{[i]} - \hat z)^T + Q
```

 $\Zeta$  把预测空间 $sigma$ 点的 $\chi$ 转为测量空间， $\hat z$ 预测 $sigma$ 点再测量空间的的均值， $S$ 协方差矩阵， $Q$ 是测量噪声。

 这里需要注意，由于我们不再将函数线性化，因此我们不再有雅可比行列式。

 ### 计算**卡尔曼增益 $K$**

首先我们要计算预测误差：状态空间中的 $sigma$ 点与测量空间中的  $sigma$ 点之间的交叉关联矩阵T。

$$
T = \sum _{i=0}^{2n} \omega ^{[i]} (\chi^{[i]} - \mu^{'})(Z^{[i]} - \hat {z})^T
$$

$$
{\color{red}K} = T.S^{-1}
$$

### 更新

$$
\mu = \mu ^{'} + {\color{red}K}(z - \hat z)
$$

$$
\sum = (I -{\color{red}K}T)\sum{'}
$$


