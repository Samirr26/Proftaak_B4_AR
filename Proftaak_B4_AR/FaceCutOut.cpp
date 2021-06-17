#include "FaceCutOut.h"

const unsigned int BORDER = 5;

FaceCutOut::FaceCutOut() {
    
}


cv::Mat FaceCutOut::GrabCut(cv::Mat image) {
    cv::Mat result, bgModel, fgModel, downsampled;
    const auto fullSize = image.size();

    resize(image, downsampled, cv::Size(), 0.5, 0.5);
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
    cv::dilate(result, resultDil, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 1)));
    cv::pyrUp(result, resultDil, fullSize);
    // Generate output image
    cv::Mat foreground(fullSize, CV_8UC3, cv::Scalar(255, 255, 255));
    image.copyTo(foreground, resultDil); // bg pixels not copied
    return foreground;
}
