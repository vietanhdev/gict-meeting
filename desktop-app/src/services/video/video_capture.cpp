#include "video/video_capture.h"

#include <iostream>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

VideoCapture::VideoCapture(const bool show_video, cv::Size image_size, int camera_index)
    : show_video_(show_video), image_size(image_size), capture_(cv::VideoCapture(camera_index)) {
}

VideoCapture::VideoCapture(const bool show_video, cv::Size image_size, const std::string & camera_path)
    : show_video_(show_video), image_size(image_size), capture_(cv::VideoCapture(camera_path)) {
    std::cout << "Stream from path: " << camera_path << std::endl;
}

VideoFrame VideoCapture::getFrameFromCamera() {
    if (!capture_.isOpened()) {
        // std::cerr << "Could not get frame. Camera not available." << std::endl;
        return VideoFrame();
    }
    cv::Mat image;
    capture_ >> image;

    // Resize the image size
    image = resizeKeepAspectRatio(image, image_size, cv::Scalar(0,0,0));

    VideoFrame video_frame(image);
    if (show_video_) {
        video_frame.display();
    }
    return video_frame;
}

cv::Mat VideoCapture::resizeKeepAspectRatio(const cv::Mat &input, const cv::Size &dstSize, const cv::Scalar &bgcolor) {
    cv::Mat output;

    double h1 = dstSize.width * (input.rows/(double)input.cols);
    double w2 = dstSize.height * (input.cols/(double)input.rows);
    if( h1 <= dstSize.height) {
        cv::resize( input, output, cv::Size(dstSize.width, h1));
    } else {
        cv::resize( input, output, cv::Size(w2, dstSize.height));
    }

    int top = (dstSize.height-output.rows) / 2;
    int down = (dstSize.height-output.rows+1) / 2;
    int left = (dstSize.width - output.cols) / 2;
    int right = (dstSize.width - output.cols+1) / 2;

    cv::copyMakeBorder(output, output, top, down, left, right, cv::BORDER_CONSTANT, bgcolor );

    return output;
}
