#include "RegionGrowing.h"

/*
˵�������������㷨
���룺ԭͼ�����ӵ��ж�׼������׼��
���أ�����ͼ��
*/
Mat regionGrowing::RegionGrow(Mat MatIn, int iGrowPoint, int iGrowJudge)//iGrowPointΪ���ӵ���ж�������iGrowJudgeΪ��������  
{
	Mat MatGrowOld(MatIn.size(), CV_8UC1, Scalar(0));
	Mat MatGrowCur(MatIn.size(), CV_8UC1, Scalar(0));
	Mat MatGrowTemp(MatIn.size(), CV_8UC1, Scalar(0));
	//��ʼ��ԭʼ���ӵ�  
	for (int i = 0; i<MatIn.rows; i++)
	{
		for (int j = 0; j<MatIn.cols; j++)
		{
			int it = MatIn.at<uchar>(i, j);

			if (it <= iGrowPoint)//ѡȡ���ӵ㣬�Լ�����  
			{
				MatGrowCur.at<uchar>(i, j) = 255;
			}
		}
	}
	//Mat element = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	//	erode(MatGrowCur, MatGrowCur,element);
	//imshow("MatGrowCur", MatGrowCur);

	int DIR[8][12] = { { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, -1 }, { 0, 1 }, { 1, -1 }, { 1, 0 }, { 1, 1 } };
	Mat MatTemp = MatGrowOld - MatGrowCur;
	int iJudge = countNonZero(MatTemp);
	if (iJudge != 0)//MatGrowOld!=MatGrowCur �жϱ��κ��ϴε����ӵ��Ƿ�һ�������һ������ֹѭ��  
	{
		MatGrowTemp = MatGrowCur;
		for (int i = 0; i<MatIn.rows; i++)
		{
			for (int j = 0; j<MatIn.cols; j++)
			{
				if (MatGrowCur.at<uchar>(i, j) == 255 && MatGrowOld.at<uchar>(i, j) != 255)
				{
					for (int iNum = 0; iNum<9; iNum++)
					{
						int iCurPosX = i + DIR[iNum][0];
						int iCurPosY = j + DIR[iNum][1];
						if (iCurPosX>0 && iCurPosX<(MatIn.rows - 1) && iCurPosY>0 && iCurPosY<(MatIn.cols - 1))
						{
							if (abs(MatIn.at<uchar>(i, j) - MatIn.at<uchar>(iCurPosX, iCurPosY))<iGrowJudge)//�����������Լ�����  
							{
								MatGrowCur.at<uchar>(iCurPosX, iCurPosY) = 255;

							}
						}
					}
				}
			}
		}
		MatGrowOld = MatGrowTemp;
	}
	return MatGrowCur;
}