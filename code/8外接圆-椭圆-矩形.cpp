int checkRound(CvSeq *contours)
{
    mem_storage = cvCreateMemStorage(0);
//-------------------------------------
    if(contours->total<5){
        contours=contours->v_next;
//        continue;
        return 1;
    }

    int c=contours->total;//当前轮廓包含多少个元素，这里的元素为点
//    cout<<c<<endl;
    double length = cvArcLength(contours); //得到指定的那个轮廓的周长
    //该函数有3个参数：序列，起点（默认计算整条曲线），是否封闭曲线
    double area = cvContourArea(contours);  //得到指定的那个轮廓的面积
    CvRect rect = cvBoundingRect(contours,1);  //根据序列，返回轮廓外围矩形；
    CvBox2D box = cvMinAreaRect2(contours,NULL);  //最小外围矩形

    cout<<"Length = "<<length<<endl;
    cout<<"Area = "<<area<<endl;

    //外围矩形的顶点
    CvPoint pt1,pt2;
    pt1.x=rect.x;
    pt1.y=rect.y;
    pt2.x=rect.x+rect.width;
    pt2.y=rect.y+rect.height;

//        IplImage *dst = cvCreateImage(cvGetSize(src),8,3); //目标图像为3通道图
//        cvZero(dst);
    //使用红色给他画出轮廓
    cvDrawContours(originImg,contours,CV_RGB(255,0,0),CV_RGB(255,0,0),0);
    //使用绿色给他画出外界矩形
    cvRectangle(originImg,pt1,pt2,CV_RGB(0,255,0));
    //将图片显示
    cvNamedWindow("dst",1);
    cvShowImage("dst",originImg);

    //根据序列找出最小面积外接圆
    CvPoint2D32f center;
    float radius;
    int a=cvMinEnclosingCircle(contours,&center,&radius);

    //圆必须是包含所有点，成功返回1，并且得到圆心和半径
    cout<<"center.x = "<<center.x<<endl;
    cout<<"center.y = "<<center.y<<endl;
    cout<<"radius = "<<radius<<endl;

    //画出最小外接圆，并显示
    cvCircle(originImg,cvPointFrom32f(center),cvRound(radius),CV_RGB(100,100,100));
    cvShowImage("dst",originImg);
//    cvWaitKey();

    //最小二乘法的椭圆拟合
    CvBox2D ellipse = cvFitEllipse2(contours);
    //用黄色在图上画椭圆,并显示
    cvEllipseBox(originImg,ellipse,CV_RGB(255,255,0));
    cvShowImage("dst",originImg);
//    cvWaitKey();

    //绘用蓝色制外接最小矩形
    CvPoint2D32f pt[4];
    cvBoxPoints(box,pt);
    for(int i = 0;i<4;++i){
        cvLine(originImg,cvPointFrom32f(pt[i]),cvPointFrom32f(pt[((i+1)%4)?(i+1):0]),CV_RGB(0,0,255));
    }
    cvShowImage("dst",originImg);
//    cvWaitKey();
   
    cvClearMemStorage(mem_storage);
    return 1;
}
