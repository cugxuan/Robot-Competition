#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
//#pragma comment(linker,"/subsystem:windows /ENTRY:mainCRTStartup")

using namespace std;
using namespace cv;
int main()
{
    // 源图像载入及判断
    Mat srcImage = imread ("/1.png"); //Red_pic是一幅全红的图
    if( !srcImage.data )
       return 1;

    Mat tempImage = srcImage.clone();

    int watch1,watch2,watch3;
    int x=60,y=100;
    int watch[3],flag[3];
    flag[0]=flag[1]=flag[2]=0;
    for(int i=0;i<3;i++)
     watch[i] = tempImage.at<Vec3b>(x, y)[i];
//    cout<<watch1<<endl;
//    cout<<watch2<<endl;
//    cout<<watch3<<endl;
    for(int i=0;i<3;i++){
        if(watch[i]>190)
            flag[i]=1;
    }//BGR蓝绿红
    if(flag[1]==1&&flag[2]==1){
        cout<<"3"<<endl;   //黄
    }else if(flag[0]==1){
        cout<<"5"<<endl;   //蓝
    }else if(flag[1]==1){
        cout<<"4"<<endl;   //绿
    }else if(flag[2]==1){
        cout<<"2"<<endl;   //红
    }else{
        cout<<"1"<<endl;   //黑
    }

    return 0;
}

