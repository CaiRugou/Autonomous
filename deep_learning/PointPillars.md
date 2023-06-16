# PointPillars

- 对于3D稀疏点云数据如何在不损失特征的同时加快卷积计，该算法提供一种思路，将点云划分为**pillars**，构成一种**伪图片**的数据然后再做检测。

- pillars （柱）相当于在 $(x,y)$ 平面上先做栅格，然后再按照栅格回填 $z$ 的值，这个栅格如何建立，按照论文应该是依据 多线激光雷达的探测范围和分辨率来划分。

整个过程如下图所示：

![image](https://github.com/CaiRugou/Autonomous/blob/main/img/pointpillars.png)

