#include <iostream>
#include "opencv2\objdetect\objdetect.hpp"
#include "opencv2\opencv.hpp"

using namespace std;
using namespace cv;

const unsigned int BORDER = 5;

Mat GrabCut(Mat image) {        
    cv::Mat result, bgModel, fgModel, downsampled;
    const auto fullSize = image.size();

    resize(image, downsampled, Size(), 0.5, 0.5);
    cv::Rect rectangle(BORDER, BORDER, downsampled.cols, downsampled.rows);

    // GrabCut segmentation
    cv::grabCut(downsampled,    // input image
        result,   // segmentation result
        rectangle,// rectangle containing foreground
        bgModel, fgModel, // models
        1,        // number of iterations
        cv::GC_INIT_WITH_RECT); // use rectangle

    // Get the pixels marked as likely foreground
    cv::compare(result, cv::GC_PR_FGD, result, cv::CMP_EQ);
    // upsample the resulting mask
    cv::Mat resultUp, resultDil;
    cv::dilate(result, resultDil, getStructuringElement(MORPH_RECT, Size(1,1) ));
    cv::pyrUp(result, resultDil, fullSize);
    // Generate output image
    cv::Mat foreground(fullSize, CV_8UC3, cv::Scalar(255, 255, 255));
    image.copyTo(foreground, resultDil); // bg pixels not copied
    return foreground;
}

int  VideoDisplay() {
    std::cout << "start recognizing..." << endl;
    string classifier = "Resources/haarcascade_frontalface_default.xml";

    CascadeClassifier face_cascade;

    if (!face_cascade.load(classifier)) {
        cout << " Error loading file" << endl;
        return -1;
    }
    VideoCapture cap(0);

    if (!cap.isOpened())
    {
        cout << "exit" << endl;
        return -1;
    }

    while (true)
    {
        vector<Rect> faces;
        Mat frame, graySacleFrame, original;

        cap >> original;

        if (!original.empty()) {
            //convert image to gray scale and equalize
            cvtColor(original, graySacleFrame, 6);

            //detect face in gray image
            face_cascade.detectMultiScale(graySacleFrame, faces, 1.1, 3, 0, cv::Size(90, 90));

            int width = 0, height = 0;
            cv::Mat seg_grabcut;
            //region of interest

            for (int i = 0; i < faces.size(); i++)
            {
                //region of interest
                Rect face_i = faces[i];

                //crop the roi from grya image
                Mat crop = original(face_i);

                //resizing the cropped image to suit to database image sizes
                Mat face_resized;
                cv::resize(crop, face_resized, Size(256, 256), 1.0, 1.0, INTER_CUBIC);

                //drawing green rectagle in recognize face
                rectangle(original, face_i, CV_RGB(0, 255, 0), 1);

                if (!face_resized.empty())
                {
                    seg_grabcut = GrabCut(face_resized);
                    if (!seg_grabcut.empty())
                    {
                        imshow("segmented result", seg_grabcut);
                    }

                }

            }

            //display to the winodw
            cv::imshow("Image", original);
        }

        if (waitKey(30) >= 0) 
            break;
    }
    return 0;
}

int main()
{
    VideoDisplay();
    return 0;
}