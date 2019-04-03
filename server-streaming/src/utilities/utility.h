#if !defined(UTILITY_H)
#define UTILITY_H

#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <iostream>

namespace gict_video_conf {

    void setLabel(cv::Mat& im, const std::string label, const cv::Point & origin);
    std::string getHomePath();

}

#endif // UTILITY_H
