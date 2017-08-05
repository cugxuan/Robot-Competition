# opencv学习笔记-2
&emsp;&emsp;

## HSV和RGB的内容
&emsp;&emsp;两种格式比较重要，当然还有灰度图GRAY，这里就不做什么解释了。
### 两者的意思
在进行图像图像处理的时候需要对图像格式进行转换。
 将RGB格式的图像转换成HSV格式。
 &emsp;&emsp;RGB：三原色光模式（RGB color model），又称RGB颜色模型或红绿蓝颜色模型，是一种加色模型，将红（Red）、绿（Green）、蓝（Blue）三原色的色光以不同的比例相加，以产生多种多样的色光。(且三原色的红绿蓝不可能用其他单色光合成)

 - void cvCvtColor(const CvArr* src, CvArr* dst, int code): 按code指定的模式将src指向的帧转换后写入dst指向的地址。

 - void calcHist(const Mat* images, int nimages, const int* channels, InputArray mask, SparseMat& hist, int dims, const int* histSize, const float** ranges, bool uniform, bool accumulate): 为image指向的帧计算直方图，保存在 hist 中。

 - void normalize(const SparseMat& src, SparseMat& dst, double alpha, int normType): 按指定模式将src正常化并写入dst中。

 - double compareHist(const SparseMat& H1, const SparseMat& H2, int method):按method指定的方式比较两个直方图的相似程度。

 - int cvWriteFrame(CvVideoWriter* writer, const IplImage* image): 向视频写入流写入一帧。成功返回 1，否则返回 0。

### 直方图处理的样例代码
```
// set background
if (backGroundFlag){
    //把frame(当前帧)的内容从BGR格式转换到HSV格式放在HSV中
    cvtColor(frame, HSV, CV_BGR2HSV);
    //绘制直方图，把HSV的内容
    calcHist(&HSV, 1, channels, Mat(), base, 2, histSize, ranges, true, false);
    normalize(base, base, 0, 1, NORM_MINMAX, -1, Mat());
    backGroundFlag = false;
}
```

## 重要基本数据结构

### CvPoint
使用(CvPoint*)cvGetSeqElem(lines,i)的到对应的指针.   
在`4检测直线.cpp`中的画线方法
```C++
for( i = 0; i < lines->total; i++ )
{
    CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
    cvLine( color_dst, line[0], line[1], CV_RGB(255,0,0), 3, CV_AA, 0 );
	//color_dst图像类型,line[0],line[1]线段的两个端点CvPoint类型,
	//CV_RGB(255,0,0)线段的颜色,彩图有效.3(thickness)线的粗细程度
	//line_type线段类型,CV_AA为线段,8为(8邻接)连接线,4同理.
	//0坐标点的小数点位数
}
```

## cvSet2D()&&cvSetReal2D()
对某个坐标点进行颜色的设置,在学习调试的过程中可以非常方便地通过这个方法来查看点的位置.
cvSet2D()是对多通道的彩色图进行设置,cvSetReal2D()是对单通道图设置

**使用样例:**
```C++
cvSetReal2D(contoursImage, pt->y, pt->x, 255.0);
cvSet2D(imgColor, pt->y, pt->x, cvScalar(0, 255, 0, 0));  
```

## 通过三个点的坐标计算角度
已知A(x1，y1)，B(x2，y2)，C(x3，y3),计算∠BAC.  
```
向量AB=(x2-x1，y2-y1),向量AC=(x3-x1，y3-y1)
AB*AC=|AB||AC|cos∠BAC
     =(x2-x1)(x3-x1)+(y2-y1)(y3-y1)
于是，cos∠BAC=(AB*AC)/[|AB|*|AC|]
```
计算得到的是余弦值,然后通过公式进行换算:  
角度=180/π*acos(∠BAC)  
在`第一阶段完成.cpp`中使用的转换余弦值的公式代码为:
```C++
double ang=angle(temp[0],&up,&mid); //mid为要求角度的点
ang=180/PI*acos(ang);        //进行余弦值转化角度值
```
---
## cvContourArea 计算轮廓面积
>就是求的轮廓所占的面积.看这个就懂了.--->[解释](http://blog.csdn.net/wjq123000/article/details/52077961?locationNum=11&fps=1)

**函数定义**:  
`double cvContourArea( const CvArr* contour, CvSlice slice=CV_WHOLE_SEQ );`
- contour：轮廓（顶点的序列或数组）。
- slice：感兴趣区轮廓部分的起点和终点，默认计算整个轮廓的面积。
**使用方法**:
```C++
if(fabs(cvContourArea(result,CV_WHOLE_SEQ)) > 1000)
    //那么执行...
```
---
## cvArcLength  计算轮廓周长
**函数定义**:  
`double cvArcLength( const void* curve, CvSlice slice=CV_WHOLE_SEQ, int is_closed=-1 );`
- `curve`
曲线点集序列或数组
- `slice`
曲线的起始点，缺省是计算整个曲线的长度
- `is_closed`
表示曲线是否闭合，有三种情况：
  - is_closed=0 - 假设曲线不闭合
  - is_closed>0 - 假设曲线闭合
  - is_closed<0 - 若曲线是序列，检查 ((CvSeq*)curve)->flags 中的标识 CV_SEQ_FLAG_CLOSED 来确定曲线是否闭合。否则 (曲线由点集的数组 (CvMat*) 表示) 假设曲线不闭合。
  