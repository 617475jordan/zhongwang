#ifndef _REGIONGROWING_H_
#define _REGIONGROWING_H_
#include <opencv_all.h>

class regionGrowing
{
public:
	Mat RegionGrow(Mat MatIn, int iGrowPoint, int iGrowJudge);//iGrowPoint为种子点的判断条件，iGrowJudge为生长条件  

};
#endif