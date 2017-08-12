#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <iostream>
#include <QWidget>
#include <QTimer>
#include <QImage>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include "initfilter.h"
#include "check.h"
#include "cal.h"

using namespace std;
using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int length,wideth;
    int length_x,wideth_y;
    QString qstrlength,qstrwideth;
    double length_changerate,width_changerate;
    int x[4],y[4];
    QMouseEvent *event;
    void test();
    void control();

protected:
    void mouseMoveEvent(QMouseEvent * event);//添加mouseMover事件响应

private slots:
    void on_open_clicked();
    void on_get_clicked();
    void on_start_clicked();
private:

    Ui::MainWindow *ui;
    QTimer *timer;
    QImage *imag;
    VideoCapture cap;
    Mat frame;
    void set_back();
    void set_inputback();
    void set_outputback();
};

#endif // MAINWINDOW_H
