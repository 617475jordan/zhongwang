#ifndef   OPENCV_ALL
#define   OPENCV_ALL

#include "opencv2/opencv.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/internal.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv/highgui.h"
#include <ctime>
#include <time.h>
#include <math.h>
#include <iostream>
#include <ostream>
#if _DEBUG 
#pragma comment(lib, "..\\lib\\opencv_calib3d2410d.lib")
#pragma comment(lib, "..\\lib\\opencv_core2410d.lib")
#pragma comment(lib, "..\\lib\\opencv_contrib2410d.lib")
#pragma comment(lib, "..\\lib\\opencv_features2d2410d.lib")
#pragma comment(lib, "..\\lib\\opencv_flann2410d.lib")
#pragma comment(lib, "..\\lib\\opencv_gpu2410d.lib")
#pragma comment(lib, "..\\lib\\opencv_highgui2410d.lib")
#pragma comment(lib, "..\\lib\\opencv_imgproc2410d.lib")
#pragma comment(lib, "..\\lib\\opencv_legacy2410d.lib")
#pragma comment(lib, "..\\lib\\opencv_ml2410d.lib")
#pragma comment(lib, "..\\lib\\opencv_nonfree2410d.lib")
#pragma comment(lib, "..\\lib\\opencv_objdetect2410d.lib")
#pragma comment(lib, "..\\lib\\opencv_ocl2410d.lib")
#pragma comment(lib, "..\\lib\\opencv_photo2410d.lib")
#pragma comment(lib, "..\\lib\\opencv_stitching2410d.lib")
#pragma comment(lib, "..\\lib\\opencv_superres2410d.lib")
#pragma comment(lib, "..\\lib\\opencv_ts2410d.lib")
#pragma comment(lib, "..\\lib\\opencv_video2410d.lib")
#pragma comment(lib, "..\\lib\\opencv_videostab2410d.lib")

#else
#pragma comment(lib, "..\\lib\\opencv_calib3d2410.lib")
#pragma comment(lib, "..\\lib\\opencv_core2410.lib")
#pragma comment(lib, "..\\lib\\opencv_contrib2410.lib")
#pragma comment(lib, "..\\lib\\opencv_features2d2410.lib")
#pragma comment(lib, "..\\lib\\opencv_flann2410.lib")
#pragma comment(lib, "..\\lib\\opencv_gpu2410.lib")
#pragma comment(lib, "..\\lib\\opencv_highgui2410.lib")
#pragma comment(lib, "..\\lib\\opencv_imgproc2410.lib")
#pragma comment(lib, "..\\lib\\opencv_legacy2410.lib")
#pragma comment(lib, "..\\lib\\opencv_ml2410.lib")
#pragma comment(lib, "..\\lib\\opencv_nonfree2410.lib")
#pragma comment(lib, "..\\lib\\opencv_objdetect2410.lib")
#pragma comment(lib, "..\\lib\\opencv_ocl2410.lib")
#pragma comment(lib, "..\\lib\\opencv_photo2410.lib")
#pragma comment(lib, "..\\lib\\opencv_stitching2410.lib")
#pragma comment(lib, "..\\lib\\opencv_superres2410.lib")
#pragma comment(lib, "..\\lib\\opencv_ts2410.lib")
#pragma comment(lib, "..\\lib\\opencv_video2410.lib")
#pragma comment(lib, "..\\lib\\opencv_videostab2410.lib")

#endif
using namespace cv;
using namespace std;


#endif