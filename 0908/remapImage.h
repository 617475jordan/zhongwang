#pragma once
#include <opencv_all.h>

class remapImage
{
public:
	remapImage();
	~remapImage();
	Mat imageRemap(Mat remap_image);
};