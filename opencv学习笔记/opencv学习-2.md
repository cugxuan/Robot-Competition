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

##