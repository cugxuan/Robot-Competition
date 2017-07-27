#include <iostream>
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
using namespace std;
int main()
{
    CvMemStorage *storage = cvCreateMemStorage(0);   // 内存存储序列
    IplImage *img = cvLoadImage("/1.png", 0);
    IplImage *imgColor = cvCreateImage(cvGetSize(img), 8, 3);
    IplImage *contoursImage = cvCreateImage(cvGetSize(img), 8, 1);

    CvSeq *contours = 0, *contoursTemp = 0;
    cvZero(contoursImage);
    cvThreshold(img, img, 100, 255, CV_THRESH_BINARY);  // 二值化操作
    cvCvtColor(img, imgColor, CV_GRAY2BGR);
    int totals = cvFindContours(img, storage,&contours, sizeof(CvContour),    //img必须是一个二值图像 storage 用来存储的contours指向存储的第一个轮廓
        CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cvPoint(0,0));
    contoursTemp = contours;
    int count = 0;
    int i;
    for(;contoursTemp != 0; contoursTemp = contoursTemp -> h_next)  /// 这样可以访问每一个轮廓  ====横向轮廓
    {
        for(i = 0; i < contoursTemp -> total; i++)    // 提取一个轮廓的所有坐标点
        {
            CvPoint *pt = (CvPoint*) cvGetSeqElem(contoursTemp, i);   // 得到一个轮廓中一个点的函数cvGetSeqElem
            cvSetReal2D(contoursImage, pt->y, pt->x, 255.0);
            cvSet2D(imgColor, pt->y, pt->x, cvScalar(0,0,255,0));
        }
        count ++;
        CvSeq *InterCon = contoursTemp->v_next;     // 访问每个轮廓的纵向轮廓
        for(; InterCon != 0; InterCon = InterCon ->h_next)
        {
            for(i = 0; i < InterCon->total; i++ )
            {
                CvPoint *pt = (CvPoint*)cvGetSeqElem(InterCon, i);
                cvSetReal2D(contoursImage, pt->y, pt->x, 255.0);
                cvSet2D(imgColor, pt->y, pt->x, cvScalar(0, 255, 0, 0));
                cout<<pt->x<<":"<<pt->y<<endl;
            }
        }
    }
    cvNamedWindow("contoursImage");
    cvShowImage("contoursImage", contoursImage);
    cvNamedWindow("imgColor");
    cvShowImage("imgColor",imgColor);
    cvWaitKey(0);
    cvReleaseMemStorage(&storage);      // 也要释放内存序列空间
    cvReleaseImage(&contoursImage);
    cvReleaseImage(&imgColor);
    cvDestroyWindow("contoursImage");
    cvDestroyWindow("imgColor");
    return 0;
}

