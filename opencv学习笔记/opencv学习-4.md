### cvStartReadSeq函数
函数 cvStartReadSeq 初始化读取部分的状态。  
**函数定义**  
`void cvStartReadSeq( const CvSeq* seq, CvSeqReader* reader, int reverse=0 )`  
**函数参数**
- `seq`  序列
- `reader`  读取部分的状态； 由该函数初始化
- `reverse`  决定遍历序列的方向。如果 reverse　为０，则读取顺序被定位从序列头部元素开始，否则从尾部开始读取

顺序读取可通过调用宏 CV_READ_SEQ_ELEM( read_elem, reader )，逆序读取可通过调用宏CV_REV_READ_SEQ_ELEM( read_elem, reader )。这两个宏都将序列元素读进read_elem中， 并将指针移到下一个元素。下面代码显示了如何去使用reader 和 writer.

---
### ApproxPoly函数
用指定精度逼近多边形曲线  
**函数定义**  
`CvSeq* cvApproxPoly( const void* src_seq, int header_size, CvMemStorage* storage,int method, double parameter, int parameter2=0 );`  
**函数参数**  
- `src_seq`  点集数组序列
- `header_size`  逼近曲线的头尺寸
- `storage`  逼近轮廓的容器。如果为 NULL， 则使用输入的序列
- `method`   逼近方法。目前仅支持 CV_POLY_APPROX_DP， 对应 Douglas-Peucker 算法.
- `parameter`  方法相关参数。对 CV_POLY_APPROX_DP它是指定的逼近精度
- `parameter2`  如果 src_seq 是序列，它表示要么逼近单个序列，要么在 src_seq 的同一个或低级层次上逼近所有序列 (参考 cvFindContours 中对轮廓继承结构的描述). 如果 src_seq 是点集的数组 (CvMat*) ， 参数指定曲线是闭合 (parameter2!=0) 还是非闭合 (parameter2=0).

函数cvApproxPoly逼近一个或多个曲线，并返回逼近结果。对多个曲线的逼近，生成的树将与输入的具有同样的结构。(1:1 的对应关系).
**使用实例**
在`第一阶段完成.cpp`中检测矩形轮廓的时候
```C++
    //在
    result = cvApproxPoly( contours, sizeof(CvContour), mem_storage,
        CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0 );
```

---
### cvFitEllipse2函数
二维点集的椭圆拟合  
**函数定义**  
`CvBox2D cvFitEllipse2( const CvArr* points );`  
**函数参数**  
- `points`  点集的序列或数组  

函数 cvFitEllipse 对给定的一组二维点集作椭圆的最佳拟合(最小二乘意义上的)。返回的结构与 cvEllipse 中的意义类似，除了 size表示椭圆轴的整个长度，而不是一半长度。

---
### cvBoundingRect函数
计算点集的最外面（up-right）矩形边界  
**函数定义**  
`CvRect cvBoundingRect( CvArr* points, int update=0 );`  
**函数参数**  
- `points` ,二维点集，点的序列或向量 (CvMat)
- `update` ,更新标识。下面是轮廓类型和标识的一些可能组合:
  - update=0, contour ~ CvContour*: 不计算矩形边界，但直接由轮廓头的 rect 域得到。
  - update=1, contour ~ CvContour*: 计算矩形边界，而且将结果写入到轮廓头的 rect 域中 header.
  - update=0, contour ~ CvSeq* or CvMat*: 计算并返回边界矩形
  - update=1, contour ~ CvSeq* or CvMat*: 产生运行错误 （runtime error is raised）

函数 cvBoundingRect 返回二维点集的最外面 （up-right）矩形边界

---
### cvMinAreaRect2函数
寻找最小面积的包围矩形  
**函数定义**  
`CvBox2D  cvMinAreaRect2( const CvArr* points, CvMemStorage* storage=NULL ); `  
**函数参数** 
- `points` ,点序列或点集数组
- `storage` ,可选的临时存储仓

函数 cvMinAreaRect2 通过建立凸外形并且旋转外形以寻找给定 2D 点集的最小面积的包围矩形。其中返回的2D盒子定义如下： （CvBox2D可参考-->[说明](http://blog.csdn.net/guoyunfei20/article/details/54616847)）
```
typedef struct CvBox2D 
{ 
    CvPoint2D32f center; /* 盒子的中心 */ 
    CvSize2D32f size; /* 盒子的长和宽 */ 
    float angle; /* 水平轴与第一个边的夹角，用弧度表示*/ 

}CvBox2D; 
```
---
### cvMinEnclosingCircle函数
寻找最小包围的圆
**函数定义**  
`int cvMinEnclosingCircle(const CvArr * points,CvPoint2D32f * 	center,float * 	radius )`	
**函数参数** 
- `points` ,输入的轮廓序列
- `center` ,返回的中心点坐标
- `radius` ，放回的半径

---
### FitLine函数
2D 或 3D 点集的直线拟合  
**函数定义**  
`void   cvFitLine( const CvArr* points, int dist_type,double param,double reps, double aeps, float* line );`  
**函数参数**  
- `points`  ,2D 或 3D 点集，32-比特整数或浮点数坐标
- `dist_type`  ,拟合的距离类型 （见讨论）.
- `param`   ,对某些距离的数字参数，如果是 0， 则选择某些最优值
- `reps, aeps`   ,半径 (坐标原点到直线的距离) 和角度的精度，一般设为0.01。
- `line`   ,输出的直线参数。2D 拟合情况下，它是包含 4 个浮点数的数组 (vx, vy, x0, y0)，其中 (vx, vy) 是线的单位向量而 (x0, y0) 是线上的某个点. 对 3D 拟合，它是包含 6 个浮点数的数组 (vx, vy, vz, x0, y0, z0)， 其中 (vx, vy, vz) 是线的单位向量，而 (x0, y0, z0)是线上某点。

函数 cvFitLine 通过求 sumiρ(ri) 的最小值方法，用 2D 或 3D 点集拟合直线，其中 ri是第 i 个点到直线的距离， ρ(r) 是下面的距离函数之一：
