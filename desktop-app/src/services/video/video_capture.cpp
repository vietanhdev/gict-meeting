#include "video/video_capture.h"

#include <iostream>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

VideoCapture::VideoCapture(const bool show_video, const float scale)
    : show_video_(show_video), scale_(scale), capture_(cv::VideoCapture(-1)) {}

VideoFrame VideoCapture::GetFrameFromCamera() {
    if (!capture_.isOpened()) {
        std::cerr << "Could not get frame. Camera not available." << std::endl;
        return VideoFrame();
    }
    cv::Mat image;
    capture_ >> image;
    // If the image is being downsampled, resize it first.
    if (scale_ < 1.0) {
        cv::resize(image, image, cv::Size(0, 0), scale_, scale_);
    }
    VideoFrame video_frame(image);
    if (show_video_) {
        video_frame.Display();
    }
    return video_frame;
}
