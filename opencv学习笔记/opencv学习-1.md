# opencv学习笔记-1
&emsp;&emsp;嗯，短期内需要学习的内容比较多，在这里记录下来便于查找。这里记录了最基本的**图像显示**和**IplImage, CvMat, Mat**的关系和相互转换

## 图像的载入和显示
在[教程](http://blog.csdn.net/poem_qianmo/article/details/20537737)中有详细的样例解释。
### 1.imread函数
```
Mat imread(const string& filename, int flags=1 );
  ■ 第一个参数，const string&类型的filename，填我们需要载入的图片路径名。
  ■ 第二个参数，int类型的flags，为载入标识，它指定一个加载图像的颜色类型。
    - flags >0返回一个3通道的彩色图像。
    - flags =0返回灰度图像。
    - flags <0返回包含Alpha通道的加载的图像。
```
### 2.namedWindow函数
```
void namedWindow(const string& winname,int flags=WINDOW_AUTOSIZE );  
  ■ 第一个参数，const string&型的name，即填被用作窗口的标识符的窗口名称。
  ■ 第二个参数，int 类型的flags ，窗口的标识，可以填如下的值：
    - WINDOW_NORMAL设置了这个值，用户便可以改变窗口的大小（没有限制）
    - WINDOW_AUTOSIZE如果设置了这个值，窗口大小会自动调整以适应所显示的图像，并且不能手动改变窗口大小。
    - WINDOW_OPENGL 如果设置了这个值的话，窗口创建的时候便会支持OpenGL。
```
### 3.imshow函数
```
void imshow(const string& winname, InputArray mat);
  ■ 第一个参数，const string&类型的winname，填需要显示的窗口标识名称。
  ■ 第二个参数，InputArray 类型的mat，填需要显示的图像。
```
### 4.imwrite函数
```
bool imwrite(const string& filename,InputArray img, const vector<int>& params=vector<int>() );
  ■ 第一个参数，const string&类型的filename，填需要写入的文件名就行了，带上后缀，比如，“123.jpg”这样。
  ■ 第二个参数，InputArray类型的img，一般填一个Mat类型的图像数据就行了。
  ■ 第三个参数，const vector<int>&类型的params，表示为特定格式保存的参数编码，它有默认值vector<int>()，所以一般情况下不需要填写。而如果要填写的话，有下面这些需要了解的地方：
```


## 从摄像头获取视频流并显示输出
通过摄像头获取并且记录
### 从摄像头读取
```
//创建一个从摄像头获取的流，index对应就是摄像头的设备编号
CvCapture* cvCreateCameraCapture( int index );
//视频获取的数据结构，也就是上一个函数的返回，通过他来得到每帧图像
typedef struct CvCapture CvCapture;
eg.  CvCapture* capture = cvCreateCameraCapture(1);
//从摄像头读取
IplImage* cvQueryFrame( CvCapture* capture );
eg.  frame = cvQueryFrame(capture);
//显示可以使用上面的显示图片namedWindow和imshow函数
```
### 写入到视频文件
```
//创建文件的路径
char* writePath = "out.avi";
CvVideoWriter* writer = cvCreateVideoWriter(
    writePath,
    CV_FOURCC('P','I','M','1'),
    24,
    cvGetSize(frame)
);
//将frame的内容通过writer写入文件
cvWriteFrame(writer, frame);
```
### 样例代码
使用的是CV_FOURCC('M','J','P','G')（即Motion JPEG）格式，如果有问题可以换成CV_FOURCC('P','I','M','1')（即MPEG-1）试试。
```
#include "cv.h"
#include "highgui.h"

int main(int argc, char** argv) {
    cvNamedWindow("win");

    CvCapture* capture = cvCreateCameraCapture(1);
    IplImage* frame = cvQueryFrame(capture);
    // IplImage* frame = cvCreateImage(cvSize(256,180),8,1);

    char* writePath = "out.avi";
    CvVideoWriter* writer = cvCreateVideoWriter(
        writePath,
        CV_FOURCC('M','J','P','G'),
        10,
        cvGetSize(frame)
    );

    while(1) {
        frame = cvQueryFrame(capture);
        if(!frame) break;

        cvWriteFrame(writer, frame);
        cvShowImage("win", frame);

        char c = cvWaitKey(1000/24);
        if(c==27) break;
    }
    
    cvReleaseVideoWriter(&writer);
    cvReleaseCapture(&capture);
    cvDestroyWindow("win");
    return 0;
}
```

## IplImage, CvMat, Mat 的关系和相互转换
>[教程地址](http://blog.csdn.net/timidsmile/article/details/7648725)

&emsp;&emsp;opencv中常见的与图像操作有关的数据容器有Mat，cvMat和IplImage，这三种类型都可以代表和显示图像，但是，Mat类型侧重于计算，数学性较高，openCV对Mat类型的计算也进行了优化。而CvMat和IplImage类型更侧重于“图像”，opencv对其中的图像操作（缩放、单通道提取、图像阈值操作等）进行了优化。在opencv2.0之前，opencv是完全用C实现的，但是，IplImage类型与CvMat类型的关系类似于面向对象中的继承关系。实际上，CvMat之上还有一个更抽象的基类----CvArr，这在源代码中会常见。

-----

### Mat类型：矩阵类型，Matrix。
在openCV中，Mat是一个多维的密集数据数组。可以用来处理向量和矩阵、图像、直方图等等常见的多维数据。
Mat有3个重要的方法：
```
1、Mat mat = imread(const String* filename);            读取图像
2、imshow(const string frameName, InputArray mat);      显示图像
3、imwrite (const string& filename, InputArray img);    储存图像
Mat类型较CvMat与IplImage类型来说，有更强的矩阵运算能力，支持常见的矩阵运算。在计算密集型的应用当中，将CvMat与IplImage类型转化为Mat类型将大大减少计算时间花费。
```

A.Mat -> IplImage  
同样只是创建图像头，而没有复制数据。例： // 假设Mat类型的imgMat图像数据存在  
`IplImage pImg= IplImage(imgMat);`

B.Mat -> CvMat  
与IplImage的转换类似，不复制数据，只创建矩阵头。例： // 假设Mat类型的imgMat图像数据存在  
`CvMat cvMat = imgMat;`

---

>CvMat类型与IplImage类型：“图像”类型
在openCV中，Mat类型与CvMat和IplImage类型都可以代表和显示图像，但是，Mat类型侧重于计算，数学性较高，openCV对Mat类型的计算也进行了优化。而CvMat和IplImage类型更侧重于“图像”，openCV对其中的图像操作（缩放、单通道提取、图像阈值操作等）进行了优化。  
补充：IplImage由CvMat派生，而CvMat由CvArr派生即CvArr -> CvMat -> IplImage.CvArr用作函数的参数，无论传入的是CvMat或IplImage，内部都是按CvMat处理。  

### CvMat
A.CvMat-> IplImage  
```C++
IplImage* img = cvCreateImage(cvGetSize(mat),8,1);
cvGetImage(matI,img);
cvSaveImage("rice1.bmp",img);
```
B.CvMat->Mat
```
与IplImage的转换类似，可以选择是否复制数据。
Mat::Mat(const CvMat* m, bool copyData=false);
```
在openCV中，没有向量（vector）的数据结构。任何时候，但我们要表示向量时，用矩阵数据表示即可。但是，CvMat类型与我们在线性代数课程上学的向量概念相比，更抽象，比如CvMat的元素数据类型并不仅限于基础数据类型，比如，下面创建一个二维数据矩阵：  
`CvMat* cvCreatMat(int rows ,int cols , int type);`  

这里的type可以是任意的预定义数据类型，比如RGB或者别的多通道数据。这样我们便可以在一个CvMat矩阵上表示丰富多彩的图像了。

---

### IplImage
在类型关系上，我们可以说IplImage类型继承自CvMat类型，当然还包括其他的变量将之解析成图像数据。  
IplImage类型较之CvMat多了很多参数，比如depth和nChannels。在普通的矩阵类型当中，通常深度和通道数被同时表示，如用32位表示RGB+Alpha.但是，在图像处理中，我们往往将深度与通道数分开处理，这样做是OpenCV对图像表示的一种优化方案。  
IplImage的对图像的另一种优化是变量origin----原点。在计算机视觉处理上，一个重要的不便是对原点的定义不清楚，图像来源，编码格式，甚至操作系统都会对原地的选取产生影响。为了弥补这一点，openCV允许用户定义自己的原点设置。取值0表示原点位于图片左上角，1表示左下角。  
dataOrder参数定义数据的格式。有IPL_DATA_ORDER_PIXEL和IPL_DATA_ORDER_PLANE两种取值，前者便是对于像素，不同的通道的数据交叉排列，后者表示所有通道按顺序平行排列。  
IplImage类型的所有额外变量都是对“图像”的表示与计算能力的优化。  

A.IplImage -> Mat
```C++
IplImage* pImg = cvLoadImage("lena.jpg");
Mat img(pImg,0); // 0是不複製影像，也就是pImg與img的data共用同個記憶體位置，header各自有
```
B.IplImage -> CvMat
```C++
法1：CvMat mathdr, *mat = cvGetMat( img, &mathdr );
法2：CvMat *mat = cvCreateMat( img->height, img->width, CV_64FC3 );
    cvConvert( img, mat );
```
C.IplImage*-> BYTE*
```C++
BYTE* data= img->imageData;
``` 

CvMat和IplImage创建时的一个小区别：  
1、建立矩阵时，第一个参数为行数，第二个参数为列数。
CvMat* cvCreateMat( int rows, int cols, int type );  
2、建立图像时，CvSize第一个参数为宽度，即列数；第二个参数为高度，即行数。这 个和CvMat矩阵正好相反。  
IplImage* cvCreateImage(CvSize size, int depth, int channels );  
CvSize cvSize( int width, int height );
 
IplImage内部buffer每行是按4字节对齐的，CvMat没有这个限制

补充：
A.BYTE*-> IplImage*  
img= cvCreateImageHeader(cvSize(width,height),depth,channels);  
cvSetData(img,data,step);  
//首先由cvCreateImageHeader()创建IplImage图像头，制定图像的尺寸，深度和通道数；  
//然后由cvSetData()根据BYTE*图像数据指针设置IplImage图像头的数据数据，  
//其中step指定该IplImage图像每行占的字节数，对于1通道的IPL_DEPTH_8U图像，step可以等于width。