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

如果stride改为 $3$ ，套用上述公式 $(32 - 4)/3 + 1$ 无法得到一个整数，所以这样设定就是错误的，我们的卷积核的size是固定的，如果我们想得到一个正确的结果，只能通过给input外围打上padding 来改变 input 的size， 但是我们又不想去改变input的特征信息，所以我们用 $0$ 去填充padding的值， 这里我们将padding的大小设为 $1$ ， 在矩阵的外围加上大小为 $1$ 的padding， 则矩阵的长和宽分别增加 $2*1$ , 再代入到上式中计算，得到 output的 shape 为 $(32 + 2 *1 - 4)/3 + 1 = 11$ , 所以最终 output的shape就是 $11 * 11$ 。

接下来引入另外一个重要的参数 **通道（channel）或者叫 深度（depth）**， 上述的黑白图像就是单个通道，如果是一张彩色的图像那么就有三原色 **RGB** 三个通道上的像素矩阵， 也就是说每个像素都有3个值组成。举例说明， 一张照片的像素矩阵的张量是 $H * W * D$ 也就是 $32 * 32 * 3$ ， 我们的卷积核设置为 $3 * 3 * 3$ ， 我们使用卷积运算，通过卷积核的每一个深度和输入矩阵的每个深度的矩阵运算，最后得到一个深度为 $3$ 的输出，假设这里 stride设置为 $1$ ， padding 为 $0$ ,根据上述公式可以计算得到输出feature map的长和宽 $(32  -3)/1 +1 = 30$ ， 深度为 $3$ ， 最后得到输出的 feature maps 为 $30 * 30 * 3$ 。

卷积的时候是用多个 filter完成的，一般经过卷积之后的output shape的通道（深度）就是filter的数量，例如下图，我们将深度为 $2$ 的卷积核作为一个 filter，就是将 输出结果进行相加，将深度信息进行压缩。 如果是多个filter放在一起，那么最后的深度就是filter的数量：

![image](https://github.com/CaiRugou/Autonomous/blob/main/img/filter.png)


训练参数：因为每个特征图对应1个卷积核及1个偏置，每个卷积核的可调整权重是 $3×3$ 个，加上偏置1个，一共3个特征图，因此可调整参数是 $3 ×（3×3+1）=30 $ 个。

1.卷积的意义是什么呢？


其实如果用图片处理上的专业术语，被叫做锐化，卷积其实强调某些特征，然后将特征强化后提取出来，不同的卷积核关注图片上不同的特征，比如有的更关注边缘而有的更关注中心地带等等.

2.如何使得不同的卷积核关注不同的地方？


设置filter矩阵的值，比如input shape是4 * 4的，filter是2 * 2，filter是以一个一个小区域为单位，如果说我们想要关注每一个小区域的左上角，那么将filter矩阵的第一个值设为1，其他全为0即可


总结来说，就是通过不断改变filter矩阵的值来关注不同的细节，提取不同的特征


3.filter矩阵里的权重参数是怎么来的？


首先会初始化权重参数，然后通过梯度下降不断降低loss来获得最好的权重参数


4.常见参数的默认设置有哪些？


一般filter的数量（output channels）通常可以设置为2的指数次，如32，64，128，512，这里提供一组比较稳定的搭配（具体还得看任务而定），F（kernel_size/filter_size）= 3，stride = 1，padding = 1；F = 5，stride = 1，Padding = 2;F = 1，S = 1，P = 0

5.参数数量？


举例来说，filter的shape为5 * 5 * 3 ，一共6个，stride设置为1，padding设为2，卷积层为(32 * 32 * 6)，注意卷积层这里是代表最后的输出shape，输入shape为 32 * 32 * 3，那么所需要的参数数量为 6 * (5 * 5 * 3 + 1)，里面 +1 的原因是原因是做完点积运算之后会加偏置（bias），当然这个参数是可以设置为没有的


6.1 x 1 卷积的意义是什么？


filter的shape为1 x 1，stride = 1，padding = 0，假如input为32 * 32 * 3，那么output shape = (32 - 1) / 1 + 1 = 32，换言之，它并没有改变原来的shape，但是filter的数量可以决定输出通道，所以，1 x 1的卷积目的是改变输出通道。可以对输出通道进行升维或者降维，降维之后乘上的参数数量会减少，训练会更快，内存占用会更少。

另外，其实1 x 1的卷积不过是实现多通道之间的线性叠加，如果你还记得上面多通道的意思，1 x 1 卷积改变卷积核的数量，无非就是使得不同的feature map进行线性叠加而已（feature map指的是最后输出的每一层叠加出来的），因为通道的数量可以随时改变，1 x 1卷积也可以有跨通道信息交流的内涵。

## 池化

卷积好之后会用RELU进行激活，当然，这并不会改变原来的shape，这样可以增加模型的非线性兼容性，如果模型是线性的，很容易出问题，如XOR问题，接下来进行池化操作（Pooling），常见的是MaxPooling（最大池化），它基本上长得跟filter一样，只不过功能是选出区域内的最大值。假如我们的shape是4 * 4 ，池化矩阵的shape是2 * 2，那么池化后的shape是2 * 2（4 / 2）


那么，池化的意义是什么？池化又可以被成为向下取样（DownSample），经过池化之后shape会减小不少，如果说卷积的意义是提取出特征，那么，池化的意义是在这些特征中取出最有代表性的特征，这样可以降低像素的重复性，使得后续的卷积更有意义，同时可以降低shape，使得计算更为方便


当然，也还有平均池化（AveragePooling），这样做试图包含区域内的所有的特征，那么，如果图片相邻色素重复很多，那么最大池化是不错的，如果说一张图片很多不同的特征需要关注，那么可以考虑平均池化


补充一下，可以给上述池操作加一个Global，这就意味着全局，而不是一个一个的小区域

## 案例LeNet-5

LeNet-5网络的整体结构，不包括输入层，包括输出层，共七层。（下图） 

![image](https://github.com/CaiRugou/Autonomous/blob/main/img/lenet.png)

LeNet-5网络的结构：（参考上图）

（1）输入层：网络输入为 $32×32×1$ 的单通道图像（黑白图像）。

（2）C1层，卷积层： 卷积层的作用是提取图像的特征，卷积神经网络中，**较前的卷积层**，主要是提取图像的**细节特征**；**较后的卷积层**，主要是提取图像的**全局特征**。

- 特征图个数：这一层得到的特征图是 $6$ 个，分别由 $6$ 个不同的卷积核对输入层图像进行卷积得到。

- 卷积核个数和大小： $6$  个不同的卷积核，大小均为 $5×5$ 。

- 特征图大小：均为 $28×28$ 。因为，<font color="#660000">特征图的宽度=(输入图像宽-卷积核宽)/步长 +1 </font><br/>，即$32-5+1=28$ ；其高度的计算方法相同，所以，每个特征图大小均为 $28×28$ 。

- 可训练参数：可训练参数是 $156$ 个。因为每个特征图对应1个卷积核及1个偏置，每个卷积核的可调整权重是 $5×5$ 个，加上偏置1个，一共6个特征图，因此可调整参数是 $6×（5×5+1）=156$ 个。

（3）S2层，池化层（下采样层或汇集层）：

池化层的作用是降低数据维度。

- 特征图个数：特征图仍为 $6$ 个。分别由 $6$ 个池化窗口对上层的 $6$ 个特征图进行池化得到。

- 池化窗口个数和大小：共 $6$ 个池化窗口，均为 $2×2$ ，步长为 $2$ 。

- 特征图大小：均为 $14×14$ 。

具体池化过程是，在输入特征图中 $2×2$ 的范围内将四个输入值相加，然后乘以一个权重，加上一个偏置，最后经过 $sigmoid$ 激活函数得到。

- 可训练参数：共 $12$ 个可训练参数。因为有 $6$ 个池化窗口，每个池化窗口都有一个权重，一个偏置，所以共 $6×2=12$ 个可训练参数。

（注：一般的卷积网络的池化过程，往往不加可调整的参数，LeNet-5网络这里设计的比较复杂一些。）

（4）C3层，卷积层：

- 特征图个数：本层得到特征图为 $16$ 个。分别由 $60$ 个不同的卷积核（通过连接表构造），对上一层（S2层）的特征图，按下述“连接表”的设定，经过卷积操作得到。

- 卷积核个数和大小： $60$ 个不同的卷积核，大小均为 $5×5$ 。（如果不用连接表，卷积核是 $16×6=96$ 个）

- 特征图大小：均为  $（14-5+1）×（14-5+1）=10×10$ 。

- 本层与上一层（C3层）的连接方法（连接表）：

一般情况下，如果没有特别的设定，**卷积层的每一个特征图，都依赖于上一层的全部的特征图**，相当于本**卷积层的特征图与上一层的特征图是<font color="#660000">全连接的关系</font><br/>**. 具体地说，卷积层的每一个特征图，都是由上一层的所有的特征图，通过不同卷积核的卷积操作，然后加起来得到的。

请见下面的示意性算式：

![image](https://github.com/CaiRugou/Autonomous/blob/main/img/connect_table.png)

其中， $x_1 , x_2 , x_3 , ... x_6 $ , 是上一层的 $6$ 个特征图； $w_{i,j} , (i=1,2,3,...,16; j =1,2,3,...,6) $ 是不同的 $5*5$ 的卷积核矩阵， 共 $16*6 = 96$ 个。

但是，这种**全连接关系不是必须的**。可以让本层的每个特征图都仅依赖上一层少数几个特征图，以简化结构，减少计算量。为此，LeNet-5定义了一个连接表，指定了本层特征图和上一层特征图之间的连接关系。

**全连接举例**，如下图是全连接的例子：

![image](https://github.com/CaiRugou/Autonomous/blob/main/img/cnn_example.png)

LeNet-5设计的连接表，给出了本层（C3）的16个特征图与上一层S2的6个特征图的连接方法，见下图：

![image](https://github.com/CaiRugou/Autonomous/blob/main/img/lenet_connect.png)

LeNet-5将S2层（上一层）的六个特征图，分别编号为 $0-5$ ，将C3层（本层）的16个特征图分别编号为 $0-15$ 。

上图表示的连接方法是：

C3层编号为 $0$ 的特征图，是用 $96$ 卷积核中的三个不同的卷积核，分别与S2的编号为 $0、1、2$ 的特征图卷积后求和，再加上一个偏置，然后经过 $sigmoid$ 激活函数得到。

同样，C3层的其他15个特征图，与S2层特征图的连接方法，都如上图所示，计算方法也相同。

总之，C3层编号0-5的特征图是分别与S2的3个特征图连接；C3层编号 $6-14$ 的特征图，分别与S2的4个特征图连接，C3层编号15的特征图，与S2的6个特征图连接。

- 可训练参数：共 $1516$ 个可训练参数
根据上图的连接表可以看到：

   C3中 16个特征图（feature maps）中， 有 $6$ (编号0-5)个特征图使用了 $3$ 个 $5×5$ 的卷积核，再加一个偏置， 所以参数个数为 $6×(5×5×3 + 1)$ 。

   以此类推，编号（6-14）的 $9$ 个特征图用了 $4$ 个卷积核的参数， $9×(5×5×4 + 1)$

   编号15的 $1$ 个特征图用了 $6$ 个卷积核的参数， $1×(5×5×6 + 1)$

一共用了60个5×5的卷积核，每个特征图加上一个偏置，可训练参数为（$5×5×3+1）×6+（5×5×4+1）×9+（5×5×6+1）×1=1516$ 个

（5）S4层，池化层：

- 特征图个数：特征图仍为 $16$ 个。**分别**由 $16$ 个池化窗口对上层的 $16$ 个特征图进行池化得到。

- 池化窗口个数和大小：共16个池化窗口，均为2×2，步长为2。

- 特征图大小：均为 5×5。

池化方法同前面（3）S2层。

- 可训练参数：共12个可训练参数。因为有6个池化窗口，每个池化窗口都有一个权重，一个偏置，所以这一层可训练参数为 $16×2=32$ 个。

（6）C5层，卷积层：

- 特征图个数：本层得到的是 $120$ 个 $1×1$ 的特征图，即 $120$ 维向量。是上一层的 $16$ 个特征图，经过 $120×16$ 个卷积核卷积操作后得到的。

- 卷积核个数和大小：$120×16$ 个，大小均为 $5×5$ 。

- 特征图大小：均为 1×1，实际已是1维的向量。

- 可训练参数： $（5×5×16+1）×120=48120$ 个。

由于上一层S4的特征图的大小是5×5，本层卷积核大小也是 5×5，故本层特征图的大小为1×1，这已经构成了本层C5和上一层S4之间的**全连接**, 没有特殊的连接表。

（7）F6层,全连接层：

- 特征图个数：本层得到的是84个节点，是上一层的120维向量与84×120维权重矩阵相乘后，加上偏置，经过 $sigmoid$ 激活函数得到的。

- 可训练参数为 $（120+1）×84=10164$ 个。

（8）F7层，输出层：

- 输出为10维向量（10个数字的类别）。与上一层F6层全连接，采用的是径向基函数（RBF）的网络连接方式。

- 权重矩阵：这一层的前面，蕴含 $84×10$ 权重矩阵，故可训练参数数量为 $84×(10+1)= 10164$ 。

LeNet模型总共包含大约 $6$ 万个参数。

总结：

在今天看来，LeNet-5模型比较简单，但其设计的卷积层、池化层、全连接层等，在深度学习的发展过程中，具有里程碑式的意义。

有研究者指出，当时Yann LeCun提出的LeNet-5模型，池化层使用的是average pool，而且各层激活函数一般是Sigmoid和tanh。实际上，可以根据需要，做出改进，使用max pool和激活函数ReLU，效果会更理想。

### 参考

[卷积神经网络](https://mp.weixin.qq.com/s/eOM3YHPkCCmMpLrv4ZDhBA)


[卷积神经网络入门--案例 LeNet-5](https://zhuanlan.zhihu.com/p/579996909)

[卷积神经网络分类](https://blog.csdn.net/qq_32790593/article/details/103528976)