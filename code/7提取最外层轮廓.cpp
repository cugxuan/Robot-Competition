#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv/cv.h>

using namespace std;
int main()
{
    IplImage* img = cvLoadImage("/2.png", CV_LOAD_IMAGE_GRAYSCALE);
    IplImage* img_temp = cvCreateImage(cvGetSize(img), 8, 1);

    cvThreshold(img, img, 128, 255, CV_THRESH_BINARY);

    CvMemStorage* mem_storage = cvCreateMemStorage(0);
    CvSeq *first_contour = NULL, *c = NULL;

    //////////////////////////////////////////////////////////////////////////
    // 1、
    cvNamedWindow("contour1");
    cvCopyImage(img, img_temp);
    double t = (double)cvGetTickCount();
    cvFindContours(img_temp, mem_storage, &first_contour);
    cvZero(img_temp);
    cvDrawContours(
        img_temp,
        first_contour,
        cvScalar(100),
        cvScalar(100),
        1
        );
    t = (double)cvGetTickCount() - t;
    cvShowImage("contour1", img_temp);

    printf("run1 = %gms\n", t/(cvGetTickFrequency()*1000.));

    cvClearMemStorage(mem_storage);

    //////////////////////////////////////////////////////////////////////////
    // 2、
    cvNamedWindow("contour2");
    cvCopyImage(img, img_temp);
    t = (double)cvGetTickCount();
    CvContourScanner scanner = cvStartFindContours(img_temp, mem_storage);
    while (cvFindNextContour(scanner));
    first_contour = cvEndFindContours(&scanner);

    cvZero(img_temp);
    cvDrawContours(
        img_temp,
        first_contour,
        cvScalar(100),
        cvScalar(100),
        1
        );
    t = (double)cvGetTickCount() - t;
    cvShowImage("contour2", img_temp);

    printf("run2 = %gms\n", t/(cvGetTickFrequency()*1000.));

    cvClearMemStorage(mem_storage);
    cvReleaseImage(&img);
    cvReleaseImage(&img_temp);

    cvWaitKey();

    /************************************************************************/
    /* 经测试 run1 = 16.1431ms run2 = 15.8677ms (参考)
       不过可以肯定这两中算法时间复杂度是相同的                                     */
    /************************************************************************/

    //////////////////////////////////////////////////////////////////////////
    // 上述两种方法完成了对轮廓的提取,如想绘制轮廓都得配合cvDrawContours来使用
    // 而cvDrawContours 函数第5个参数为 max_level 经查ICVL含义如下:
    //
    // 绘制轮廓的最大等级。如果等级为0，绘制单独的轮廓。如果为1，绘制轮廓及在其后的相同的级别下轮廓。
    // 如果值为2，所有的轮廓。如果等级为2，绘制所有同级轮廓及所有低一级轮廓，诸此种种。如果值为负数，
    // 函数不绘制同级轮廓，但会升序绘制直到级别为abs(max_level)-1的子轮廓。
    //
    // 相信好多读者初次都无法理解等级的含义,而且测试时候输入>=1 的整数效果几乎一样
    // 只有提取轮廓时候的提取模式设为 CV_RETR_CCOMP CV_RETR_TREE 时这个参数才有意义
    //
    // 经查FindContours 函数里面这样介绍提取模式(mode)的这两个参数:
    // CV_RETR_CCOMP - 提取所有轮廓，并且将其组织为两层的 hierarchy: 顶层为连通域的外围边界，次层为洞的内层边界。
    // CV_RETR_TREE - 提取所有轮廓，并且重构嵌套轮廓的全部 hierarchy
    //
    // 下面用第一种方法进行测试

    cvNamedWindow("contour_test");
    cvNamedWindow("contour_raw");
    img = cvLoadImage("/2.png", CV_LOAD_IMAGE_GRAYSCALE);
    cvShowImage("contour_raw", img);
    cvThreshold(img, img, 180, 255, CV_THRESH_BINARY);
    cvShowImage("erzhi", img);
    img_temp = cvCloneImage(img);
    cvFindContours(
        img_temp,
        mem_storage,
        &first_contour,
        sizeof(CvContour),
        CV_RETR_TREE,       //old:CV_RETR_CCOMP,           //#1 需更改区域
        CV_CHAIN_APPROX_NONE, cvPoint(0,0)
        );

    cvZero(img_temp);
    cvDrawContours(
        img_temp,
        first_contour,
        cvScalar(100),
        cvScalar(100),
        2            //old:1           //#2 需更改区域
        );
    cvShowImage("contour_test", img_temp);
    /************************************************************************/
    /* (1, 2) = (CV_RETR_CCOMP, 1)  如图1
       (1, 2) = (CV_RETR_CCOMP, 2)  如图2
       (1, 2) = (CV_RETR_TREE, 1)   如图3
       (1, 2) = (CV_RETR_TREE, 2)   如图4
       (1, 2) = (CV_RETR_TREE, 6)   如图5
       经分析CV_RETR_CCOMP 只把图像分为两个层次,顶层和次层,一等级轮廓只匹配与其最接近
       的内侧轮廓即2等级
       CV_RETR_TREE 则从轮廓外到内按等级1 - n 全部分配
       CV_RETR_LIST 全部轮廓均为1级                        */
    /************************************************************************/

    cvWaitKey();
    cvReleaseImage(&img);
    cvReleaseImage(&img_temp);
    cvReleaseMemStorage(&mem_storage);
    cvDestroyAllWindows();
    return 0;
}

