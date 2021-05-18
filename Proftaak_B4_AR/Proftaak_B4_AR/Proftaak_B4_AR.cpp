#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace cv;
using namespace std;


///////////////  Images  //////////////////////
VideoCapture cap(0);
Mat img;

void main() {

    //string path = "Resources/test.png";
    //Mat img = imread(path);
    CascadeClassifier faceCascade, eyeCascade, rightEyeCascade, leftEyeCascade;
    faceCascade.load("Resources/haarcascade_frontalface_default.xml");
    eyeCascade.load("Resources/haarcascade_eye.xml");
    //rightEyeCascade.load("Resources/haarcascade_right_eye.xml");
    //leftEyeCascade.load("Resources/haarcascade_left_eye.xml");
    if (faceCascade.empty()) { cout << "XML file not loaded" << endl; }
    //if (eyeCascade.empty()) { cout << "XML file not loaded" << endl; }



    while (true)
    {
        cap.read(img);

        vector<Rect> faces;
        vector<Rect> eyes;
        //vector<Rect> rightEyes;
        //vector<Rect> LeftEyes;
        faceCascade.detectMultiScale(img, faces, 1.1, 10);
        eyeCascade.detectMultiScale(img, eyes, 1.1, 10);
        //rightEyeCascade.detectMultiScale(img, rightEyes, 1.1, 10);
        //leftEyeCascade.detectMultiScale(img, LeftEyes, 1.1, 10);

        for (int i = 0; i < faces.size(); i++)
        {
            rectangle(img, faces[i].tl(), faces[i].br(), Scalar(255, 0, 255), 3);

        }

        for (int n = 0; n < eyes.size(); n++)
        {
            rectangle(img, eyes[n].tl(), eyes[n].br(), Scalar(0, 255, 255), 3);
        }

        imshow("Image", img);
        waitKey(1);

    }

}