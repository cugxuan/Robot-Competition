#ifndef CHECK_H
#define CHECK_H

#include "origin.h"
#include "cal.h"   //用来计算颜色等

int checkRec(CvSeq *contours,CvSeq *&squares);
int checkRound(CvSeq *contours);
int checkOthers(CvSeq *contours);
void check();
void checkTest();

#endif // CHECK_H
