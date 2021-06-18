#include "FaceDetection.h"
#include "FaceCutOut.h"
#include "ThreadManagement.h"
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

cv::Mat face;

FaceDetection::FaceDetection(int cameraId) {
	this->cameraId = cameraId;
	this->face = NULL;
    maskOn = false;
    changeTexture = false;
    overwriting = false;
}

int FaceDetection::VideoDisplay() {
    FaceCutOut faceCutOut;
    maskOn = false;
    std::cout << "start recognizing!" << std::endl;
    static String faceCascadePath = "Resources/haarcascade_frontalface_default.xml";
    static String maskCascadePath = "Resources/cascade.xml";

    cv::CascadeClassifier face_cascade;
    cv::CascadeClassifier mask_cascade;

    if (!face_cascade.load(faceCascadePath)) {
        std::cout << " Error loading file face" << std::endl;
        return -1;
    }

    if (!mask_cascade.load(maskCascadePath)) {
        std::cout << " Error loading file mask" << std::endl;
        return -1;
    }

    cv::VideoCapture cap(0);

    if (!cap.isOpened())
    {
        std::cout << "exit" << std::endl;
        return -1;
    }

    while (true)
    {
        std::vector<cv::Rect> faces;
        std::vector<cv::Rect> mask;
        cv::Mat frame, graySacleFrame, original;

        cap >> original;

        if (!original.empty()) {
            //convert image to gray scale and equalize
            cvtColor(original, graySacleFrame, 6);

            //detect face in gray image
            face_cascade.detectMultiScale(graySacleFrame, faces, 1.1, 10);

            //detect eyes in the face
            mask_cascade.detectMultiScale(graySacleFrame, mask, 1.1, 10);

            int width = 0, height = 0;
            cv::Mat seg_grabcut;
            //region of interest

            for (int i = 0; i < faces.size(); i++)
            {
                //region of interest
                cv::Rect face_i = faces[i];

                //crop the roi from gray image
                cv::Mat crop = original(face_i);

                //resizing the cropped image to suit to database image sizes
                cv::Mat face_resized;
                cv::resize(crop, face_resized, cv::Size(256, 256), 1.0, 1.0, cv::INTER_CUBIC);

                //drawing rectagle in recognized face
                cv::rectangle(original, faces[i].tl(), faces[i].br(), Scalar(255, 0, 255), 3);

                if (!face_resized.empty())
                {
                    //grabcut the picture
                    face = faceCutOut.GrabCut(face_resized);
                    if (!face.empty())
                    {
                        changeTexture = false;
                        imwrite("Resources/Faces/detectedFace.png", face_resized);
                        maskOn = false;                   
                    }

                }

            }

            for (int i = 0; i < mask.size(); i++)
            {
                //region of interest
                cv::Rect eye_i = mask[i];

                //crop the roi from gray image
                cv::Mat crop = original(eye_i);

                //resizing the cropped image 
                cv::Mat eye_resized;
                cv::resize(crop, eye_resized, cv::Size(256, 256), 1.0, 1.0, cv::INTER_CUBIC);

                //drawing rectagle in recognized eyes
                cv::rectangle(original, mask[i].tl(), mask[i].br(), Scalar(0, 0, 255), 3);

                if (!eye_resized.empty())
                {
                    //grabcut the picture
                    face = faceCutOut.GrabCut(eye_resized);
                    if (!face.empty())
                    {                       
                        changeTexture = false;
                        imwrite("Resources/Faces/detectedFaceWithMask.png", eye_resized);
                        maskOn = true;
                      
                    }

                }
            }

            //display to the window
            cv::imshow("Image", original);
        }

        if (cv::waitKey(30) >= 0)
            break;
    }
    return 0;
}

cv::Mat getFace() {
    return face;
}