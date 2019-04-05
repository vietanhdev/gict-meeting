// The VideoCapture class uses OpenCV to capture video frames from a webcam or
// connected video camera and returns the raw frame data (after compressing the
// frame image in JPEG format). This data can be sent over the network.

#ifndef VIDEO_VIDEO_CAPTURE_H
#define VIDEO_VIDEO_CAPTURE_H

#include <string>

#include "video/video_frame.h"

#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"

class VideoCapture {
   public:
    // Initializes the OpenCV VideoCapture object by selecting the default
    // camera. If no camera is provided it will not be possile to read any video
    // frames.
    //
    // Specify whether or not the video being sent is displayed in a window, and
    // the scale = (0, 1] which will affect the size of the data.
    VideoCapture(const bool show_video, cv::Size image_size);

    cv::Mat resizeKeepAspectRatio(const cv::Mat &input, const cv::Size &dstSize, const cv::Scalar &bgcolor);

    // Captures and returns a frame from the available video camera.
    //
    // If the show_video option was set to true, the frame will be displayed.
    //
    // NOTE: This method cannot be const, since the cv::VideoCapture object is
    // modified through a non-const method when getting a new frame from the
    // camera.
    VideoFrame getFrameFromCamera();

   private:
    // The OpenCV camera capture object. This is used to interface with a
    // connected camera and extract frames from it.
    cv::VideoCapture capture_;

    // Set to true to show the video.
    const bool show_video_;

    cv::Size image_size;
};

#endif  // VIDEO_VIDEO_CAPTURE_H
