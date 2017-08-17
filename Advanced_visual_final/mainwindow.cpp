#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPoint>
#include <QString>
#include <QMouseEvent>
#include <QDebug>
#include <iostream>
#include <QLineEdit>
#include <QWidget>
#include <iostream>

using namespace std;
using namespace cv;

extern int vecNum;
struct Countour{

    //圆形、正方形、长方形、椭圆形4种（ID依次为1, 2, 3, 4）
    //可乐罐、口香糖、方便桶面、饼干盒4种（ID依次为81, 82, 83, 84）
    //黑、红、黄、绿、蓝5种（ID依次为1, 2, 3, 4, 5）
    int ID[2];
    int X,Y,S;
    double TH;   //识别物体朝向角
};
extern Countour VecCol[20];
extern IplImage* originImg;
extern Mat originMat;
extern void Gau();
extern void thres();
extern void filter();
extern void check();
extern void checkTest();
extern void PrintC();
extern void release();

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    this->setMouseTracking(true);

    this->set_back();
    this->set_inputback();
    this->set_outputback();

    this->mouseMoveEvent(event);

    imag = new QImage();// 初始化
    /*信号和槽*/
    connect(ui->open, SIGNAL(clicked()), this, SLOT(on_open_clicked()));
    connect(ui->get, SIGNAL(clicked()), this, SLOT(on_get_clicked()));
    connect(ui->start, SIGNAL(clicked()), this, SLOT(on_start_clicked()));
}

MainWindow::~MainWindow()
{
    cap.release();
    delete ui;
}

void MainWindow::set_back()
{
    //设置背景图片
    this->setAutoFillBackground(true); // 显示出背景图.
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,
        QBrush(QPixmap("/back.png").scaled(// 缩放背景图.
           this->size(),
           Qt::IgnoreAspectRatio,
           Qt::SmoothTransformation)));// 使用平滑的缩放方式
    this->setPalette(palette);   // 给widget加上背景图
}

void MainWindow::set_inputback()
{
    QPalette pal(ui->widget_input->palette());
    //设置背景黑色
    pal.setColor(QPalette::Background, Qt::white);
    ui->widget_input->setAutoFillBackground(true);
    ui->widget_input->setPalette(pal);
    ui->widget_input->show();
}

void MainWindow::set_outputback()
{
    QPalette pal(ui->widget_output->palette());
    //设置背景黑色
    pal.setColor(QPalette::Background, Qt::white);
    ui->widget_output->setAutoFillBackground(true);
    ui->widget_output->setPalette(pal);
    ui->widget_output->show();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    int temp_x,temp_y;
    temp_x=event->pos().x();
    temp_y=event->pos().y();
    //std::cout<<temp_x<<","<<temp_y<<std::endl;
    QString x = QString::number(temp_x, 10);
    QString y = QString::number(temp_y, 10);
    ui->x->setText(x);
    ui->y->setText(y);
}

void MainWindow::on_open_clicked()
{
    //opne the camera
    cap=1;
    if(!cap.isOpened())
    {
         std::cout<<"error"<<std::endl;
    }
    while (1)
    {
        cap>>frame;
        waitKey(30);//给图像绘制留点时间
        // 将抓取到的帧，转换为QImage格式。QImage::Format_RGB888不同的摄像头用不同的格式。
        QImage imag = QImage((const uchar
                              *)(frame.data), frame.cols, frame.rows, QImage::Format_RGB888);
        // deep copy the data from mat to QImage
        imag.bits();
        ui->label->setPixmap(QPixmap::fromImage(imag));  // 将图片显示到label上
    }
}

void MainWindow::on_get_clicked()
{
//    cap=1;
    //finish the demarcate
#if 1//read in capture
    cap>>frame;// 从摄像头中抓取并返回每一帧
#else
    frame=imread("/home/xuan/qt/piture/normal/3.jpg");
//    frame=imread("/home/xuan/qt/piture/8.jpg");
//    frame=imread("/frame1.jpg");
#endif
    imwrite("frame0.jpg",frame);
    // 将抓取到的帧，转换为QImage格式。QImage::Format_RGB888不同的摄像头用不同的格式。
    QImage imag = QImage((const uchar*)(frame.data), frame.cols, frame.rows, QImage::Format_RGB888);
    // deep copy the data from mat to QImage
    imag.bits();
    originMat=frame.clone();
    //
    Mat dst;
    double alpha =1.4;
    double beta = 35;

    dst = Mat::zeros(originMat.size(),originMat.type());
    for (int i = 0;i<originMat.rows;++i)
        for(int j= 0;j<originMat.cols;++j)
            for(int k = 0;k<3;++k)
                dst.at<Vec3b>(i,j)[k] = saturate_cast<uchar>(originMat.at<Vec3b>(i,j)[k]*alpha+beta);

//    namedWindow("yuantu");
//    imshow("yuantu",frame);

    qstrlength=ui->lineEdit_length->text();
    length=qstrlength.toFloat();
    qstrwideth=ui->lineEdit_wideth->text();
    wideth=qstrwideth.toFloat();
    x[0]=ui->lineEdit_1->text().toInt();
    x[1]=ui->lineEdit_3->text().toInt();
    x[2]=ui->lineEdit_5->text().toInt();
    x[3]=ui->lineEdit_7->text().toInt();
    y[0]=ui->lineEdit_2->text().toInt();
    y[1]=ui->lineEdit_4->text().toInt();
    y[2]=ui->lineEdit_6->text().toInt();
    y[3]=ui->lineEdit_8->text().toInt();


//    length_x=x[2]-x[0]-4;
//    wideth_y=y[1]-y[0]-4;
//    Mat imageROI(dst,Rect(x[0]+2,y[0]+2,length_x,wideth_y));
    length_x=x[2]-x[0];
    wideth_y=y[1]-y[0];
    Mat imageROI(dst,Rect(x[0],y[0]-20,length_x,wideth_y));

//    namedWindow("qietu");
//    imshow("qietu",imageROI);

    originMat=imageROI.clone();
//    namedWindow("Handled Image");
//    imshow("Handled Image",originMat);
    imwrite("zengqiang.png",dst);

    //
    originImg=cvCreateImage(cvSize(originMat.cols,originMat.rows),8,3);
//    originImg->imageData=(char *)originMat.data;
    *originImg=IplImage(originMat);
    cvShowImage("原图", originImg);
    imwrite("yuantu.jpg",originMat);
//    waitKey(0);
//    length_changerate=length/sqrt((x[2]-x[0])*(x[2]-x[0])+(y[2]-y[0])*(y[2]-y[0]));
//    width_changerate=wideth/sqrt((y[3]-y[1])*(y[3]-y[1])+(x[3]-x[1])*(x[3]-x[1]));
    length_changerate=(double)length/(double)length_x;
    width_changerate=(double)wideth/(double)wideth_y;

    /*
    qDebug() << "the lenth of the input:"<<length<<endl;
    qDebug() <<"the length of the input:"<<wideth<<endl;

    for(int i=0;i<4;i++)
    {
        qDebug()<<"x"<<i<<":"<<x[i]<<",y"<<i<<":"<<y[i]<<endl;
    }
*/
}

void MainWindow::test()
{
    ui->textEdit->clear();
    QString vec = QString::number(vecNum, 10);
    ui->num->setText(vec);
    double temp;
    for(int i=0;i<vecNum;i++)
    {
        char str[4];
        str[0]=VecCol[i].ID[0]+'0';
        str[1]=VecCol[i].ID[1]+'0';
        str[2]=i+'a';
//        cout<<str<<endl;
        putText(originMat,str,cvPoint(VecCol[i].X,VecCol[i].Y),1,1,CV_RGB(0,0,0));
        if(VecCol[vecNum].TH==-90.0)
            VecCol[vecNum].TH=90;
        QString  id,x,y,th,s;
        id.sprintf("%s %d %s %d%d","识别目标",i+1,"ID ",VecCol[i].ID[0],VecCol[i].ID[1]);
//        x.sprintf("%s %d %s %.2f","识别目标",i+1,"中心x ",VecCol[i].X*length_changerate);
//        y.sprintf("%s %d %s %.2f","识别目标",i+1,"中心y ",VecCol[i].Y*width_changerate);
//        th.sprintf("%s %d %s %.2f","识别目标",i+1,"朝向角TH ",VecCol[i].TH);
//        s.sprintf("%s %d %s %.2f","识别目标",i+1,"尺寸S ",VecCol[i].S*length_changerate*width_changerate);
        x.sprintf("%s %d %s %d","识别目标",i+1,"中心x ",(int)(VecCol[i].X*length_changerate+0.5));
        y.sprintf("%s %d %s %d","识别目标",i+1,"中心y ",(int)(VecCol[i].Y*width_changerate+0.5));
        th.sprintf("%s %d %s %d","识别目标",i+1,"朝向角TH ",(int)(VecCol[i].TH+0.5));
        s.sprintf("%s %d %s %d","识别目标",i+1,"尺寸S ",(int)(VecCol[i].S*length_changerate*width_changerate+0.5));

        ui->textEdit->append(id);
        ui->textEdit->append(x);
        ui->textEdit->append(y);
        ui->textEdit->append(th);
        ui->textEdit->append(s);
        ui->textEdit->append("\n");
    }
    imshow("number",originMat);
    cvWaitKey();
}

void MainWindow::control()
{//流程控制
//    while (originImg!=NULL) {
        //init(T);
        Gau();
        thres();
        filter();
//        cvWaitKey();
#if 1   //使用部分
        check();
#else   //测试部分
        checkTest();
#endif
        PrintC();
        test();
//        release();

        cvWaitKey(0);
//    }
}


void MainWindow::on_start_clicked()
{
    //timer->stop();         // 停止读取数据。
    vecNum=0;
    cap.release();
    control();
//    release();
}
