
#include "initfilter.h"

//-------外部全局变量定义--------
extern double PI;
extern IplImage* originImg;   //初始图像
extern IplImage* gaussImg;    //高斯过滤图像
extern IplImage* thrImg;      //二值化图像
extern IplImage* contourImg;  //轮廓图像
extern IplImage* originHSV;   //初始HSV图片
extern Mat originMat;         //Mat型原图

extern CvMemStorage* mem_storage;
extern CvSeq *first_contour, *c;
extern CvPoint pt[4];
extern char *filename[];
extern int colorRecgnize; //颜色识别的限度---190

extern struct Countour{
    //圆形、正方形、长方形、椭圆形4种（ID依次为1, 2, 3, 4）
    //可乐罐、口香糖、方便桶面、饼干盒4种（ID依次为81, 82, 83, 84）
    //黑、红、黄、绿、蓝5种（ID依次为1, 2, 3, 4, 5）
    int ID[2];
    int X,Y,S;
    double TH;   //识别物体朝向角
}VecCol[15];
extern int vecNum;    //检测到的物体的个数
extern double areaS;  //轮廓的面积
//-------变量定义--------

/*
//-------初始化和高斯过滤-------
void init(int T)
{//进行图片的初始化
    vecNum=0;
    originImg=cvLoadImage(filename[T]);
    originMat=imread(filename[T]);
    originHSV=cvCreateImage(cvGetSize(originImg),8,3);
    cvCvtColor(originImg,originHSV,CV_RGB2HSV);
    cvShowImage("原图", originImg);
//    cout<<"picture x="<<originImg->height<<endl;
//    cout<<"picture y="<<originImg->width<endl;
//    cvWaitKey(0);
}

*/

void Gau()
{//对originImg进行高斯过滤
    gaussImg=cvCreateImage(cvGetSize(originImg), 8, 3);
    cvSmooth(originImg,gaussImg,CV_GAUSSIAN,3,3,0,0);  //  Gauss 平滑
    cvShowImage("高斯过滤", gaussImg);
}
//-------初始化和高斯过滤-------

//-------二值化&寻找轮廓-------
void thres()
{//对高斯过滤后的图像进行二值化
    //创建临时图，灰度化高斯图像
    IplImage* temp=cvCreateImage(cvGetSize(originImg), 8, 1);
    cvCvtColor(gaussImg,temp,CV_RGB2GRAY);
    cvShowImage("灰化高斯", temp);
    //分配thrImg空间，并且二值化
    thrImg=cvCreateImage(cvGetSize(originImg), 8, 1);
    cvThreshold(temp, thrImg, 190, 255, CV_THRESH_BINARY); //190为阈值范围
    cvShowImage("二值化结果", thrImg);
    //清空临时图缓存
    cvReleaseImage(&temp);
}

void filter()
{//将寻找的二值轮廓放到thrImg
    mem_storage = cvCreateMemStorage(0);
    cvFindContours(
            thrImg,
            mem_storage,
            &first_contour,
            sizeof(CvContour),
            CV_RETR_TREE,       //old:CV_RETR_CCOMP,           //#1 需更改区域
            CV_CHAIN_APPROX_NONE, cvPoint(0,0)
            );
    cvZero(thrImg);
    cvDrawContours(
        thrImg,
        first_contour,
        cvScalar(100),
        cvScalar(100),
        2            //old:1           //#2 需更改区域
        );
    cvShowImage("二值轮廓", thrImg);
    cvClearMemStorage(mem_storage);
}

//-------二值化&寻找轮廓-------
