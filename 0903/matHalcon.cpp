﻿#include "matHalcon.h"


matHalcon::matHalcon()
{
}


matHalcon::~matHalcon()
{
}

Mat matHalcon::HImageToIplImage(HObject &Hobj)
{
	//get_grayval(Image : : Row, Column : Grayval)  

	cv::Mat pImage;
	HTuple htChannels;
	HTuple     width, height;
	width = height = 0;
	//×ª»»Í¼Ïñ¸ñÊ½    
	ConvertImageType(Hobj, &Hobj, "byte");
	CountChannels(Hobj, &htChannels);
	HTuple cType;
	HTuple grayVal;

	if (htChannels.I() == 1)
	{
		GetImageSize(Hobj, &width, &height);

		pImage = cv::Mat(height, width, CV_8UC1);
		pImage = Mat::zeros(height, width, CV_8UC1);

		for (int i = 0; i < height.I(); ++i)
		{
			for (int j = 0; j < width.I(); ++j)
			{
				GetGrayval(Hobj, i, j, &grayVal);
				pImage.at<uchar>(i, j) = (uchar)grayVal.I();
			}

		}

	}
	else if (htChannels.I() == 3)
	{
		GetImageSize(Hobj, &width, &height);
		pImage = cv::Mat(height, width, CV_8UC3);
		for (int row = 0; row < height.I(); row++)
		{
			for (int col = 0; col < width.I(); col++)
			{
				GetGrayval(Hobj, row, col, &grayVal);

				pImage.at<uchar>(row, col * 3) = (uchar)grayVal[2].I();
				pImage.at<uchar>(row, col * 3 + 1) = (uchar)grayVal[1].I();
				pImage.at<uchar>(row, col * 3 + 2) = (uchar)grayVal[0].I();

			}
		}

	}

	return pImage;
}

HObject matHalcon::IplImageToHImage(cv::Mat& pImage)
{
	HObject Hobj;
	if (3 == pImage.channels())
	{
		cv::Mat pImageRed, pImageGreen, pImageBlue;
		std::vector<cv::Mat> sbgr(3);
		cv::split(pImage, sbgr);

		int length = pImage.rows * pImage.cols;
		uchar *dataBlue = new uchar[length];
		uchar *dataGreen = new uchar[length];
		uchar *dataRed = new uchar[length];

		int height = pImage.rows;
		int width = pImage.cols;
		for (int row = 0; row < height; row++)
		{
			uchar* ptr = pImage.ptr<uchar>(row);
			for (int col = 0; col < width; col++)
			{
				dataBlue[row * width + col] = ptr[3 * col];
				dataGreen[row * width + col] = ptr[3 * col + 1];
				dataRed[row * width + col] = ptr[3 * col + 2];
			}
		}

		GenImage3(&Hobj, "byte", width, height, (Hlong)(dataRed), (Hlong)(dataGreen), (Hlong)(dataBlue));
		delete[] dataRed;
		delete[] dataGreen;
		delete[] dataBlue;
	}
	else if (1 == pImage.channels())
	{
		int height = pImage.rows;
		int width = pImage.cols;
		uchar *dataGray = new uchar[width * height];
		memcpy(dataGray, pImage.data, width * height);
		GenImage1(&Hobj, "byte", width, height, (Hlong)(dataGray));
		delete[] dataGray;
	}

	return Hobj;
}
