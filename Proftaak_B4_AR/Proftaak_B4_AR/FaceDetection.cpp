#include "FaceDetection.h"

FaceDetection::FaceDetection(int cameraId) {
	this->cameraId = cameraId;
	this->face = NULL;
}

void FaceDetection::detectFace() {
	std::cout << "start recognizing..." << std::endl;
	std::string classifier = "Resources/haarcascade_frontalface_default.xml";

	cv::VideoCapture videoCapture(cameraId);
	cv::Mat testImage;

	while (true) {
		videoCapture.read(testImage);
		cv::imshow("Image", testImage);
		cv::waitKey(1);
	}
}