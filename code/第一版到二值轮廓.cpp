#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv/cv.h>

using namespace std;
using namespace cv;
//-------变量定义--------
IplImage* originImg;   //初始图像
IplImage* gaussImg;    //高斯过滤图像
IplImage* thrImg;      //二值化图像
IplImage* contourImg;  //轮廓图像

CvMemStorage* mem_storage;
CvSeq *first_contour = NULL, *c = NULL;
char *filename[]={"/1.png",/*"/2.png",*/0};


struct Countour{
    int ID[2];
    int X,Y,S;
    int TH;   //识别物体朝向角
};
//-------变量定义--------

//-------初始化和高斯过滤-------
void init(int T)
{//进行图片的初始化
    originImg=cvLoadImage(filename[T]);
    cvShowImage("原图", originImg);
//    cvWaitKey(0);
}
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
    cvThreshold(temp, thrImg, 200, 255, CV_THRESH_BINARY);
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
}

//-------二值化&寻找轮廓-------
//-------形状判断-------

//-------形状判断-------

//-------流程控制&释放内存-------
void release()
{
    cvReleaseImage(&originImg);
    cvReleaseImage(&gaussImg);
    cvReleaseImage(&thrImg);
    cvReleaseImage(&contourImg);

    cvClearMemStorage(mem_storage);
}
void control()
{//流程控制
    int T=0;
    while (filename[T][0]!=0) {
        init(T);
        Gau();
        thres();
        filter();
        release();
        cvWaitKey(0);
        T++;
    }
}
//-------二值化&寻找轮廓-------

int main()
{
    control();
}

