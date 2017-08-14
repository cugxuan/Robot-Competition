#ifndef CAL_H
#define CAL_H

#include "origin.h"

double angle( CvPoint* pt1, CvPoint* pt2, CvPoint* pt0 );
double getDistance(CvPoint *pt1,CvPoint *pt2);
float boxAngle(CvBox2D box);
void drawSquares( IplImage* img, CvSeq* squares );

int getColor(int x,int y);
int isColorPure(int x,int y);
int isColorPure(int x,int y,int depth);

#endif // CAL_H
