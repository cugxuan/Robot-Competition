#include <cv.h>
#include <highgui.h>
#include <math.h>

int main(int argc, char** argv)
{
    IplImage* img;
    img=cvLoadImage("/2.png", 1);
    IplImage* gray = cvCreateImage( cvGetSize(img), 8, 1 );
    CvMemStorage* storage = cvCreateMemStorage(0);
    cvCvtColor( img, gray, CV_BGR2GRAY );
    cvSmooth( gray, gray, CV_GAUSSIAN, 9, 9 ); // smooth it, otherwise a lot of false circles may be detected
    CvSeq* circles = cvHoughCircles( gray, storage, CV_HOUGH_GRADIENT, 2, gray->height/4, 200, 100 );
    int i;
    for( i = 0; i < circles->total; i++ )
    {
         float* p = (float*)cvGetSeqElem( circles, i );
         cvCircle( img, cvPoint(cvRound(p[0]),cvRound(p[1])), 3, CV_RGB(0,255,0), -1, 8, 0 );
         cvCircle( img, cvPoint(cvRound(p[0]),cvRound(p[1])), cvRound(p[2]), CV_RGB(255,0,0), 3, 8, 0 );
    }
    cvNamedWindow( "circles", 1 );
    cvShowImage( "circles", img );
    cvWaitKey(0);
    return 0;
}

