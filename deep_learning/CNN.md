# 卷积网络（CNN）解析

## Tensor
张量和空间本质上没有区别，只是不同的叫法而已。建议摒弃掉，标量、向量、矩阵等等传统叫法，统一用 $n$ 维度的张量来代替它们。

## Filter

CNN常用于图像识别，在深度学习中，我们将图片转为像素矩阵再输入到算法中，**对于黑白的图片**，每一个点只有一个像素值， 如果是**彩色**的，每个点就会有三个像素值(RGB)。

连接运算就是做矩阵的点乘运算，如果是图像的话，其实就是用 kernel (filter)来与像素矩阵局部做乘积，如下图，output的第一个阴影值就是input和kernel的阴影部分进行矩阵乘法所得：

![image](https://github.com/CaiRugou/Autonomous/blob/main/img/sample_cnn.png)

接下来引入一个**参数 (stride) 步长**， 代表我们每一次 卷积核（滤波器）在像素矩阵上移动的步长，分为水平步长和垂直步长，如下图水平步长为2，垂直步长为3的的过程：

![image](https://github.com/CaiRugou/Autonomous/blob/main/img/stride.png)

在卷积核不断地滑过输入像素矩阵，所覆盖之处做矩阵的点积，完成点积之后会形成新的矩阵并且得到新的shape。

假设input矩阵的shape是 $32 * 32$ , stride为 $1$ ，filter shape为 $4*4$ ,那么经过卷积计算之后的 shape为 $29 * 29$ ， 计算公式是：

$$
{(input \ shape - filter  \ shape) \over stride} + 1
$$

在深度学习中**务必要掌握每一层的输入和输出**。

如果stride改为 $3$ ，套用上述公式 $(32 - 4)/3 + 1$ 无法得到一个整数，所以这样设定就是错误的，我们的卷积核的size是固定的，如果我们想得到一个正确的结果，只能通过给input外围打上padding 来改变 input 的size， 但是我们又不想去改变input的特征信息，所以我们用 $0$ 去填充padding的值， 这里我们将padding的大小设为 $1$ ， 在矩阵的外围加上大小为 $1$ 的padding， 则矩阵的长和宽分别增加 $2*1$ , 再代入到上式中计算，得到 output的 shape 为 $(32 + 2 *1 - 4)/3 + 1 = 11$ , 所以最终 output的shape就是 $11 * 11$。
