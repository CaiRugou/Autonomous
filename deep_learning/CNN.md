# 卷积网络（CNN）解析

## Filter

CNN常用于图像识别，在深度学习中，我们将图片转为像素矩阵再输入到算法中，**对于黑白的图片**，每一个点只有一个像素值， 如果是**彩色**的，每个点就会有三个像素值(RGB)。

连接运算就是做矩阵的点乘运算，如果是图像的话，其实就是用 kernel (filter)来与像素矩阵局部做乘积，如下图，output的第一个阴影值就是input和kernel的阴影部分进行矩阵乘法所得：

![image](https://github.com/CaiRugou/Autonomous/blob/main/img/sample_cnn.png)

接下来引入一个参数 (stride) 步长， 代表我们每一次 卷积核（滤波器）在像素矩阵上移动的步长，分为水平步长和垂直步长，如下图水平步长为2，垂直步长为3的的过程：

![image](https://github.com/CaiRugou/Autonomous/blob/main/img/stride.png)
