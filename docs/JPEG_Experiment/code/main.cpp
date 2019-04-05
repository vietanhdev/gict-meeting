#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <iostream>

using namespace cv;


void jpegResizeExperiment(const cv::Mat& input, int kJPEGQuality) {
    const std::vector<int> compression_params = {cv::IMWRITE_JPEG_QUALITY,
                                                 kJPEGQuality};
    std::vector<unsigned char> data_buffer;
    cv::imencode(".jpg", input, data_buffer, compression_params);

    std::cout << "** Quality = " << kJPEGQuality << " %%" << std::endl;

    int size = data_buffer.size();
    std::cout << "Size = " << size << " bytes ~ " << size/1000 << " kilobytes." << std::endl;

    cv::Mat write_img = cv::imdecode(data_buffer, cv::IMREAD_COLOR);

    std::stringstream filename;
    filename << input.cols << "x" << input.rows << "-" << kJPEGQuality << ".png";

    cv::imwrite(filename.str(), write_img);
}

void doExperiment(const cv::Mat& input, cv::Size img_size) {

    cv::Mat img = input.clone();

    int size = img.cols * img.rows * 3;
    std::cout << "Raw size: " << img.cols << " x " << img.rows << " x 3 = " << size << " bytes ~ " << size/1000 << " kilobytes." << std::endl;


    cv::resize(img, img, img_size);

    size = img.cols * img.rows * 3;
    std::cout << "After resized: " << img.cols << " x " << img.rows << " x 3 = " << size << " bytes ~ " << size/1000 << " kilobytes." << std::endl;


    jpegResizeExperiment(img, 80);
    jpegResizeExperiment(img, 50);
    jpegResizeExperiment(img, 25);

}

int main(int argc, char** argv )
{

    Mat image;
    image = imread( "photo.jpg", 1 );

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }

    doExperiment(image, cv::Size(640, 480));
    doExperiment(image, cv::Size(320, 240));
    

    waitKey(0);

    return 0;
}
