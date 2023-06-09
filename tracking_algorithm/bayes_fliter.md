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
  f_X(x) = \begin {cases}  p(x)=P(X=x), x\in{x_i}, i=1,2,3,... \\ 
  0, other \end {cases} 
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
\int_{-\infty}^{+\infty} f_X(x)dx = 1
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
#### 二维离散型随机变量的边缘概率质量函数
若二维离散型随机变量 $(X,Y)$ 所有可能得取值为

$$
(x_i,y_j), i,j=1,2,3,...
$$

且联合概率质量函数 $f_{X,Y}(x,y)$ 为：

$$
f_{X,Y}(x,y) = p(x,y)=P(X=x,Y=y),(x,y)\in {x,y},i,j=1,2,3,...
$$

对 $j$ 求和所得的函数：

$$
f_X(x)=\sum_{j=1}^\infty f_{X,Y}(x,y_i) = \sum_{j=1}^\infty P(X=x,Y=y_j) = P(X=x), x\in \{x_i\}, i=1,2,3,...
$$

称为 $X$ 的**边缘概率质量函数**（Marginal Probability Mass Function），又称边缘分布列。

类似的对 $i$ 求和所得的函数：

$$
f_Y(y) = \sum_{i=1}^\infty f_{X,Y}(x_i, y) = \sum_{i=1}^\infty P(X=x, Y=y) = P(Y=y), y \in \{y_i\}, j = 1,2,3,...
$$

称为 $Y$ 的边缘概率质量函数。

**二维离散型随机变量边缘概率质量函数的叠加得到了二维离散型随机变量的边缘累积分布函数（Marginal Cumulative Distribution Function）：**

$$
F_X(x) = P(X \leq x) = \sum_{u \leq x} f_X(u) = \sum_{u \leq x} \sum_{j=1}^\infty P(X=u, Y=y_i) = \sum_{u \leq x} P(X=x)
$$

$$
F_Y(y) = P(Y \leq y) = \sum_{v \leq y}f_Y(v) = \sum_{v \leq y} \sum_{x=1}^\infty P(X=u_i, Y=y) = \sum_{v \leq y} P(Y=y)
$$

##### **离散联合概率举例**

上文提到**随机变量**，一个随机变量是从样本空间到实数的映射。然而，所谓的映射是人为创造的。从一个样本空间，可以同时产生多个映射。比如，我们的实验是**连续三次投硬币**，样本空间为 $\Omega$ :

$$
\Omega = \lbrace  正正正， 正正反，正反正，反正正，正反反，反正反，反反正，反反反  \rbrace
$$

在**同一样本空间**上，我们可以定义**多个随机变量**，映射到实数空间 $R$，比如:

- $X$ : 每次投掷为正面的硬币的总数，可以取值 $0，1，2，3 $
- $Y$ : 最后一次出现反面的情况，可以取值 $0, 1$ 
- $Z$ : 将正记着10，反面记着5， 第一次和第三次取值差值，可以取值为 $5, -5, 0$

这三个随机变量 $X, Y, Z$ 可以看作有三个分量的矢量，所以定义在同一个样本空间的多个随机变量，是一个样本空间到矢量的映射。其中投掷硬币就是**随机事件**。

如果样本空间 $\Omega$ 中每个结果出现的概率相等。可以知道该样本空间有 $8$ 个结果，那么每个结果出现的概率为 $1 \over 8$ 。由此我们可以计算联合概率，比如：

$$
P(X=0,Y=1) = P({反反反}) = {1 \over 8}
$$

表示 $X$ 中出现正面次数为 $0$ 且 $Y$ 中出现最后一次出现反面

$$
P(X=1, Y=1) = P({正反反，反正反}) = {2 \over 8}
$$

对于 $X=x,Y=y，$ 我们寻找样本空间中满足这两个取值的所有元素。这些元素构成一个样本空间的子集，该子集的概率就是 $P(X=x,Y=y)$ 的联合概率。 $p(x,y)=P(X=x,Y=y)$ 称为联合概率质量函数(joint PMF, joint probability mass function)。联合概率可以看做两个事件同时发生时的概率，事件 $A$ 为 $X=x$ ，事件 $B$ 为 $Y=y$ ，即 $P(A\bigcap B)$ 。

找到所有可能取值组合的概率，就找到了这两个随机变量的联合分布:

| $X$ | $Y$ | $P(X,Y)$ | 对应子集   |
|-    | :-:         | :-:      | :-:       |
| 0 | 0 | 0         | $\emptyset$          |
| 1 | 0 | $1\over8$ | ${反反正}$          |
| 2 | 0 | $2\over8$ | ${反正正}，{正反正}$ |
| 3 | 0 | $1\over8$ | ${正正正}$ |
| 0 | 1 | $1\over8$ | ${反反正}$ |
| 1 | 1 | $1\over8$ | ${正反反}，{反正反}$ |
| 2 | 1 | $2\over8$ | ${正正反}$ |
| 3 | 1 | $0$       | $\emptyset$ |

联合分布描述了所有可能得取值情况。因此，联合概率密度函数的累计和为 $1$ 。

##### **离散边缘概率举例**
联合分布包含了多个随机变量的分布信息。我们当然可以从联合分布中，提取出任意一个单一随机变量的分布，也就是所谓的边缘分布(marginal distribution)。
对于离散随机变量，可以获得<font color="#660000">边缘概率质量函数</font>(marginal pmf):

$$
p(x) = \sum_{all \  y} p(x,y)
$$

$$
p(y) = \sum_{all \ x} p(x,y)
$$

在求解 $x$ 的单一边缘分布时，则累加相同 $x$ 值、不同 $y$ 值时的多个联合概率，从而获得该 $x$ 值的总体概率，即**边缘概率**。

以上述两个随机变量 $(X, Y)$ 为例子：

|     | $x$ | $0$ | $1$ | $2$ | $3$ | |
|-    | -   |:-:  | :-: | :-: | :-:    |-: |
| $y$ |     |     |     |     |     |$p_Y(y)$ |
| $0$ |     | 0   | $1\over8$  | $2\over8$ |  $1\over8$  | $1\over2$|
| $1$ |     | $1\over8$   | $2\over8$   |   $1\over8$ |  0  | $1\over2$|
|     |$p_X(x)$| $1\over8$   | $3\over8$   |   $3\over8$ |  $1\over8$  | |

边缘概率就是对各行、各列的累加，可以得到最后一列 $p_Y(y)$ 是 $Y$ 的分布， $Y$ 有 $1\over2$ 的概率取 $0$ ， $1\over2$ 的概率取 $1$ .最后一行 $p_X(x)$ 是 $X$ 的分布。

### 条件概率

#### 二维离散型随机变量的条件概率质量函数

若 $(X,Y)$ 是二维离散型随机变量，对于固定的 $j$ ， 若 $ P(Y=y_j) > 0 $ , 则称：

$$
f_{X|Y}(x|y) = P(X=x|Y=y) = { {P(X=x,Y=y)} \over {P(Y=y)} } = {f_{X,Y}(x,y) \over f_Y(y)}, (x,y) \in {x_i, y_i},i,j = 1,2,3,...
$$

为 $Y = y$ 条件下离散型随机变量 $X$ 的条件概率质量函数。
同样的，随机变量 $X = x$ 具有类似的表达，不在赘述。

简而言之，对于二维离散型随机变量：

$$
条件概率质量函数 = {联合概率质量函数 \over 边缘概率质量函数}
$$

**二维离散型随机质量条件概率质量函数的叠加得到了二维离散型随机变量的条件累积分布函数**：

$$
F_{X|Y}(x|y) = P(X \leq x | Y =y) = \sum_{u\leq x} f_{X|Y}(u|y) = \sum_{u\leq x} {f_{X,Y}(u,y) \over f_Y(y)}
$$

$$
F_{Y|X}(y|x) = P(Y \leq y | X=x) = \sum_{v\leq y} f_{Y|X}(v|x) = \sum_{v\leq y} {f_{X,Y}(x,v) \over f_X(x)}
$$

#### 举例

基于上述介绍的条件概率，如果事件B发生，得到事件A发生的概率。相同的概念可以引申到随机变量。随机变量 $Y$ 取值 $y$, 另外一个随机变量 $X$ 为 $x$ 的概率。

假设 $p_Y(Y=y) \not= 0$ , 在 $Y = y$ 的条件下，随机变量 $X$ 取值为 $x$ 的概率定义为：

$$
p(x|y) = {p(x,y) \over p_Y(Y=y)}
$$

即 $X=x, Y=y$ 同时发生的概率，除以随机变量 $Y$ 取值 $y$ 的边缘概率。

接着上诉 三次投掷硬币的例子：

最后一次为反面的次数为 $Y=0$ 的条件下，求出现两次正面 $X = 2$ 的概率。


$$
p(X=2| Y=0) = {{p(X=2, Y=0)} \over {p_Y(0)}} = {{2/ 8} \over {1/ 2}}  = 0.5
$$

### 全概率

### 二维离散型随机变量的全概率公式

对于二维离散型随机变量 $(X, Y)$ , 已知条件概率质量函数：

$$
f_{X|Y}(x|y) = {{f_{X,Y}(x,y)} \over {f_Y(y)}}, (x,y) \in {x_i, y_i}, i, j=1,2,3,...
$$

$$
f_{Y|X}(y|x) = {{f_{X,Y}(x,y)} \over {f_X(x)}} ,(x,y) , i,j = 1,2,3,...
$$

容易得到二维离散型随机变量联合概率质量函数等价形式：

$$
f_{X,Y}(x,y)=f_{X|Y} (x|y)f_Y(y) = f_{Y|x}(y|x)f_X(x)
$$

由边缘概率质量函数与联合概率质量函数间的数量关系， 得到二维离散型随机质量的**全概率公式**：

$$
f_X(x) = \sum_{j=i}^\infty f_{X,Y}(x,y_j) = \sum_{j=1}^\infty f_{X|Y}(x|y_j)f_Y(y_j)
$$

$$
f_Y(y) = \sum_{i=1}^\infty f_{X,Y}(x_i,y) = \sum_{i=1}^\infty f_{Y|X}(y|x_i)f_X(x_i)
$$


## 应用举例

在某游戏中，玩家通过开宝箱获取宝贝，但是宝箱有 $2\over 3$ 的概率是陷阱， 而玩家有个判断宝箱的技能，但是误判率是 $1 \over 4$, 现需要知道，玩家技能判断是无陷阱，开出宝箱是陷阱的概率。

由上述可得到如下，先验概率：

$$
X=(\omega), 其中 \omega 就是开启宝箱， P(x=陷阱) = 2/3 , \  P(x=无陷阱)=1/3
$$

*在宝箱已知的前提下，玩家用技能去判断*：

$$
Y=(\omega), 其中 \omega就是判断宝箱， P(y=判无|x=陷阱) =1/4 , \ P(y=判有|x=无陷阱) = 1/4
$$


求解 $P(x=陷阱| y=判无)$ 的值：

$$
P(x=陷阱| y=判无) = {{P(x=陷阱，y=判无)}\over {P(y=判无)}}
$$

边缘概率累积和

$$
P(y=判无) = \sum_{x \ all} p(x_i,y=判无)
$$

全概率公式：

$$
P(y=判无) = P(x=陷阱)P(y=判无|x=陷阱) + P(x=陷阱)P(y=判无|x=无陷阱)
$$

最终可以得到：

$$
P(x=陷阱|y=判无) = {{P(x=陷阱，y=判无)} \over {P(x=陷阱)P(y=判无|x=陷阱) + P(x=陷阱)P(y=判无|x=无陷阱)}}
$$

$$
P(x=陷阱，y=判无) = p(y=判无|x=陷阱)p(x=陷阱)
$$


# 贝叶斯滤波

## 贝叶斯公式

对于二维离散型随机变量 $(X,Y)$ , 由其条件概率质量函数与全概率公式，容易得到贝叶斯公式：

$$
f_{X|Y}(x \ | \ y)=\frac{f_{X,Y}(x,y)}{f_Y(y)}=\frac{f_{Y|X}(y \ | \ x)f_X(x)}{\sum\limits_{i=1}^{\infty}f_{Y|X}(y \ | \ x_i)f_X(x_i)}, \quad (x,y) \in \{x_i,y_j\}, i,j=1, 2, 3, ···
$$

对于连续型随机变量有类似关系。

## 先验概率、似然概率与后验概率

在二维连续型随机变量的贝叶斯公式中，有如下定义：
- $f_X(x)$ 被称为**先验概率密度**， 表示根据以往的经验和分析，在本次试验或采样前便可获得随机变量 $X$ 的概率密度；
- $f_{Y|X}(y|x)$ 被称为**似然概率密度**,表示在状态随机变量 $X$ 取值为 $x$ 的条件下，观测随机变量 $Y$ 取值为 $y$ 的概率密度函数， 状态为因，观测为果，**由因推果** 。

- $f_{X|Y}(x|y)$ 被称为**后验概率密度**， 表示在观察随机变量 $Y$ 取值为 $y$ 的条件下， 状态随机变量 $X$ 取值为 $x$ 的概率密度，状态为因 ，观测为果， **由果推因**。

此外，当 $y$ 为定值时， $\eta = [\int_{-\infty}^{+\infty}f_{Y|X}(y|x)f_X(x)dx]^{-1}$ 为常数， 被称为贝叶斯公式的**归一化常数**。

因此，二维连续型随机变量的贝叶斯公式可表示为：

$$
后验概率密度 = \eta \times 似然概率密度 \times 先验概率密度
$$

## 似然概率

上文中提到，似然概率密度函数 $f_Y|X(y | x)$ 表示在状态随机变量 $X$ 取值为 $x$ 的条件下，观测随机变量 $Y$ 取值为 $y$ 的概率密度。似然概率密度函数表征了传感器检测精度，对于给定的状态条件 $X=x$ ，观测结果 $Y=y$ 的概率分布通常有三种模型：

(1)  等可能型

观测值在状态量真值附近呈均匀分布，此时的似然概率密度函数为常数。

(2) 阶梯型

观测值在状态量真值附近呈阶梯分布，此时的似然概率密度函数为分段常数

(3) 正态分布型

观测值在状态量真值附近呈高斯分布，此时的似然概率密度函数为高斯函数：

$$
f_{Y|X}(y|x) = {1 \over {\sigma \sqrt{2\pi}}} e^{-{{y-x}^2\over {2\sigma ^2}}}
$$

若假定似然概率密度函数为高斯函数，此时，似然概率密度函数的均值 $x$ 代表状态量真值，$\sigma$ 代表传感器检测精度范围。若同时假定先验概率密度函数为高斯函数，即：

$$
f_X(x) \sim \mathcal{N}(\mu_1, \ \sigma_1^2), \quad f_{Y|X}(y \ | \ x) \sim \mathcal{N}(\mu_2, \ \sigma_2^2)
$$

则

$$
f_{X|Y}(x \ | \ y) \sim \mathcal{N}(\frac{\sigma_2^2}{\sigma_1^2 + \sigma_2^2}\mu_1+\frac{\sigma_1^2}{\sigma_1^2 + \sigma_2^2}\mu_2, \ \frac{\sigma_1^2\sigma_2^2}{\sigma_1^2+\sigma_2^2})
$$

## 贝叶斯滤波推导

### 问题建模

- 对于某状态量随机变量 $X$ ，从初始时刻 $0$ 开始，对其进行观测，得到 $0 - k$ 时刻的观测值：

$$y_0,y_1,y_2,⋯,y_k$$

求解 $k$ 时刻状态量随机变量 $X_k$
的最优估计 $\hat{x}_k$ 。

- 求解思路

以贝叶斯公式为求解方向，将问题转化为求解状态量随机变量 $X_k$ 后验概率密度函数的期望：

$$
\hat{x}_k = E[f_{X_k}^+(x)]
$$

进而需要求解状态量随机变量 $X_k$ 的先验概率密度函数与似然概率密度函数。我们认为， $k$ 时刻的状态量随机变量 $X_k$ 与且仅与上一时刻的状态量随机变量 $X_{k−1}$ 有关， $k$ 时刻的观测量随机变量 $Y_k$ 与且仅与 $k$ 时刻的状态量随机变量 $X_k$ 有关，其中的数量关系我们分别称之为状态方程与观测方程：

$$
\begin{cases}
    X_k=f(X_{k-1})+Q_k \quad \Rightarrow \color{red}{状态方程} \\
    Y_k=h(X_k)+R_k \quad \Rightarrow \color{red}{观测方程}
\end{cases}
$$

$f(x)$ 被称为状态转移函数， $h(x)$ 被称为观测函数。

对于 $0$ 时刻的初始状态量随机变量 $X_0$ ，认为观测值 $y_0$ 即为其真值，其后验概率密度函数即为其先验概率密度函数。我们可以根据经验知识（建模精度和传感器精度）写出 $0$ 时刻的初始状态量随机变量 $X_0$ 的后验概率密度函数 $f^+_{X_0}(x)$ 、 $k$ 时刻过程噪声随机变量 $Q_k$ 的概率密度函数 $f_{Q_k}(x)$ 和 $k$ 时刻观测噪声随机变量 $R_k$ 的概率密度函数 $f_{R_k}(x)$ 。

- 符号定义

各时刻的状态量随机变量

$$X_0, X_1, X_2, \cdots , X_k $$

各时刻的观测量随机变量

$$Y_0, Y_1, Y_2, \cdots , Y_k $$

各时刻的观测值

$$ y_0, y_1, y_2, \cdots , y_k $$

各时刻的过程噪声随机变量

$$ Q_1, Q_2, \cdots , Q_k $$

各时刻的观测噪声随机变量

$$ R_1, R_2, \cdots , R_k $$

各时刻的过程噪声随机变量概率密度函数

$$ f_{Q_1}(x), f_{Q_2}(x), \cdots , f_{Q_k}(x) $$

各时刻的观测噪声随机变量概率密度函数

$$ f_{R_1}(x), f_{R_2}(x), \cdots , f_{R_k}(x) $$

各时刻的状态量随机变量先验概率密度函数

$$f_{X_0}^-(x), f_{X_1}^-(x), f_{X_2}^-(x), \cdots , f_{X_k}^-(x)$$

各时刻的状态量随机变量后验概率密度函数

$$f_{X_0}^+(x), f_{X_1}^+(x), f_{X_2}^+(x), \cdots , f_{X_k}^+(x)$$

各时刻状态量随机变量与观测量随机变量的似然概率密度函数

$$f_{Y_1|X_1}(y_1 \ | \ x), f_{Y_2|X_2}(y_2 \ | \ x), \cdots , f_{Y_k|X_k}(y_k \ | \ x)$$

- 重要假设

$X_0$ 分别与 $Q_1,Q_2,⋯,Q_k$
相互独立；

$X_0$ 分别与 $R_1,R_2,⋯,R_k$ 相互独立；

$X_{k−1}$ 与 $Q_k$ 相互独立；

$X_k$ 与 $R_k$ 相互独立。

- 重要定理

**条件概率里的条件可以作逻辑推导**。例如

$$
P(X=1 \ | \ Y=2,Z=3)=P(X+Y=3 \ | \ Y=2,Z=3)=P(X+Y=3 \ | \ Y=2,Z-Y=1)
$$

### 预测推导

已知 $0$ 时刻状态量随机变量 $X_0$ 的后验概率密度函数 $ f^+_{X_0}(x) $ ，状态转移函数 $ f(x) $， $1$ 时刻过程噪声随机变量 $Q_1$ 的概率密度函数 $f^−_{Q_1}(x)$ ，求解 $1$ 时刻状态量随机变量 $X_1$ 的先验概率密度函数 $f^−_{X_1}(x)$ 。

类似二维连续型随机变量贝叶斯公式的推导过程，我们从求解 $X_1$ 的先验累积分布函数 $F^−_{X_1}$ 入手。

```math

\begin{aligned}
    F_{X_1}^-(x) & = P(X_1 \leq x) \\
    & = \sum_{u=-\infty}^x P(X_1=u) \quad \Rightarrow \color{red}{化连续为离散无穷小的累加} \\
    & = \sum_{u=-\infty}^x\sum_{v=-\infty}^{+\infty}P(X_1=u \ | \ X_0=v)P(X_0=v) \quad \Rightarrow \color{red}{全概率公式} \\
    & = \sum_{u=-\infty}^x\sum_{v=-\infty}^{+\infty}P[X_1-f(X_0)=u-f(v) \ | \ X_0=v]P(X_0=v) \quad \Rightarrow \color{red}{条件概率里的条件可以作逻辑推导} \\
    & = \sum_{u=-\infty}^x\sum_{v=-\infty}^{+\infty}P[Q_1=u-f(v) \ | \ X_0=v]P(X_0=v) \quad \Rightarrow \color{red}{状态方程} \\
    & = \sum_{u=-\infty}^x\sum_{v=-\infty}^{+\infty}P[Q_1=u-f(v)]P(X_0=v) \quad \Rightarrow \color{red}{X_{k-1}与Q_k相互独立} \\
    & = \sum_{u=-\infty}^x\left\{\lim_{\epsilon \to 0}\sum_{v=-\infty}^{+\infty}f_{Q_1}[u-f(v)]·\epsilon · f_{X_0}^+(v)·\epsilon \right\} \quad \Rightarrow \color{red}{类似二维连续型随机变量贝叶斯公式的推导过程，将点概率化为概率密度与无穷小的乘积} \\
    & = \sum_{u=-\infty}^x\left\{\lim_{\epsilon \to 0}\int_{-\infty}^{+\infty}f_{Q_1}[u-f(v)]f_{X_0}^+(v)\mathrm{d}v·\epsilon \right\} \quad \Rightarrow \color{red}{积分定义} \\
    & = \int_{-\infty}^x \int_{-\infty}^{+\infty}f_{Q_1}[u-f(v)]f_{X_0}^+(v)\mathrm{d}v\mathrm{d}u \quad \Rightarrow \color{red}{积分定义} \\
    & = \int_{-\infty}^x \int_{-\infty}^{+\infty}f_{Q_1}[x-f(v)]f_{X_0}^+(v)\mathrm{d}v\mathrm{d}x \quad \Rightarrow \color{red}{替换自变量符号u为x} \\
\end{aligned}

```

故， $1$ 时刻状态量随机变量 $X_1$ 的先验概率密度函数为：

$$
f_{X_1}^-(x)=\frac{\mathrm{d}F_{X_1}^-(x)}{\mathrm{d}x}=\int_{-\infty}^{+\infty}f_{Q_1}[x-f(v)]f_{X_0}^+(v)\mathrm{d}v
$$

推导完毕。可以发现，先验概率密度函数本质来源于状态方程。

### 更新推导

已知 $1$ 时刻观测量随机变量 $Y_1$ 的取值 $y_1$，求解 $1$ 时刻状态量随机变量与观测量随机变量的似然概率密度函数 $f_{Y_1|X_1}(y_1 \ | \ x)$，并联合预测步得到的 $1$ 时刻状态量随机变量 $X_1$ 的先验概率密度函数 $f_{X_1}^-(x)$，求解 $1$ 时刻状态量随机变量 $X_1$ 的后验概率密度函数 $f_{X_1}^+(x)$ 。

首先，求解似然概率密度函数 $f_{Y_1|X_1}(y_1 \ | \ x)$：

$$
\begin{aligned}
    f_{Y_1|X_1}(y_1 \ | \ x) & =\lim_{\epsilon \to 0}\frac{F_{Y_1 | X_1}(y_1+\epsilon \ | \ x)-F_{Y_1 | X_1}(y_1 \ | \ x)}{\epsilon} \quad \Rightarrow \color{red}{导数的定义} \\
    & =\lim_{\epsilon \to 0}\frac{P(y_1 \le Y_1 \le y_1 + \epsilon \ | \ X_1=x)}{\epsilon} \quad \Rightarrow \color{red}{累积分布函数的性质} \\
    & =\lim_{\epsilon \to 0}\frac{P[y_1-h(x) \le Y_1-h(X_1) \le y_1 - h(x) + \epsilon \ | \ X_1=x]}{\epsilon} \quad \Rightarrow \color{red}{条件概率里的条件可以作逻辑推导} \\
    & =\lim_{\epsilon \to 0}\frac{P[y_1-h(x) \le R_1 \le y_1 - h(x) + \epsilon \ | \ X_1=x]}{\epsilon} \quad \Rightarrow \color{red}{观测方程} \\
    & =\lim_{\epsilon \to 0}\frac{P[y_1-h(x) \le R_1 \le y_1 - h(x) + \epsilon]}{\epsilon} \quad \Rightarrow \color{red}{X_{k}与R_k相互独立} \\
    & =\lim_{\epsilon \to 0}\frac{F_{R_1}[y_1 - h(x) + \epsilon]-F_{R_1}[y_1 - h(x)]}{\epsilon} \quad \Rightarrow \color{red}{累积分布函数的性质} \\
    & =f_{R_1}[y_1-h(x)] \quad \Rightarrow \color{red}{导数的定义}
\end{aligned}
$$

可以发现，似然概率密度函数本质来源于观测方程。

然后，联合预测步得到的 $1$ 时刻状态量随机变量 $X_1$ 的先验概率密度函数 $f_{X_1}^-(x)$，求解 $1$ 时刻状态量随机变量 $X_1$ 的后验概率密度函数 $f_{X_1}^+(x)$：

$$
f_{X_1}^+(x)=\eta_1·f_{Y_1|X_1}(y_1 \ | \ x)·f_{X_1}^-(x)=\eta_1·f_{R_1}[y_1-h(x)]·f_{X_1}^-(x)
$$

其中，归一化常数 $\eta_1$ 为：

$$
\eta_1=\left[\int_{-\infty}^{+\infty}f_{Y_1|X_1}(y_1 \ | \ x)f_{X_1}^-(x)\mathrm{d}x\right]^{-1}=\left \lbrace \int_{-\infty}^{+\infty}f_{R_1}[y_1-h(x)]f_{X_1}^-(x)\mathrm{d}x\right\rbrace ^{-1}
$$


### 递推流程

由预测步和更新步的推导结果，可得到由 $0$ 时刻状态量随机变量 $X_0$ 的后验概率密度函数 $f_{X_0}^+(x)$ 到 $k$ 时刻状态量随机变量 $X_k$ 的后验概率密度函数 $f_{X_0}^+(x)$ 的递推流程：

$$
\begin{aligned}
    f_{X_0}^+(x) & \stackrel{预测}{\Longrightarrow} f_{X_1}^-(x)=\int_{-\infty}^{+\infty}f_{Q_1}[x-f(v)]f_{X_0}^+(v)\mathrm{d}v \stackrel{观测更新}{\Longrightarrow} f_{X_1}^+(x)=\eta_1·f_{R_1}[y_1-h(x)]·f_{X_1}^-(x) \\
    & \stackrel{预测}{\Longrightarrow} f_{X_2}^-(x)=\int_{-\infty}^{+\infty}f_{Q_2}[x-f(v)]f_{X_1}^+(v)\mathrm{d}v \stackrel{观测更新}{\Longrightarrow} f_{X_2}^+(x)=\eta_2·f_{R_2}[y_2-h(x)]·f_{X_2}^-(x) \\
    & \cdots \\
    & \stackrel{预测}{\Longrightarrow} f_{X_k}^-(x)=\int_{-\infty}^{+\infty}f_{Q_k}[x-f(v)]f_{X_{k-1}}^+(v)\mathrm{d}v \stackrel{观测更新}{\Longrightarrow} f_{X_k}^+(x)=\eta_k·f_{R_k}[y_k-h(x)]·f_{X_k}^-(x)
\end{aligned}
$$

其中，归一化常数 $\eta_1$ 为：

$$
\eta_k=\left \lbrace \int_{-\infty}^{+\infty}f_{R_k}[y_k-h(x)]f_{X_k}^-(x)\mathrm{d}x\right \rbrace^{-1}
$$

最终，可得到 $k$ 时刻状态量随机变量 $X_k$ 的最优估计 $\hat{x}_k$：

```math
\hat{x}_k=E[f_{X_k}^+(x)]=\int_{-\infty}^{+\infty}xf_{X_k}^+(x)\mathrm{d}x
```

## 完整算法框架

- 1 设初值

初始 $0$ 时刻状态量随机变量 $X_0$ 的后验概率密度函数：

$$f_{X_0}^+(x)$$

- 2 预测

$k$ 时刻状态量随机变量 $X_k$ 的先验概率密度函数：

$$f_{X_k}^-(x)=\int_{-\infty}^{+\infty}f_{Q_k}[x-f(v)]f_{X_{k-1}}^+(v)\mathrm{d}v$$

- 3 更新

$k$ 时刻状态量随机变量 $X_k$ 的后验概率密度函数：

$$f_{X_k}^+(x)=\eta_k·f_{R_k}[y_k-h(x)]·f_{X_k}^-(x)$$

归一化常数 $η_k$：

$$
\eta_k=\left \lbrace\int_{-\infty}^{+\infty}f_{R_k}[y_k-h(x)]f_{X_k}^-(x)\mathrm{d}x\right \rbrace^{-1}
$$

- 4 求解最优估计

$k$ 时刻状态量随机变量 $X_k$ 的后验估计：

```math
\hat x_k^+ = E[f_{X_k}^+(x)] = \int _{-\infty} ^ {+\infty} xf_{X_k} ^ + (x)dx
```

## 贝叶斯滤波的缺点以及解决方法

- 缺点

从上文的推导及结论中可以发现，求解预测步中的先验概率密度函数 $f_{X_k}^-(x)$、更新步中的归一化常数 $η_k$、最终的最优估计 $\hat{x}_k$ 时均涉及到无穷积分，而大多数情况无法得到解析解，使得贝叶斯滤波算法的直接应用十分困难。

- 解决方法

为了解决贝叶斯滤波中的无穷积分问题，通常从两个角度出发：

1. 作理想假设

- 假设状态转移函数 $f(x)$ 和观测函数 $h(x)$ 均为线性函数，过程噪声随机变量 $Q_k$ 和 观测噪声随机变量 $R_k$ 均服从均值为 $0$ 的正态分布——卡尔曼滤波（Kalman Filter）
- 假设状态转移函数 $f(x)$ 和（或）观测函数 $h(x)$ 为非线性函数，过程噪声随机变量 $Qk$ 和 观测噪声随机变量 $Rk$ 均服从均值为 $0$ 的正态分布——扩展卡尔曼滤波（Extended Kalman Filter）和无迹卡尔曼滤波（Unscented Kalman Filter）

2.  化连续为离散

将无穷积分转化为数值积分，一般有以下方法：

    高斯积分（不常用）
    蒙特卡罗积分（粒子滤波，Particle Filter）
    直方图滤波

针对本节内容中提到的卡尔曼滤波、扩展卡尔曼滤波、无迹卡尔曼滤波、粒子滤波、直方图滤波等常用滤波算法，将在后续文章中进行详细展开讨论。


# 参考

[从概率到贝叶斯滤波](https://blog.shipengx.com/archives/9fb25cec.html)


[MarkDown数学公式](https://zhuanlan.zhihu.com/p/441454622)