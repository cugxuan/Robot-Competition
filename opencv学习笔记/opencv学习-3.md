## 识别轮廓


在进行颜色识别的时候对于蓝色的情况
得到的BGR图的
## 高斯过滤

## canny算子求边缘
> 教程链接:[Canny算子,Sobel算子,Laplace算子,Scharr滤波器合辑](http://blog.csdn.net/poem_qianmo/article/details/25560901)

### 简介
Canny边缘检测算子是John F.Canny于 1986 年开发出来的一个多级边缘检测算法。更为重要的是 Canny 创立了边缘检测计算理论（Computational theory ofedge detection），解释了这项技术是如何工作的。Canny边缘检测算法以Canny的名字命名，被很多人推崇为当今最优的边缘检测的算法。  
其中，Canny 的目标是找到一个最优的边缘检测算法，让我们看一下最优边缘检测的三个主要评价标准:  
1.**低错误率**: 标识出尽可能多的实际边缘，同时尽可能的减少噪声产生的误报。  
2.**高定位性**: 标识出的边缘要与图像中的实际边缘尽可能接近。  
3.**最小响应**: 图像中的边缘只能标识一次，并且可能存在的图像噪声不应标识为边缘。  
为了满足这些要求 Canny 使用了变分法，这是一种寻找满足特定功能的函数的方法。最优检测使用四个指数函数项的和表示，但是它非常近似于高斯函数的一阶导数。  

### 函数详解
`C++: void Canny(InputArray image,OutputArray edges, double threshold1, double threshold2, int apertureSize=3,bool L2gradient=false )`
- 第一个参数，InputArray类型的image，输入图像，即源图像，填Mat类的对象即可，且需为单通道8位图像。
- 第二个参数，OutputArray类型的edges，输出的边缘图，需要和源图片有一样的尺寸和类型。
- 第三个参数，double类型的threshold1，第一个滞后性阈值。
- 第四个参数，double类型的threshold2，第二个滞后性阈值。
- 第五个参数，int类型的apertureSize，表示应用Sobel算子的孔径大小，其有默认值3。
- 第六个参数，bool类型的L2gradient，一个计算图像梯度幅值的标识，有默认值false。

需要注意的是，这个函数阈值1和阈值2两者的小者用于边缘连接，而大者用来控制强边缘的初始段，推荐的高低阈值比在2:1到3:1之间。  
调用示例：
```
//载入原始图   
       Mat src = imread("1.jpg");  //工程目录下应该有一张名为1.jpg的素材图
       Canny(src, src, 3, 9,3 );
       imshow("【效果图】Canny边缘检测", src);
```

## cvHoughLines2()函数寻找直线
利用Hough变换在二值图像中寻找直线。
>对于opencv的霍夫变换讲解:[教程](http://blog.csdn.net/poem_qianmo/article/details/26977557)  
### 函数详解
`CvSeq* cvHonghLines2(CvArr* image,void* line_storage,int mehtod,double rho,
double theta,int threshold,double param1 =0,double param2 =0);`

**参数说明：**  
`image`  
输入8-比特、单通道(二值)图像，当用CV_HOUGH_PROBABILISTIC方法检测的时候其内容会被函数改变。  
`line_storage`  
检测到的线段存储仓.可以是内存存储仓(此种情况下，一个线段序列在存储仓中被创建，并且由函数返回），或者是包含线段参数的特殊类型（见下面）的具有单行/单列的矩阵(CvMat*)。矩阵头为函数所修改，使得它的 cols/rows 将包含一组检测到的线段。如果 line_storage 是矩阵，而实际线段的数目超过矩阵尺寸，那么最大可能数目的线段被返回(线段没有按照长度、可信度或其它指标排序).  
`method`  
Hough 变换变量，是下面变量的其中之一：  
- **CV_HOUGH_STANDARD** - 传统或标准 Hough 变换. 每一个线段由两个浮点数 (ρ, θ) 表示，其中 ρ 是直线与原点(0,0) 之间的距离，θ 线段与 x-轴之间的夹角。因此，矩阵类型必须是 CV_32FC2 type.
- **CV_HOUGH_PROBABILISTIC-** 概率 Hough 变换(如果图像包含一些长的线性分割，则效率更高). 它返回线段分割而不是整个线段。每个分割用起点和终点来表示，所以矩阵（或创建的序列）类型是 CV_32SC4.
- **CV_HOUGH_MULTI_SCALE** - 传统 Hough 变换的多尺度变种。线段的编码方式与 CV_HOUGH_STANDARD 的一致.

`rho`
以像素为单位的距离精度，一般取1  
`theta`  
以弧度为单位角度精度,一般取CV_PI/180  
`threshold`  
阈值参数。当在一条直线上的像素点数大于threshold时，才将该直线作为检测结果显示出来。该值越大，得到直线越少。  
`param1`  
对传统 Hough 变换，不使用(0).  
对概率Hough变换，它是最小线段长度.即当线段长度大于param1时，才将该线段作为检测结果显示。与上一参数类似，不过上一参数为像素数，而该参数为线段长度。  
对多尺度 Hough 变换，它是距离精度 rho 的分母 (大致的距离精度是 rho 而精确的应该是 rho / param1 ).  
`param2`  
对传统 Hough 变换，不使用 (0).  
对概率 Hough 变换，这个参数表示在同一条直线上进行碎线段连接的最大间隔值(gap), 即当同一条直线上的两条碎线段之间的间隔小于param2时，将其合二为一条长直线。  
对多尺度 Hough 变换，它是角度精度 theta 的分母 (大致的角度精度是 theta 而精确的角度应该是 theta / param2).

在`4检测直线.cpp`中使用例子
```C++
    lines = cvHoughLines2( dst, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 50, 50, 10 );
```