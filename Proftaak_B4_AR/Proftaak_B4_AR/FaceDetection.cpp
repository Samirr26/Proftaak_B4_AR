#include "FaceDetection.h"
#include "FaceCutOut.h"

cv::Mat face;

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


int FaceDetection::VideoDisplay() {
    FaceCutOut faceCutOut;
    
    std::cout << "start recognizing..." << std::endl;
    std::string classifier = "Resources/haarcascade_frontalface_default.xml";
    std::string classifierEyes = "Resources/haarcascade_eye.xml";

    cv::CascadeClassifier face_cascade;
    cv::CascadeClassifier eyes_cascade;

    if (!face_cascade.load(classifier)) {
        std::cout << " Error loading file" << std::endl;
        return -1;
    }

    if (!eyes_cascade.load(classifierEyes)) {
        std::cout << " Error loading file" << std::endl;
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
        std::vector<cv::Rect> eyes;
        cv::Mat frame, graySacleFrame, original;

        cap >> original;

        if (!original.empty()) {
            //convert image to gray scale and equalize
            cvtColor(original, graySacleFrame, 6);

            //detect face in gray image
            face_cascade.detectMultiScale(graySacleFrame, faces, 1.1, 3, 0, cv::Size(90, 90));

            //detect eyes in the face
            eyes_cascade.detectMultiScale(graySacleFrame, faces, 1.1, 3, 0, cv::Size(90, 90));

            int width = 0, height = 0;
            cv::Mat seg_grabcut;
            //region of interest

            for (int i = 0; i < faces.size(); i++)
            {
                //region of interest
                cv::Rect face_i = faces[i];

                //crop the roi from grya image
                cv::Mat crop = original(face_i);

                //resizing the cropped image to suit to database image sizes
                cv::Mat face_resized;
                cv::resize(crop, face_resized, cv::Size(256, 256), 1.0, 1.0, cv::INTER_CUBIC);

                //drawing green rectagle in recognize face
                rectangle(original, face_i, CV_RGB(0, 255, 0), 1);

                if (!face_resized.empty())
                {
                    face = faceCutOut.GrabCut(face_resized);
                    if (!face.empty())
                    {
                        imshow("segmented result", face);
                    }

                }

            }

            for (int i = 0; i < eyes.size(); i++)
            {
                //region of interest
                cv::Rect eye_i = eyes[i];

                //crop the roi from grya image
                cv::Mat crop = original(eye_i);

                //resizing the cropped image to suit to database image sizes
                cv::Mat eye_resized;
                cv::resize(crop, eye_resized, cv::Size(64, 64), 1.0, 1.0, cv::INTER_CUBIC);

                //drawing green rectagle in recognize face
                rectangle(original, eye_i, CV_RGB(255, 0, 0), 1);

                if (!eye_resized.empty())
                {

                    

                }

            }

            //display to the winodw
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