#pragma once
#include <halcon_all.h>
class halconObjectRecognition
{
public:
	halconObjectRecognition();
	~halconObjectRecognition();
	void initialize();
	void set_display_font(HTuple hv_WindowHandle, HTuple hv_Size, HTuple hv_Font, HTuple hv_Bold,
		HTuple hv_Slant);
	// Chapter: Graphics / Text
	// Short Description: This procedure displays 'Click 'Run' to continue' in the lower right corner of the screen. 
	void disp_continue_message(HTuple hv_WindowHandle, HTuple hv_Color, HTuple hv_Box);
	void action();
};

