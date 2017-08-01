#include <iostream>
#include <cmath>
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
CvPoint pt[4];
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
    cvClearMemStorage(mem_storage);
}

//-------二值化&寻找轮廓-------

//-------辅助形状判断,距离/角度-------
// pt0->pt1 和 pt0->pt2，找到向量之间的角度余弦
double angle( CvPoint* pt1, CvPoint* pt2, CvPoint* pt0 )
{
    double dx1 = pt1->x - pt0->x;
    double dy1 = pt1->y - pt0->y;
    double dx2 = pt2->x - pt0->x;
    double dy2 = pt2->y - pt0->y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}
double getDistance(CvPoint *pt1,CvPoint *pt2)
{
    double d1=(pt1->x-pt2->x)*(pt1->x-pt2->x);
    double d2=(pt1->y-pt2->y)*(pt1->y-pt2->y);
    return d1+d2;
}
// the function draws all the squares in the image
void drawSquares( IplImage* img, CvSeq* squares )
{
    CvSeqReader reader;
    IplImage* cpy = cvCloneImage( img );
    int i;

    // initialize reader of the sequence
    cvStartReadSeq( squares, &reader, 0 );

    // read 4 sequence elements at a time (all vertices of a square)
    for( i = 0; i < squares->total; i += 4 )
    {
        CvPoint* rect = pt;
        int count = 4;

        // read 4 vertices
        memcpy( pt, reader.ptr, squares->elem_size );
        CV_NEXT_SEQ_ELEM( squares->elem_size, reader );
        memcpy( pt + 1, reader.ptr, squares->elem_size );
        CV_NEXT_SEQ_ELEM( squares->elem_size, reader );
        memcpy( pt + 2, reader.ptr, squares->elem_size );
        CV_NEXT_SEQ_ELEM( squares->elem_size, reader );
        memcpy( pt + 3, reader.ptr, squares->elem_size );
        CV_NEXT_SEQ_ELEM( squares->elem_size, reader );

        // draw the square as a closed polyline
        cvPolyLine( cpy, &rect, &count, 1, 1, CV_RGB(0,255,0), 3, CV_AA, 0 );
    }

    // show the resultant image
    cvShowImage( "picture", cpy );
    cvReleaseImage( &cpy );
}
//-------辅助形状判断,距离/角度-------

//-------形状判断-------
int checkRec(CvSeq *contours,CvSeq *&squares)
{
    int i;
    double s,t;
    CvSeq* result;
    // 使用近似轮廓精度与轮廓周长成比例
    result = cvApproxPoly( contours, sizeof(CvContour), mem_storage,
        CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0 );
    // 矩形轮廓在近似后有四个顶点
    // 相对较大的区域判断过滤掉噪声轮廓，并且是凸的
    // 注意：使用一个区域的绝对值，区域可能是正面或负面，按照轮廓方向
    if( result->total == 4 &&
        fabs(cvContourArea(result,CV_WHOLE_SEQ)) > 1000 &&
        cvCheckContourConvexity(result) )
    {
        s = 0;
        for(i=0;i<5;i++){
            CvPoint *pos=(CvPoint*)cvGetSeqElem(result, i);
            cout<<pos->x<<":"<<pos->y<<endl;
        }
        for( i = 0; i < 5; i++ )
        {
            // find minimum angle between joint
            // edges (maximum of cosine)
            if( i >= 2 )
            {
                t = fabs(angle(
                (CvPoint*)cvGetSeqElem( result, i ),
                (CvPoint*)cvGetSeqElem( result, i-2 ),
                (CvPoint*)cvGetSeqElem( result, i-1 )));
                s = s > t ? s : t;
            }
        }
        // if cosines of all angles are small
        // (all angles are ~90 degree) then write quandrange
        // vertices to resultant sequence
        if( s < 0.3 )
            for( i = 0; i < 4; i++ )
                cvSeqPush( squares,
                    (CvPoint*)cvGetSeqElem( result, i ));
        return 1;   //是矩形
    }
    return 0;     //不是矩形,进行下一个形状的检测
}

int checkRound()
{
    return 1;
}

void check()
{//在已经提取好的thrImg二值轮廓中一一判断每个轮廓
    CvSeq* contours;

    mem_storage = cvCreateMemStorage(0);
    CvSeq* squares = cvCreateSeq( 0, sizeof(CvSeq), sizeof(CvPoint), mem_storage );

    //缩小放大来过滤掉噪声
//    IplImage* timg = cvCloneImage( ithrImgmg );
//    IplImage* pyr = cvCreateImage( cvSize(sz.width/2, sz.height/2), 8, 3 );
//    cvPyrDown( timg, pyr, 7 );
//    cvPyrUp( pyr, timg, 7 );
//    cvReleaseImage(&timg);
//    cvReleaseImage(&pyr);

    cvFindContours( thrImg, mem_storage, &contours, sizeof(CvContour),
                CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );
    //遍历每个轮廓
    while( contours )
    {

        if(checkRec(contours,squares)){
            cout<<"Rec"<<endl;  //该轮廓是矩形
//            continue;
        }else if(checkRound()){
            cout<<"Round"<<endl; //该轮廓是圆
//            continue;
        }

//        cout<<"draw"<<endl;
//        drawSquares(  originImg,  squares );
//        cvShowImage("draw", originImg);
//        cvWaitKey(0);
        //两次去掉对称的情况
        contours = contours->h_next;
        contours = contours->h_next;
    }
    cvClearMemStorage(mem_storage);
}
//-------形状判断-------

//-------流程控制&释放内存-------
void release()
{
    cvReleaseImage(&originImg);
    cvReleaseImage(&gaussImg);
    cvReleaseImage(&thrImg);
    cvReleaseImage(&contourImg);


}
void control()
{//流程控制
    int T=0;
    while (filename[T][0]!=0) {
        init(T);
        Gau();
        thres();
        filter();
        check();
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


