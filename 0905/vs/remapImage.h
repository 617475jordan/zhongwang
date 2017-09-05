#pragma once
#include <opencv_all.h>

class remapImage
{
public:
	remapImage();
	~remapImage();
	Mat Photo_Remap(Mat remap_image);
};