#include <iostream>
#include <cmath>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv/cv.h>

using namespace std;
using namespace cv;
double PI=atan(1.0)*4;
//-------变量定义--------
IplImage* originImg;   //初始图像
IplImage* gaussImg;    //高斯过滤图像
IplImage* thrImg;      //二值化图像
IplImage* contourImg;  //轮廓图像
Mat originMat;         //Mat型原图

CvMemStorage* mem_storage;
CvSeq *first_contour = NULL, *c = NULL;
CvPoint pt[4];
char *filename[]={/*"/1.png","/2.png",*/"/h-5.png",/*"/2.png",*/0};
const int colorRecgnize=190; //颜色识别的限度

struct Countour{
    //圆形、正方形、长方形、椭圆形4种（ID依次为1, 2, 3, 4）
    //可乐罐、口香糖、方便桶面、饼干盒4种（ID依次为81, 82, 83, 84）
    //黑、红、黄、绿、蓝5种（ID依次为1, 2, 3, 4, 5）
    int ID[2];
    int X,Y,S;
    double TH;   //识别物体朝向角
}VecCol[15];
int vecNum;
//-------变量定义--------

//-------初始化和高斯过滤-------
void init(int T)
{//进行图片的初始化
    vecNum=0;
    originImg=cvLoadImage(filename[T]);
    originMat=imread(filename[T]);
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
//-------辅助形状判断,距离/角度-------

//--------color test-------
int getColor(int x,int y)
{
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

    cvSet2D(originImg,x,y, cvScalar(0, 255, 0, 0));
    cvShowImage("yanse", originImg);
    if(flag[0]==0&&flag[1]==1&&flag[2]==1){
        return 3;//黄
    }else if(flag[0]==1&&flag[1]==0&&flag[2]==0){
        return 5;//蓝
    }else if(flag[0]==0&&flag[1]==1&&flag[2]==0){
        return 4;//绿
    }else if(flag[0]==0&&flag[1]==0&&flag[2]==1){
        return 2;//红
    }else{
        return 1;//黑
    }
}
//--------color test-------

//-------形状判断-------
int checkRec(CvSeq *contours,CvSeq *&squares)
{
    int i;
    double s,t;
    CvSeq* result;
    // 使用近似轮廓精度与轮廓周长成比例
    result = cvApproxPoly( contours, sizeof(CvContour), mem_storage,
        CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0 );
    if(0){//如果是图像框,则跳过
        return 1;
    }
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
        for(int i=0;i<vecNum;i++){
            if(VecCol[vecNum].X==VecCol[i].X&&VecCol[vecNum].Y==VecCol[i].Y)
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

int checkRound(CvSeq *contours)
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
        //判断轮廓的大小.过小的不要
        if(fabs(cvContourArea(contours,CV_WHOLE_SEQ)) < 1000){
            contours=contours->h_next;
            continue;
         }
         //进行形状的判断
         if(checkRec(contours,squares)){
            cout<<"Rec"<<endl;  //该轮廓是矩形
//            continue;
        }else if(checkRound(contours)){
            cout<<"Round"<<endl; //该轮廓是圆
//            continue;
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
    cvClearMemStorage(mem_storage);
}
//-------形状判断-------




//-------流程控制&释放内存-------
void PrintC()
{
    if(vecNum==0){
        cout<<"没有检测到形状"<<endl;
        return ;
    }else{
        cout<<"检测到"<<vecNum<<"个物体"<<endl;
        for(int i=0;i<vecNum;i++){
            cout<<"第"<<i<<"轮廓为"<<endl;
            cout<<"ID:"<<VecCol[i].ID[0]<<VecCol[i].ID[1]<<endl;
            cout<<"X:"<<VecCol[i].X<<" Y:"<<VecCol[i].Y<<endl;
            cout<<"S:"<<VecCol[i].S<<endl;
            cout<<"angle:"<<VecCol[i].TH<<endl;
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
void control()
{//流程控制
    int T=0;
    while (filename[T][0]!=0) {
        init(T);
        Gau();
        thres();
        filter();

        check();
        PrintC();
        release();

        cvWaitKey(0);
        T++;
    }
}
//-------流程控制&释放内存-------

int main()
{
    control();
}


