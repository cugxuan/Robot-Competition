/* This is a standalone program. Pass an image name as a first parameter of the program.
   Switch between standard and probabilistic Hough transform by changing "#if 1" to "#if 0" and back */
#include <cv.h>
#include <highgui.h>
#include <math.h>

int main(int argc, char** argv)
{
    const char* filename = argc >= 2 ? argv[1] : "/2.png";
    IplImage* src = cvLoadImage( filename, 0 );
    IplImage* dst;
    IplImage* color_dst;
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* lines = 0;
    int i;

    if( !src )  //判断是否导入图片成功
        return -1;

    dst = cvCreateImage( cvGetSize(src), 8, 1 );
    color_dst = cvCreateImage( cvGetSize(src), 8, 3 );

    cvCanny( src, dst, 50, 200, 3 );   //canny算子求边缘
    cvShowImage( "cannyDst", dst );    //显示计算得到的dst图像
    cvCvtColor( dst, color_dst, CV_GRAY2BGR );
    cvShowImage( "cannyColor_Dst", color_dst );
    
#if 0 //
    lines = cvHoughLines2( dst, storage, CV_HOUGH_STANDARD, 1, CV_PI/180, 100, 0, 0 );

    for( i = 0; i < MIN(lines->total,100); i++ )
    {
        float* line = (float*)cvGetSeqElem(lines,i);
        float rho = line[0];
        float theta = line[1];
        CvPoint pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        cvLine( color_dst, pt1, pt2, CV_RGB(255,0,0), 3, CV_AA, 0 );
    }
#else
    lines = cvHoughLines2( dst, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 50, 50, 10 );  //概率霍夫变换
    for( i = 0; i < lines->total; i++ )
    {
        CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
        cvLine( color_dst, line[0], line[1], CV_RGB(255,0,0), 3, CV_AA, 0 );
	//color_dst图像类型,line[0],line[1]线段的两个端点CvPoint类型,
	//CV_RGB(255,0,0)线段的颜色,彩图有效.3(thickness)线的粗细程度
	//line_type线段类型,CV_AA为线段,8为(8邻接)连接线,4同理.
	//0坐标点的小数点位数
    }
#endif
    cvNamedWindow( "Source", 1 );
    cvShowImage( "Source", src );

    cvNamedWindow( "Hough", 1 );
    cvShowImage( "Hough", color_dst );

    cvWaitKey(0);

    return 0;
}

