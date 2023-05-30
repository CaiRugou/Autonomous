# **Bayes Fliter**

- 贝叶斯滤波基于贝叶斯公式，通过上一时刻的状态及当前时刻的输入，对当前时刻的状态作出预测，并通过当前时刻的观测对预测作出更新（也可称为纠正），最终实现对当前时刻状态的估计。贝叶斯滤波思想是卡尔曼滤波、粒子滤波等算法的基础。本文从概率基础概念出发，逐步进行贝叶斯滤波的推导。

## 概率基础知识

### 随机变量
定义在样本空间 $\Omega$ 上的实值函数： 

$$
X = X(\omega), \omega \in \Omega
$$ 

被称作为**随机变量**,如下图所示： $\omega$ 表示样本空间 $\Omega$ 的**随机事件**，可能与数据量有关（如激光雷达测量得到的前方障碍物的距离值，随机投掷骰子得到的点数），也可能和数量无关（如随机投掷硬币的结果可能为正面或者反面）。

![image](https://github.com/CaiRugou/Autonomous/blob/main/img/sample.png)

**随机变量**表示**随机试验各种*结果*的实值单值函数**。随机事件不论与数量是否直接有关，都可以数量化，即都能用数量化的方式表达。

“随机变量 $X$ 的取值为 $x$ ”就是满足等式 $X(\omega) = x$ 的一切 $\omega$ 组成的集合，简记为“ $X=x$ ”, 这是 $\Omega$ 的一个子集，即：

$$
X=x = {\omega : X(\omega) = x} \subset \Omega
$$

类似地，有：

$$
X \leq x = {\omega : X(\omega) \leq x} \subset \Omega
$$

### 离散型随机变量
如果随机变量的函数值是实数轴上孤立的点（有限个或者无限个），则称为**离散型随机变量**：

![image](https://github.com/CaiRugou/Autonomous/blob/main/img/aix.png)

### 连续型随机变量
如果随机变量的函数值是实数轴上某个区间上所有的值（也可以是 $(−∞,+∞)$ 区间），则称为连续型随机变量：

![image](https://github.com/CaiRugou/Autonomous/blob/main/img/continue.png)

### 累积分布函数
设 $X$ 为一个随机变量，对于任意实数 $x$ ,称：

 $$
 F(x) = P(X \leq x)
 $$

为随机变量 $X$ 的累积分布函数(Cumulative Distribution Function，CDF), 又称作概率分布函数，简称**分布函数**。

若 $X$ 为离散型随机变量，存在函数 $f_X(x)$ ,使其分布函数满足：

$$
F(x) = P(X \leq x) = \sum_{u \leq x} f_X(u)
$$

若 $X$ 为连续型随机变量，存在函数 $f_X(x)$ ,使其分布函数满足：

$$
F(x) = P(X \leq x) = \int_ {-\infty}^xf_X(u)du
$$

分布函数满足单调性、有界性和右连续性：
- 单调性

  对任意 $x_1 \leq x_2$ ,有 $F(x_1) \leq F(x_2)$

- 有界性

  对任意 $x$ ,有：

$$
    0 \leq F(x) \leq 1
$$

  且有：

$$
    \lim_{x->-\infty} = 0, \lim_{x->+\infty} = 1
$$

- 右连续性
  对任意 $x_0$ ,有：

$$
  \lim_{x->x_0^+} F(x) = F(x_0)
$$

### 概率质量函数

假设 $X$ 为离散型随机变量，其全部可能取值为 $x_1,x_2,x_3,⋅⋅⋅$ ，其概率分布列可表述为：

| $X$ | $x_1$    | $x_2$    | $x_3$    | $...$ |
|-    | :-:      | :-:      | :-:      | -:    |
| $P$ | $p(x_1)$ | $p(x_2)$ | $p(x_3)$ | $...$ | 

则称：

$$
  f_X(x) = \begin {cases} p(x)=P(X=x), x\in{x_i}, i=1,2,3,... \\ 0, other \end {cases}
$$

为离散型随机变量 $X$ 的概率质量函数Probability Mass Function，PMF）。也可以采用如下的表示方法：

$$
X :~ p(x)
$$

表示 $X$ 服从 $p(x)$ 的概率分布。
概率质量函数表征了离散型随机变量在各特定取值上的概率，其满足非负性、规范性和可加性：
- 非负性

$$
f_X(x_i) \geq 0
$$

- 规范性和可加性

$$
\sum_{i=1}^\infty f_X(x_i) = 1
$$

**离散型随机变量概率质量函数**的**叠加**得到了离散型随机变量的**累积分布函数**。

### 概率密度函数

假设 $X$ 为连续型随机变量，存在函数 $f_X(x)$，且满足：

- 非负性

$$
f_X(x_i) \geq 0
$$

- 规范性和可加性

$$
\int_{-\infty}^{+\infty} fX(x)dx = 1
$$

则称函数 $f_X(x)$ 为连续型随机变量 $X$

的概率密度函数（Probability Density Function，PDF），连续型随机变量概率密度函数的积分得到了连续型随机变量的累积分布函数。

概率密度函数表征了连续型随机变量的输出值在某个确定的取值点附近的可能性。连续型随机变量的输出值在某个确定的取值点上的概率为 $0$ ，但并不代表该事件不会发生。

### 联合概率

#### 二维离散型随机变量的联合概率质量函数

若二维离散型随机变量 $(X,Y)$ 所有可能的取值为:

$$
(x_i, y_j), i,j = 1,2,3,...
$$

且存在函数 $f_{X,Y}(x,y)$ :


$$
f_{X,Y}(x,y) = p(x,y) = P(X=x, Y=y), (x,y)\in{x_i,y_i},i,j=1,2,3,...
$$

函数 $f_{X,Y}(x,y)$ 满足如下性质：

- 非负性

$$
f_{X,Y}(x_i,y_j) \geq 0
$$

- 规范性和可加性

$$
\sum_{i=1}^\infty \sum_{j=1}^\infty f_{X,Y}(x_i, y_j) = 1
$$

则称函数 $f_{X,Y}(x,y)$ 为 $(X,Y)$ 的联合概率质量函数（Joint Probability Mass Function），又称联合分布列，此定义可以推广到多维离散型随机变量。

**二维离散型随机变量联合概率质量函数**的**叠加**得到了二维离散型随机变量的联合累积分布函数（Joint Cumulative Distribution Function）:

$$
F_{X,Y}(x,y) = P(X \leq x, Y \leq y) = \sum_{u \leq x} \sum_{v \leq y}f_{X,Y}(u,v)
$$

#### 二维连续型随机变量的联合概率密度函数

若对于二维连续型随机变量 $(X,Y)$ ，存在二元函数 $f_{X,Y}(x,y)$ 满足：

- 非负性

$$
    f_{X,Y}(x,y) \geq 0
$$

- 规范性和可加性

$$
\int_{-\infty}^{+\infty} \int_{-\infty}^{+\infty} f_{X,Y}(x,y)dxdy = 1
$$

则称函数 $f_{X,Y}(x,y)$ 为 $(X,Y)$ 的联合概率密度函数（Joint Probability Density Function），此定义可以推广到多维连续型随机变量。

二维连续型随机变量联合概率密度函数的积分得到了二维连续型随机变量的联合累积分布函数：

$$
F_{X,Y}(x,y)=P(X \leq x,Y \leq y)= \int_{-\infty}^x \int_{-\infty}^y f_{X,Y}(u,v)dvdu
$$

### 边缘概率