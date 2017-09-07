#include "remapImage.h"

remapImage::remapImage()
{

}
remapImage::~remapImage()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Photo remap. </summary>
///
/// <remarks>	Admin, 2017/8/2. </remarks>
///
/// <param name="remap_image">	The remap image. </param>
///
/// <returns>	A Mat. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

Mat remapImage::Photo_Remap(Mat remap_image)
{
	Mat map_x, map_y;
	int i, j;
	vector<Vec4i> photo_line_num0;//photo_line_num1;
	map_x.create(remap_image.size(), CV_32FC1);
	map_y.create(remap_image.size(), CV_32FC1);

	for (i = 0; i < remap_image.rows; i++)//ÐÐ
	{
		for (j = 0; j < remap_image.cols; j++)//ÁÐ
		{
			map_y.at<float>(i, j) = static_cast<float>(i);
			map_x.at<float>(i, j) = static_cast<float>(remap_image.cols - j);
		}
	}
	remap(remap_image, remap_image, map_x, map_y, INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));
	return remap_image;
}
