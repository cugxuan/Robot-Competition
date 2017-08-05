---
title: Opencv教程列表
date: 2017-07-27 21:13:55
tags:
 - opencv
 - C++
---
## 前言
放一些opencv的教程便于查找


1. [IplImage, CvMat, Mat 的关系和相互转换](http://blog.csdn.net/timidsmile/article/details/7648725)
2. [OpenCV函数cvFindContours](http://blog.csdn.net/augusdi/article/details/9000893)
3. [提取轮廓两种方法及绘制轮廓中最大等级分析](http://blog.csdn.net/zcube/article/details/7357602#)
4. [OpenCV中寻找轮廓函数cvFindContours的使用说明以及序列cvSeq的用法说明](http://blog.csdn.net/augusdi/article/details/9000276)
5. [RGB坐标像素的储存和提取](http://blog.csdn.net/wenhao_ir/article/details/51554530)
6. [opencv 连通区域边界坐标提取](http://blog.csdn.net/fei13148687/article/details/46225243)
7. [OpenCV霍夫变换：霍夫线变换，霍夫圆变换合辑](http://blog.csdn.net/poem_qianmo/article/details/26977557)
8. [均值，中值，高斯滤波](http://blog.csdn.net/abcjennifer/article/details/7314749)
9. [检测矩形的参考&&检测圆,直线](http://blog.csdn.net/byxdaz/article/details/4912136)
10. [霍夫圆变换函数HoughCircles](http://b217dgy.blog.51cto.com/5704306/1320360)
11. 还没看,不知道用不用的上--->[OpenCV霍夫变换识别圆](http://blog.csdn.net/u013162930/article/details/47396257)

-----

下一步方向::估计是轮廓特征.  (机器学习??)
已经8.4了.没几天的时间了.还有学校集训.稍微转移重心做比赛.
1. 很详细,轮廓处理方法-->[轮廓处理函数详细](http://www.verydemo.com/demo_c92_i143180.html)
2. []()
3. []()
4. []()


参考代码,,寻找方法
```C++
//需要获取的坐标  
CvPoint2D32f rectpoint[4];   
CvBox2D End_Rage2D = cvMinAreaRect2(pFirstSeq); //寻找包围矩形，获取角度  

cvBoxPoints(End_Rage2D, rectpoint); //获取4个顶点坐标  
//与水平线的角度  
float angle = End_Rage2D.angle;
//然后判断angle

```