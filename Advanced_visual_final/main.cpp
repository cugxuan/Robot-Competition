#include "mainwindow.h"
#include <QApplication>

#include "origin.h"
#include "initfilter.h"
#include "cal.h"
#include "check.h"

//-------全局变量定义--------
double PI=atan(1.0)*4;

IplImage* originImg;   //初始图像
IplImage* gaussImg;    //高斯过滤图像
IplImage* thrImg;      //二值化图像
IplImage* contourImg;  //轮廓图像
IplImage* originHSV;   //初始HSV图片
Mat originMat;         //Mat型原图

CvMemStorage* mem_storage;
CvSeq *first_contour = NULL, *c = NULL;
CvPoint pt[4];
//char *filename[]={"/home/dongshifo/opencv/image/1/32.png",0};
int colorRecgnize=140; //颜色识别的限度

struct Countour{
    //圆形、正方形、长方形、椭圆形4种（ID依次为1, 2, 3, 4）
    //可乐罐、口香糖、方便桶面、饼干盒4种（ID依次为81, 82, 83, 84）
    //黑、红、黄、绿、蓝5种（ID依次为1, 2, 3, 4, 5）
    int ID[2];
    int X,Y,S;
    double TH;   //识别物体朝向角
}VecCol[15];
int vecNum;    //检测到的物体的个数
double areaS;  //轮廓的面积
//-------变量定义--------

//-------流程控制&释放内存-------
void PrintC()
{
    if(vecNum==0){
        cout<<"没有检测到形状"<<endl;
        return ;
    }else{
        cout<<"检测到"<<vecNum<<"个物体"<<endl;
        for(int i=0;i<vecNum;i++){
            cout<<"第"<<i+1<<"轮廓为"<<endl;
            cout<<"ID:"<<VecCol[i].ID[0]<<VecCol[i].ID[1]<<endl;
            cout<<"X:"<<VecCol[i].X<<" Y:"<<VecCol[i].Y<<endl;
            cout<<"S:"<<VecCol[i].S<<endl;
            cout<<"angle:"<<VecCol[i].TH<<endl<<endl;

            cvSetReal2D(thrImg,VecCol[i].Y, VecCol[i].X, 255.0);
            cvShowImage("yanse", thrImg);
        }
    }
}

void release()
{
    cvReleaseImage(&originImg);
    cvReleaseImage(&gaussImg);
    cvReleaseImage(&thrImg);
    cvReleaseImage(&contourImg);

}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
