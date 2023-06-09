# PointPillars

- 对于3D稀疏点云数据如何在不损失特征的同时加快卷积计，该算法提供一种思路，将点云划分为**pillars**，构成一种**伪图片**的数据然后再做检测。

- pillars （柱）相当于在 $(x,y)$ 平面上先做栅格，然后再按照栅格回填 $z$ 的值，这个栅格如何建立，按照论文应该是依据 多线激光雷达的探测范围和分辨率来划分。

整个过程如下图所示：

![image](https://github.com/CaiRugou/Autonomous/blob/main/img/pointpillars.png)

## 点云伪图片化
具体步骤如下：

- 1. 按照点云数据所在的X,Y轴将点云划分为一个个的**网格**，总共大小为 $H * W$ ， 凡是落入到同一个网格的点云数据被视为在同一个 **pillar**里。

- 2. 每个点云用一个 $D=9$ 维的向量表示，分别为 $(x,y,z,r,x_c,y_c,z_c,x_p,y_p)$  。其中 $x,y,z,r$ 表示点云的坐标和反射强度； $x_c,y_c,z_c$ 为该点云所在的pillar中所有点得**几何中心**; $x_p = x -x_c ,  y_p = y- y_c$ ,反应的是点云到几何中心的**相对位置**。

- 3. 假设每个样本有 $P$ 个非空的pillars， 每个pillar中包含 $N$ 个点云数据，最终可以使用 $(D,P,N)$ 的张量来描述点云样本。 对于不满 $N$ 个点云的 pillar 使用 $0$ 填充； 对于多于 $N$ 个点云的pillar则采用随机采样出 $N$ 个点云。

- 4. 这样我们就将点云样本通过上边的步骤就完成了**张量化**，再利用简化版的 $PointNet$ 对张量化的点云数据进行处理和 $特征提取$ 。

- 5. 特征提取可以理解为对点云的维度进行处理，将原来的 $D=9$ ，处理为 $C$ 维，这样我们就得到一个 $(C,P,N)$ 的张量。

- 6. 按照pillar所在维度，对 $N$ 做 $max pool$ 操作，就可以得到一个 $C,P$ 维度的特征图。

- 7. 在得到 $(C,P)$ 的特征图后，按照 1 中的 栅格将 $P$ **散射**回对应的栅格中，就形成了 $(C, H, W)$ 的**伪图片**。至此点云的 **伪图片**化完成。


## PointPillars 网络结构

- 1 PFN (Pillar Feature Net)  就是伪图片特征提取过程。

- 2 Backbone 通过 2D CNN 的两层网络，第一个网络 **不断缩小特征图的分辨率**，同时 **提升特征图的维度**， 最终可以获得**三个**不同分辨率的特征图； 第二个网络对**三个特征图**进行上采样到相同的大小（分辨率）。然后再进行连接。

- 3 选择这样的主干网络，是因为**不同分辨率的特征图负责不同大小物体的检测**。比如分辨率大的特征图**感受野**较小，适合捕捉小物体。

以上就是PointPillars的整个网路结构。

## 损失函数

PointPillars的损失函数，作者采用了和SECOND中类似的损失函数，每个3D BBOX用一个 $7$ 维的向量表示，分别为 $(x,y,z,w,h,l,\theta)$  。
其中， $(x,y,z)$ 为box的中心， $w,h,l$ 是box的尺寸， $\theta$ 为方向角(主方向)。

那么检测框回归任务重要学习的参数为这 $7$ 个变量的**偏移量**。

```math

\Delta x = {{x^{gt} - x^a} \over d^a} \\

\Delta y = {{y^{gt} - y^a} \over d^a} \\

\Delta z = {{z^{gt} - z^a} \over d^a} \\

\Delta w = log {w^{gt}\over w^a} \\

\Delta l = log {l^{gt}\over l^a} \\

\Delta h = log {h^{gt}\over h^a} \\

\Delta \theta = sin(\theta ^{gt} - \theta ^a)

```

作者采用了 $Smooth \ L1$ 损失函数进行训练：

```math
L_{loc} = \sum _{b \in (x,y,z,w,l,h,\theta)} SmoothL1(\Delta b)
```

为了避免方向判别错误，引入了Softmax损失 **学习物体的方向** 。 该方向记作 $L_{dir}$ 。

分类损失函数采用了 Focal Loss, 定义如下：

```math
L_{cls} = -\alpha _a (1-p^a)^{\gamma} log p^a
```

最终总函数定义如下：

```math
L = {1\over N}(\beta _{loc}L_{loc} + \beta _{cls}L_{cls} + \beta_{dir}L_{dir})
```
