### cvStartReadSeq函数
函数 cvStartReadSeq 初始化读取部分的状态。  
**函数定义**  
`void cvStartReadSeq( const CvSeq* seq, CvSeqReader* reader, int reverse=0 )`  
**函数参数**
- `seq`  序列
- `reader`  读取部分的状态； 由该函数初始化
- `reverse`  决定遍历序列的方向。如果 reverse　为０，则读取顺序被定位从序列头部元素开始，否则从尾部开始读取

顺序读取可通过调用宏 CV_READ_SEQ_ELEM( read_elem, reader )，逆序读取可通过调用宏CV_REV_READ_SEQ_ELEM( read_elem, reader )。这两个宏都将序列元素读进read_elem中， 并将指针移到下一个元素。下面代码显示了如何去使用reader 和 writer.

### ApproxPoly函数
用指定精度逼近多边形曲线  
**函数定义** 
`CvSeq* cvApproxPoly( const void* src_seq, int header_size, CvMemStorage* storage,                       int method, double parameter, int parameter2=0 );`  
**函数参数**  
- `src_seq`  点集数组序列
- `header_size`  逼近曲线的头尺寸
- `storage`  逼近轮廓的容器。如果为 NULL， 则使用输入的序列
- `method`   逼近方法。目前仅支持 CV_POLY_APPROX_DP， 对应 Douglas-Peucker 算法.
- `parameter`  方法相关参数。对 CV_POLY_APPROX_DP它是指定的逼近精度
- `parameter2`  如果 src_seq 是序列，它表示要么逼近单个序列，要么在 src_seq 的同一个或低级层次上逼近所有序列 (参考 cvFindContours 中对轮廓继承结构的描述). 如果 src_seq 是点集的数组 (CvMat*) ， 参数指定曲线是闭合 (parameter2!=0) 还是非闭合 (parameter2=0).

函数cvApproxPoly逼近一个或多个曲线，并返回逼近结果。对多个曲线的逼近，生成的树将与输入的具有同样的结构。(1:1 的对应关系).
**使用实例**
在`第一阶段完成.cpp`中检测矩形轮廓的时候
```C++
    //在
    result = cvApproxPoly( contours, sizeof(CvContour), mem_storage,
        CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0 );
```