#include <iostream>
#include "opencv2\objdetect\objdetect.hpp"
#include "opencv2\opencv.hpp"
#include <thread>

#include "FaceDetection.h"

using namespace std;
using namespace cv;

FaceDetection faceDetection(0);

void faceDetectionTask() {

    faceDetection.VideoDisplay();
}

int main()
{

    std::thread faceDetectThread(faceDetectionTask);
    while (true) {

        //faceDetection.getFace();

    }

    return 0;
}