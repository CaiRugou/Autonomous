
## 背景

在对物体进行跟踪的过程中,我们需要对物体下一时刻的位置,运动状态进行预测. 预测总是基于一定的规则, 对于车辆而言, 运动模型有CV(constant velocity), CA(constant acceleration), CTRV(constant turn rate and velocity) ，IMM(Interacting Multiple Model) 等等. 每次预测之后，我们需要使用合适的滤波算法，用测量值对预测值进行更新，获得一个更为精确的运动信息。常用的滤波算法包括卡尔曼滤波，粒子滤波、贝叶斯滤波等等。其中卡尔曼滤波包括线性卡尔曼，非线性的扩展卡尔曼（EKF）以及无损卡尔曼(UKF)等等。

## CV/CA线性模型与卡尔曼

运动模型中最为简单的就是恒定速度（Constant Velocity），恒定加速度（Constant Acceleration）模型。

- CV:

$$
\vec X_t = (x,y,v_x,v_y)^T
$$

状态转移函数：

```math
\vec X_{t+1} = 
\left [ \begin{array} {}

x_t+v_x*\Delta t \\
y_t+v_y*\Delta t \\
v_x \\
v_y

\end{array} \right]
```

状态转移矩阵：

```math
A = 
\left[ \begin {array} {}
1,0,\Delta t,0 \\
0,1,0,\Delta t  \\
0,0,1,0 \\
0,0,0,1

\end{array} \right]
```

- CA

状态转移函数：

```math
\vec x_{t+1} = 

\left[ \begin {array} {}
x_t + v_x * \Delta t + {1\over 2}*a_x*\Delta t^2 \\
\\
y_t + v_y * \Delta t + {1\over 2}*a_y*\Delta t^2  \\
\\
v_x + \Delta t * a_x \\
\\
v_y + \Delta t * a_y \\
a_x \\
a_y

\end{array} \right]

```

状态转移矩阵：

```math
A = 
\left[ \begin {array} {}
1,0,\ \Delta t,\ 0, \ 0.5*\Delta t^2, \ 0 \\
\\
0,1,0,\ \Delta t, \ 0, \  0.5*\Delta t^2  \\
\\
0, \ 0, 1, 0,\ \  \Delta t , 0 \\
\\
0, \  0,0,1,0, \ \  \Delta t

\end{array} \right]
```

对于线性模型直接使用线性卡尔曼滤波。

## 恒定转率和速度模型(CTRV) 和 恒定转率和加速度(CTRA)

 ![image](https://github.com/CaiRugou/Autonomous/blob/main/img/ctrv.jpg) 

 状态量：

 $$
 X = (x, y, v, \theta , \omega)
 $$

$\theta$ 是航向角-追踪目标和本车坐标系的 $x$ 轴的夹角， $\omega$ 是偏航角速度

状态方程：

```math
X_{k+1} = X_k + \int ^{t_{k+1}}_{t_k}

\left [ \begin{array} {}

x{'} \\
y{'} \\
v{'} \\
\theta{'} \\
\omega{'}

\end{array} \right] dt =
\left [ \begin{array} {}

x_k \\
y_k \\
v_k \\
\theta_{k} \\
\omega_{k}

\end{array} \right] +

\left [ \begin{array} {}

\int vcos(\theta)dt \\
\int vsin(\theta)dt \\
0 \\
\omega . \Delta t \\
0

\end{array} \right]

```

结合过程噪声，完整的状态方程：其中包含径向方向的速度 $v$ 的加速度 $a_{v,k}$, 偏航角加速度 $\omega$ 的加速度 $a_{\omega ,k}$ 。

得到状态转移函数 $g(x(t),u)$ :

![image](https://github.com/CaiRugou/Autonomous/blob/main/img/ctrv_g.jpg) 

对于非线性关系的模型，可以使用 扩展卡尔曼和无损卡尔曼。


## IMM

IMM模型最直观的思想是如果任意模型都不能够描述目标全部的运动可能，那我们就用多集中模型来对目标状态进行估计，并通过一定的方式将这些模型的结果按照一定的比例相融合，从而获得适应性更广的估计模型。

1、在IMM算法中共有 $n$ 个系统模型；

2、算法的迭代是离散的， $k$ 表示离散时间；

3、每时刻，我们能够得到的测量为 $z$；

4、模型之间存在相关性，可以用一个相关性矩阵
 $P$ 表示；

5、每个模型对目标当前状态的描述的正确性的概率为 $u_i$ 。即表示模型在所有模型中的精确性的占比。

## 混合估计

在 $K-1$ 时刻，每个模型都有一个最优估计 $\hat x _{i,k-1}$ , 以及协方差估计 $\hat P_{i, k-1}$ (表示第 $i$ 个模型，在 $k-1$ 时刻的最优估计)，再根据IMM的滤波中的正确性的概率，可以将每个模型的估计状态和协方差矩阵进行**加权融合**, 最终得到目标在 $k-1$ 时刻的最优状态以及协方差矩阵：

$$
\hat x_{j0,k-1} = \sum _{i=1}^n \lambda _{ij,k-1} \hat x _{i,k-1}
$$

```math
\hat P_{j0,k-1} = \sum _{i=1}^n \lambda _{ij,k-1} [\hat P_{i,k-1} + (\hat x_{j,0,k-1} - \hat x_{i,k-1})(\hat x_{j,0,k-1} - \hat x_{i,k-1})^T]
```

其中 $\lambda _{ij,k-1}$ 为**融合的相关性系数**。是通过预先设定的模型转移概率矩阵 $P$ ，第 $i$ 行表示所有模型（包含本身）与 第 $i$ 个模型的关系，

因此，根据相关性，对模型进行带权融合从而获得第 $i$
个模型的融合估计状态及协方差矩阵，而每个模型本身又有一个正确性概率，因此需要对 $P$
的每个元素乘上对应的正确性概率 $u_{i,k-1}$
后，再进行归一化处理得到 $\lambda _{ij,k-1}$
：

```math
\lambda _{ij,k-1} = {{p_{ij}u_{i,k-1}} \over {{\sum{^n}_{i=1}} p_{ij}u_{i,k-1}}}
```

例如：

有模型 $CV, CA, CTRV$ , 三个模型，模型转移矩阵 $P$ 如下：

```math
P_{ij} =
\left [ \begin{array} {}

0.9 & 0.05 & 0.05 \\
0.05 & 0.9 & 0.05 \\
0.05 & 0.05 & 0.9 

\end{array} \right]
```

每个模型的正确性概率

$$
u_i = \left [ \begin{array} {} 0.8 & 0.1 & 0.1\end{array} \right]
$$

使用模型 $j=1$ 的预测概率(包含所有模型转移到该模型)：

$C_{j=1} = {\sum^n_{i=1}} p_{i,j=1}u_i$

$=p_{1,1}u_1 + p_{2,1}u_2 + p_{3,1}u_3$

$=0.9*0.8 + 0.05*0.1 + 0.05*0.1$

模型 $i$ 转移到模型 $j=1$ 的混合概率为：

- 由模型 $i=1$ 转移到模型 $j=1$ 的混合概率：

$u_{i=1,j=1} = {{p_{i=1,j=1}u_{i=1}} \over C_{j=1}}$
  
  可以计算得到：

$u_{11} = {{0.9 * 0.8} \over {0.9*0.8 + 0.05*0.1 + 0.05*0.1}}$

- 由模型 $i=2$ 转移到模型 $j=1$ 的混合概率：

$u_{i=2,j=1} = {{p_{i=2,j=1}u_{2=1}} \over C_{j=1}}$

  可以计算得到：

$u_{21} = {{0.05 * 0.1} \over {0.9*0.8 + 0.05*0.1 + 0.05*0.1}}$

- 由模型 $i=3$ 转移到模型 $j=1$ 的混合概率：

$u_{i=3,j=1} = {{p_{i=3,j=1}u_{3=1}} \over C_{j=1}}$

  可以计算得到：

```math
  u_{31} = {{0.05 * 0.1} \over {0.9*0.8 + 0.05*0.1 + 0.05*0.1}}
```

- 最终得到模型 $j=1$ 的混合状态估计为：
  
```math
  \hat X_{0j=1} = \hat X_1 u_{11} + \hat X_2u_{21} + \hat X_3u_{31}
```

- 最终得到模型 $j=1$ 的混合协方差估计为：

```math
P_{0j=1} = 
(u_{11}[P_1 + (\hat x_{0j}-\hat x_{1})(\hat x_{0j} - \hat x_1)^T]) +

 (u_{21}[P_2 + (\hat x_{0j}-\hat x_{2})(\hat x_{0j} - \hat x_2)^T]) +

 (u_{31}[P_3 + (\hat x_{0j}-\hat x_{3})(\hat x_{0j} - \hat x_3)^T])
```


### 使用相关卡尔曼滤波算法预测更新

根据前两章介绍的线性卡尔曼滤波和非线性卡尔曼滤波算法，我们可以得到各模型更新后的估计目标状态 $\hat x_{i,k}$
和协方差矩阵 $\hat P _{i,k}$ 。

#### 计算各个模型的似然函数值

通过上述的滤波算法可以得到测量误差以及测量的协方差矩阵，符合高斯分布 $N  (y_{i,k}, S_{i,k})$:

$y_{i,k} = z_k - h_i (\hat x_{i0,k})$

$S_{i,k} = H_{i,k}\hat P_{i0,k}H_{i,k}^T + R_{i,k}$

当每个模型都估计出目标状态口，根据估计出的状态来评价模型对当前状态估计的好坏，如下：

```math
\Lambda _{i,k} = {1 \over {\sqrt{(2\pi)^N |S_{i,k}|}}}e^{-{1\over 2} y_i^TS_{i,k}^{-1}y_{i,k}}
```

### 更新每个模型的可信度（准确率）

根据计算的 $k$ 时刻的每个模型的似然值，可以得到每个模型描述的准确程度，从而实现模型可信度的更新:

$$
u_{i,k} = {{u_{i,k-1} \Lambda_{i,k}} \over {C}}
$$

其中 $u_{i,k-1}$ 上一时刻融合后的可信度， $C$ 是 $u_{i,k}$ 归一化系数：

```math
u_{i,k-1} = \sum _{i=1}^n p_{i,j}u_{i,k-1}
```

```math
C = \sum _{i=1}^n u_{i,k-1} \Lambda _{i,k}
```

### 多模型估计融合最优估计


根据更新后的置信度 $u_i$ 可以依次将模型融合起来，得到 $IMM$ 滤波算法的输出目标状态及协方差矩阵：

```math
\hat x_k = \sum _{i=1} ^n u_{i,k}\hat x_{i,k}
```

```math
\hat P_k = \sum _{i=1}^n u_{i,k} [\hat P_{i,k} + (\hat x_k-\hat x_{i,k})(\hat x_k-\hat x_{i,k})^T]
```

### $IMM$ 的五个步骤

- 混合模型估计
- 滤波算法
- 计算模型似然
- 更新模型可信度
- 多模型融合估计

