#pragma once
#include <iostream>
#include "opencv2\objdetect\objdetect.hpp"
#include "opencv2\opencv.hpp"

class FaceDetection
{
public:
	FaceDetection(int cameraId);
	void detectFace();

private:
	int cameraId;
	cv::Mat face;
};

