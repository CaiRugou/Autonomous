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
  f_X(x) = \begin{cases}p(x)=P(X=x), x\in{x_i}, i=1,2,3,... \\ 0, 其它 \end{cases}
$$