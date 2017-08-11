#include "check.h"

//-------外部全局变量定义--------
extern double PI;
extern IplImage* originImg;   //初始图像
extern IplImage* gaussImg;    //高斯过滤图像
extern IplImage* thrImg;      //二值化图像
extern IplImage* contourImg;  //轮廓图像
extern IplImage* originHSV;   //初始HSV图片
extern Mat originMat;         //Mat型原图

extern CvMemStorage* mem_storage;
extern CvSeq *first_contour, *c;
extern CvPoint pt[4];
extern char *filename[];
extern int colorRecgnize; //颜色识别的限度---190

extern struct Countour{
    //圆形、正方形、长方形、椭圆形4种（ID依次为1, 2, 3, 4）
    //可乐罐、口香糖、方便桶面、饼干盒4种（ID依次为81, 82, 83, 84）
    //黑、红、黄、绿、蓝5种（ID依次为1, 2, 3, 4, 5）
    int ID[2];
    int X,Y,S;
    double TH;   //识别物体朝向角
}VecCol[15];
extern int vecNum;    //检测到的物体的个数
extern double areaS;  //轮廓的面积
//-------变量定义--------


//-------形状判断-------
int isExist(int x,int y)
{
    for(int i=0;i<vecNum;i++){
        if(abs(x-VecCol[i].X)<3&&abs(y-VecCol[i].Y)<3)
            return 1;
    }
    return 0;
}

int checkRec(CvSeq *contours,CvSeq *&squares)
{
    int i;
    double s,t;
    CvSeq* result;
    // 使用近似轮廓精度与轮廓周长成比例
    result = cvApproxPoly( contours, sizeof(CvContour), mem_storage,
        CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0 );
//    if(0){//如果是图像框,则跳过
//        return 1;
//    }
    // 矩形轮廓在近似后有四个顶点
    // 相对较大的区域判断过滤掉噪声轮廓，并且是凸的
    // 注意：使用一个区域的绝对值，区域可能是正面或负面，按照轮廓方向
    if( result->total == 4 &&
        fabs(cvContourArea(result,CV_WHOLE_SEQ)) > 1000 &&
        cvCheckContourConvexity(result) )
    {
        s = 0;
        cout<<endl;
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

        //将信息保存到Vec数组中便于输出
        //得到四个顶角的坐标
        CvPoint *temp[5];
        for(int i=0;i<4;i++)
            temp[i]=(CvPoint*)cvGetSeqElem(result, i);
        //计算两条边的长度,得到S
        double disA=getDistance(temp[0],temp[1]);
        double disB=getDistance(temp[1],temp[2]);
        if(abs(sqrt(disA)-sqrt(disB))<4)
            VecCol[vecNum].ID[1]=2;  //正方形是2
        else
            VecCol[vecNum].ID[1]=3;  //长方形是3
        VecCol[vecNum].S=sqrt(disA)*sqrt(disB);
        //计算中心点的位置
        CvPoint recCenter,mid,up;
//        VecCol[vecNum].X=recCenter.x=(temp[0]->x+temp[1]->x+temp[2]->x+temp[3]->x)/4.0;
//        VecCol[vecNum].Y=recCenter.y=(temp[0]->y+temp[1]->y+temp[2]->y+temp[3]->y)/4.0;
        VecCol[vecNum].X=recCenter.x=(temp[0]->x+temp[2]->x)/2.0;
        VecCol[vecNum].Y=recCenter.y=(temp[0]->y+temp[2]->y)/2.0;

        //可能有个bug.相同的剔除
        if(isExist(VecCol[vecNum].X,VecCol[vecNum].Y)){
            return 1;
        }

        //计算旋转角度
        mid.x=(temp[0]->x+temp[1]->x)/2;
        mid.y=(temp[0]->y+temp[1]->y)/2;
        up.x=mid.x,up.y=mid.y-20;

        double ang=angle(temp[0],&up,&mid); //mid为要求角度的点
        ang=180/PI*acos(ang);     //进行余弦值转化角度值
        if(ang>90)
            VecCol[vecNum].TH=ang-90;
        else
            VecCol[vecNum].TH=90-ang;
        cout<<endl<<"angle::"<<ang<<endl;

//        VecCol[vecNum].ID[0]=getColor(104,76);

        VecCol[vecNum].ID[0]=getColor(VecCol[vecNum].Y+10,VecCol[vecNum].X+10);
        //保存信息完成

        vecNum++;
        return 1;   //是矩形或者正方形
    }
    return 0;     //不是矩形,进行下一个形状的检测
}

void checkTest()
{//现实轮廓的大小和点的个数
    CvSeq* contours;
    mem_storage = cvCreateMemStorage(0);
    //--------------------------------
    CvSeq* squares = cvCreateSeq( 0, sizeof(CvSeq), sizeof(CvPoint), mem_storage );


    cvFindContours( thrImg, mem_storage, &contours, sizeof(CvContour),
                CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );
    while (contours) {
        areaS=fabs(cvContourArea(contours,CV_WHOLE_SEQ));
        cout<<"Points:"<<contours->total<<endl;
        cout<<"AreaS:"<<areaS<<endl;
        contours=contours->h_next;
    }
    //--------------------------------
    cvClearMemStorage(mem_storage);
}

void checkLine()
{
    CvSeq* contours;
    mem_storage = cvCreateMemStorage(0);
    CvSeq* squares = cvCreateSeq( 0, sizeof(CvSeq), sizeof(CvPoint), mem_storage );


    cvFindContours( thrImg, mem_storage, &contours, sizeof(CvContour),
                CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );
    while (contours) {
        areaS=fabs(cvContourArea(contours,CV_WHOLE_SEQ));
        cout<<"Points:"<<contours->total<<endl;
        cout<<"AreaS:"<<areaS<<endl;
//        cvFitLine( contours, CV_DIST_L1, 1, 0.001, 0.001, line );
        


        contours=contours->h_next;
    }
    cvClearMemStorage(mem_storage);
}

int checkRound(CvSeq *contours)
{//注意去掉奥利奥的情况，使用颜色
//----------进行圆和椭圆的判定---------------------------
    if(contours->total<5){    //轮廓中点的个数
        contours=contours->v_next;
//        continue;
        return 1;
    }

    double length = cvArcLength(contours); //得到指定的那个轮廓的周长
    //该函数有3个参数：序列，起点（默认计算整条曲线），是否封闭曲线
    areaS = cvContourArea(contours);  //得到指定的那个轮廓的面积    
    
    //根据序列找出最小面积外接圆
    CvPoint2D32f center;
    float radius;
    int a=cvMinEnclosingCircle(contours,&center,&radius);
    //圆必须是包含所有点，成功返回1，并且得到圆心和半径
    cout<<"center.x = "<<center.x<<endl;
    cout<<"center.y = "<<center.y<<endl;
    cout<<"radius =  "<<radius<<endl;
    cout<<"Area: ="<<areaS<<endl;
    cout<<"S = "<<PI*(radius-3)*(radius-3)<<endl;
    //进行圆轮廓的比对
    double calRadius=sqrt((areaS/PI));
    cout<<"Radiu = "<<calRadius<<endl;
    //画出最小外接圆，并显示
    cvCircle(originImg,cvPointFrom32f(center),cvRound(radius),CV_RGB(100,100,100));
    cvShowImage("dst",originImg);
    // cvWaitKey();



    //最小二乘法的椭圆拟合
    CvBox2D ellipse = cvFitEllipse2(contours);
    double tempS=areaS/PI;
    cout<<endl<<"tempS:"<<tempS<<endl;
    cout<<ellipse.size.height<<endl<<ellipse.size.width<<endl;
    cout<<"abs::"<<abs(tempS-ellipse.size.height*ellipse.size.width/4.0)<<endl;
    //用黄色在图上画椭圆,并显示
    cvEllipseBox(originImg,ellipse,CV_RGB(255,0,0));
    cvShowImage("dst",originImg);
    // cvWaitKey();



    //通过颜色过滤奥利奥，最小外接圆和

    //进行圆的判定
    if(abs(calRadius-radius)<4){
        if(isExist(center.x,center.y)){  //过滤掉逆时针的情况
            return 1;
        }


        VecCol[vecNum].ID[0]=getColor(center.y,center.x);
//        cvSetReal2D(thrImg,center.y,center.x+25, 255.0);
//        cvShowImage("yanse", thrImg);
        VecCol[vecNum].ID[1]=1;
        VecCol[vecNum].X=center.x;
        VecCol[vecNum].Y=center.y;
        VecCol[vecNum].S=(2*calRadius)*(2*calRadius);
        VecCol[vecNum].TH=0;
        vecNum++;
        return 1;
    }
    //进行椭圆的判定
    if(abs(tempS-ellipse.size.height*ellipse.size.width/4.0)<30){
//        cout<<"ellipse!!!"<<endl;
        if(isExist(ellipse.center.x,ellipse.center.y)){  //过滤掉逆时针的情况
            return 1;
        }

        VecCol[vecNum].ID[0]=getColor(ellipse.center.y,ellipse.center.x);
        VecCol[vecNum].ID[1]=4;
        VecCol[vecNum].X=ellipse.center.x;
        VecCol[vecNum].Y=ellipse.center.y;
        VecCol[vecNum].S=tempS;
        VecCol[vecNum].TH=ellipse.angle;
        vecNum++;
        return 1;
    }

    return 1;

//    CvRect rect = cvBoundingRect(contours,1);  //根据序列，返回轮廓外围矩形；
//    CvBox2D box = cvMinAreaRect2(contours,NULL);  //最小外围矩形

//    cout<<"Length = "<<length<<endl;
//    cout<<"Area = "<<areaS<<endl;

//    //外围矩形的顶点
//    CvPoint pt1,pt2;
//    pt1.x=rect.x;
//    pt1.y=rect.y;
//    pt2.x=rect.x+rect.width;
//    pt2.y=rect.y+rect.height;

////        IplImage *dst = cvCreateImage(cvGetSize(src),8,3); //目标图像为3通道图
////        cvZero(dst);
//    //使用红色给他画出轮廓
//    cvDrawContours(originImg,contours,CV_RGB(255,0,0),CV_RGB(255,0,0),0);
//    //使用绿色给他画出外界矩形
//    cvRectangle(originImg,pt1,pt2,CV_RGB(0,255,0));
//    //将图片显示
//    cvNamedWindow("dst",1);
//    cvShowImage("dst",originImg);




//    //绘用蓝色制外接最小矩形
//    CvPoint2D32f pt[4];
//    cvBoxPoints(box,pt);
//    for(int i = 0;i<4;++i){
//        cvLine(originImg,cvPointFrom32f(pt[i]),cvPointFrom32f(pt[((i+1)%4)?(i+1):0]),CV_RGB(0,0,255));
//    }
//    cvShowImage("dst",originImg);
//    cvWaitKey();
}


int checkOthers(CvSeq *contours)
{

}

void check()
{//在已经提取好的thrImg二值轮廓中一一判断每个轮廓
    CvSeq* contours;
    mem_storage = cvCreateMemStorage(0);
    //--------------------------------
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
        //判断轮廓的大小.过小的不要,也可直接用contours->totals的大小判断
        areaS=fabs(cvContourArea(contours,CV_WHOLE_SEQ));
        if( areaS< 100){
            contours=contours->h_next;
            continue;
        }
        //进行形状的判断k
        if(checkRec(contours,squares)){
            cout<<"Rec"<<endl;  //该轮廓是矩形
        }else if(checkRound(contours)){
            cout<<"Round"<<endl; //该轮廓是圆
        }else if(checkOthers(contours)){
             cout<<"Others"<<endl;
        }

//        cout<<"draw"<<endl;
//        drawSquares(  originImg,  squares );
//        cvShowImage("draw", originImg);
//        cvWaitKey(0);

#if 1  //傻瓜过滤掉最后一个也就是图片外框
        CvSeq *contemp;
        contemp=contours;
        contemp=contemp->h_next;
        contemp=contemp->h_next;
        if(contemp){
            contemp=contemp->h_next;
            contemp=contemp->h_next;
            if(contemp==0)
                break;
        }
#endif
        //两次去掉对称的情况
        contours = contours->h_next;
//        contours = contours->h_next;
    }
    //--------------------------------
    cvClearMemStorage(mem_storage);
}


//-------形状判断-------
