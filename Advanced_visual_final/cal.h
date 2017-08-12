#ifndef CAL_H
#define CAL_H

#include "origin.h"

double angle( CvPoint* pt1, CvPoint* pt2, CvPoint* pt0 );
double getDistance(CvPoint *pt1,CvPoint *pt2);
void drawSquares( IplImage* img, CvSeq* squares );
int getColor(int x,int y);
int color_test(int x,int y);

#endif // CAL_H
