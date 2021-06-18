#pragma once
#include <iostream>
#include "opencv2\objdetect\objdetect.hpp"
#include "opencv2\opencv.hpp"
#include <opencv2\imgcodecs.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\objdetect.hpp>

using namespace cv;

class FaceDetection
{
public:
	int getOverwriting();
	FaceDetection(int cameraId);
	void detectFace();
	int VideoDisplay();
	cv::Mat getFace();
	bool maskOn;

private:
	int cameraId;
	cv::Mat face, eye;
};

