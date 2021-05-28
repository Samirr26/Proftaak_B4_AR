#pragma once
#include <iostream>
#include "opencv2\objdetect\objdetect.hpp"
#include "opencv2\opencv.hpp"

class FaceCutOut
{
	public:
		FaceCutOut();
		cv::Mat GrabCut(cv::Mat image);
		cv::Mat getFace();
		
	
};

