#include "utility.h"


namespace gict_video_conf {

// Define Microsoft functions and data types to use in other platform
#if !defined(_WIN32)
typedef int errno_t;

// https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/dupenv-s-wdupenv-s?view=vs-2017
error_t _dupenv_s(char **pValue, size_t *len, const char *pPath) {
    char *value;
    value = getenv(pPath);

    if (value != NULL) {
        *len = strlen(value);

        char *buf = (char *)malloc((*len + 1) * sizeof(char));
        // Check memory allocation
        if (buf == NULL) {  // Not enough memory
            return ENOMEM;
        }

        strncpy(buf, value, *len);
        buf[*len] = '\0';
        *pValue = buf;
    } else {
        return EINVAL;
    }

    return 0;
}
#endif  // _WIN32

void setLabel(cv::Mat &im, const std::string label, const cv::Point &origin) {
    int fontface = cv::FONT_HERSHEY_SIMPLEX;
    double scale = 0.5;
    int thickness = 1;
    int baseline = 0;

    cv::Size text =
        cv::getTextSize(label, fontface, scale, thickness, &baseline);
    cv::rectangle(im, origin + cv::Point(0, baseline),
                  origin + cv::Point(text.width, -text.height),
                  cv::Scalar(0, 255, 0), cv::FILLED);
    cv::putText(im, label, origin, fontface, scale, cv::Scalar(0, 0, 0),
                thickness, 8);
}

std::string getHomePath() {
    char *pValue;
    char *pValue2;
    size_t len;
    errno_t err = _dupenv_s(&pValue, &len, "HOME");
    errno_t err2;

    if (!err && pValue != NULL) {
        std::string pValueStdStr = pValue;
        free(pValue);
        return pValueStdStr;
    } else {
        err = _dupenv_s(&pValue, &len, "USERPROFILE");

        if (!err && pValue != NULL) {
            std::string pValueStdStr = pValue;
            free(pValue);
            return pValueStdStr;
        } else {
            err = _dupenv_s(&pValue, &len, "HOMEDRIVE");
            err2 = _dupenv_s(&pValue2, &len, "HOMEPATH");

            if (!err && !err2 && pValue != NULL && pValue2 != NULL) {
                std::string pValueStdStr = std::string(pValue) + pValue2;
                return pValueStdStr;
            }

            free(pValue);
            free(pValue2);
        }
    }

    return "";
}

}  // namespace gict_video_conf