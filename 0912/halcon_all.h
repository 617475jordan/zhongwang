#pragma once 
#ifndef __APPLE__
#  include "HalconCpp.h"
#  include "HDevThread.h"
#  if defined(__linux__) && !defined(__arm__) && !defined(NO_EXPORT_APP_MAIN)
#    include <X11/Xlib.h>
#  endif
#else
#  ifndef HC_LARGE_IMAGES
#    include <HALCONCpp/HalconCpp.h>
#    include <HALCONCpp/HDevThread.h>
#  else
#    include <HALCONCppxl/HalconCpp.h>
#    include <HALCONCppxl/HDevThread.h>
#  endif
#  include <stdio.h>
#  include <HALCON/HpThread.h>
#  include <CoreFoundation/CFRunLoop.h>
#endif
#pragma comment(lib, "..\\lib\\halconcpp.lib")
#include <vector>
#include <ctime>
#include <time.h>
#include <math.h>
#include <iostream>
#include <ostream>
#include <fstream>
#include <cassert>
#include <string>
#include <stack>
#include <math.h>
using namespace HalconCpp;
using namespace std;