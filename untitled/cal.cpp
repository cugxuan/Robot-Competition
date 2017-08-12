#include "cal.h"

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



//-------辅助形状判断,距离/角度-------
// pt0->pt1 和 pt0->pt2，找到向量之间的角度余弦
double angle( CvPoint* pt1, CvPoint* pt2, CvPoint* pt0 )
{
    double dx1 = abs(pt1->x - pt0->x);
    double dy1 = abs(pt1->y - pt0->y);
    double dx2 = abs(pt2->x - pt0->x);
    double dy2 = abs(pt2->y - pt0->y);
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
float boxAngle(CvBox2D box)
{
    if(box.size.width>box.size.height){
        //说明角度是正的
        return -box.angle;
    }else{
        //否则角度是负的
        box.angle=-(90+box.angle);
        return box.angle;
    }
}

//-------辅助形状判断,距离/角度-------

//--------color test-------
void dfs()
{

}

//int getColor(int x, int y)
//{//黑、红、黄、绿、蓝5种（ID依次为1, 2, 3, 4, 5）
//    CvScalar s=cvGet2D(originHSV,x,y);  //s[0]-s[2],HSV
//    cvShowImage("originHSV", originHSV);

//    int i;
//    IplImage* imgChannel[4] = { 0, 0, 0, 0 };
//    IplImage* dst = cvCreateImage( cvGetSize( originHSV ), IPL_DEPTH_8U, 3 );
//    for( i = 0; i < originHSV -> nChannels; i++ )
//    {
//        imgChannel[i] = cvCreateImage( cvGetSize( originHSV ), IPL_DEPTH_8U, 1 );  //要求单通道图像才能直方图均衡化
//    }
//    //通道分离
//    cvSplit( originHSV, imgChannel[0], imgChannel[1], imgChannel[2], imgChannel[3] );
//    for( i = 0; i < dst -> nChannels; i++ )
//    {
//        //直方图均衡化，原始图像和目标图像必须是单通道
//        cvEqualizeHist( imgChannel[i], imgChannel[i] );
//    }
//    //通道组合
//    cvMerge( imgChannel[0], imgChannel[1], imgChannel[2], imgChannel[3], dst );
//    cvNamedWindow( "src", 1 );
//    cvShowImage( "src", originHSV );
//    cvNamedWindow( "Equalize", 1 );
//    cvShowImage( "Equalize", dst );
//    s=cvGet2D(dst,5,5);  //s[0]-s[2],HSV


//    if(s.val[0]>=0&&s.val[0]<=180&&s.val[1]>=0&&s.val[1]<=255&&s.val[2]>=0&&s.val[2]<=46)
//        return 1;
//    else if( (s.val[0]>=140&&s.val[0]<=180)
//            &&s.val[1]>=75&&s.val[1]<=255&&s.val[2]>=75&&s.val[2]<=255)
//        return 2;
//    else if(s.val[0]>=26&&s.val[0]<=34&&s.val[1]>=43&&s.val[1]<=255&&s.val[2]>=46&&s.val[2]<=255)
//        return 3;
//    else if(s.val[0]>=35&&s.val[0]<=77&&s.val[1]>=43&&s.val[1]<=255&&s.val[2]>=46&&s.val[2]<=255)
//        return 4;
//    else if(s.val[0]>=100&&s.val[0]<=124&&s.val[1]>=43&&s.val[1]<=255&&s.val[2]>=46&&s.val[2]<=255)
//        return 5;
//    return 0;
//}

//int getColor(int x, int y)
//{//黑、红、黄、绿、蓝5种（ID依次为1, 2, 3, 4, 5）
//    CvScalar s=cvGet2D(originHSV,x,y);  //s[0]-s[2],HSV
//    if(s.val[0]>=0&&s.val[0]<=180&&s.val[1]>=0&&s.val[1]<=255&&s.val[2]>=0&&s.val[2]<=46)
//        return 1;
//    else if( ((s.val[0]>=0&&s.val[0]<=10)||(s.val[0]>=156&&s.val[0]<=180))
//            &&s.val[1]>=43&&s.val[1]<=255&&s.val[2]>=46&&s.val[2]<=255)
//        return 2;
//    else if(s.val[0]>=26&&s.val[0]<=34&&s.val[1]>=43&&s.val[1]<=255&&s.val[2]>=46&&s.val[2]<=255)
//        return 3;
//    else if(s.val[0]>=35&&s.val[0]<=77&&s.val[1]>=43&&s.val[1]<=255&&s.val[2]>=46&&s.val[2]<=255)
//        return 4;
//    else if(s.val[0]>=100&&s.val[0]<=124&&s.val[1]>=43&&s.val[1]<=255&&s.val[2]>=46&&s.val[2]<=255)
//        return 5;
//    return 0;
//}

int isColorPure(int x,int y)
{//传值按照x，y
    int color[16];
    int test[6]={0,1,2,3,4,5};
    int test_count[6]={0};
    int final_color;

    color[0]=getColor(y+9,x);
    color[1]=getColor(y+6,x+6);
    color[2]=getColor(y,x+9);
    color[3]=getColor(y-6,x+6);
    color[4]=getColor(y-9,x);
    color[5]=getColor(y-6,x-6);
    color[6]=getColor(y,x-9);
    color[7]=getColor(y+6,x-6);

    color[8]=getColor(y+18,x);
    color[9]=getColor(y+13,x+12);
    color[10]=getColor(y,x+18);
    color[11]=getColor(y-13,x+12);
    color[12]=getColor(y-18,x);
    color[13]=getColor(y-13,x-12);
    color[14]=getColor(y,x-18);
    color[15]=getColor(y+13,x-12);
    for(int i=0;i<6;i++)
    {
        for(int j=0;j<16;j++)
        {
           if(test[i]==color[j])
               test_count[i]++;
        }
    }
    for(int k=0;k<6;k++)
    {
        if(test_count[k]>14)
        {
            final_color=test[k];
            break;
        }
        else final_color=-1;
    }
    return final_color;
}

int getColor(int x,int y)
{//传值按照y，x
//    imshow("mat",originMat);
    // 源图像载入及判断
    if( !originMat.data )
       return -1;
    Mat tempImage = originMat.clone();
    int watch[3],flag[3];
    flag[0]=flag[1]=flag[2]=0;
    for(int i=0;i<3;i++)
        watch[i] = originMat.at<Vec3b>(x, y)[i];

    for(int i=0;i<3;i++){
        if(watch[i]>colorRecgnize)   //
            flag[i]=1;
    }//BGR蓝绿红

    cvSetReal2D(thrImg, x, y, 255.0);
    cvShowImage("colorJudge", thrImg);
//    cvSet2D(originImg,x,y, cvScalar(0, 255, 0, 0)); //绘制来查看检测点的位置
//    cvShowImage("yanse", originImg);
    if(flag[0]==0&&flag[1]==1&&flag[2]==1){
        return 3;//黄
    }else if(flag[0]==1&&flag[1]==0&&flag[2]==0){
        return 5;//蓝
    }else if(flag[0]==0&&flag[1]==1&&flag[2]==0){
        return 4;//绿
    }else if(flag[0]==0&&flag[1]==0&&flag[2]==1){
        return 2;//红
    }else if(flag[0]==0&&flag[1]==0&&flag[2]==0){
        return 1;//黑
    }
    return 0;
}
//--------color test-------
