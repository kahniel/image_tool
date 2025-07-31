#include "file_resizer.h"
#include <cmath>

RgbImage *MyResizer::upSize(const RgbImage *lowResImage, int scale) {
    cv::Mat final;
    cv::resize(*lowResImage, final, cv::Size(), scale, scale, cv::INTER_NEAREST);

    return new RgbImage(final);
}

RgbImage *MyResizer::downSize(const RgbImage *image, int scale) {
    cv::Mat input_float;
    image->convertTo(input_float, CV_32FC3, 1.0 / 255.0);

    cv::Mat linear;
    cv::pow(input_float, 2.2, linear);

    cv::Mat linear_small;
    cv::resize(linear, linear_small, cv::Size(image->cols / scale, image->rows / scale), 0, 0, cv::INTER_AREA);

    cv::Mat corrected;
    cv::pow(linear_small, 1.0 / 2.2, corrected);

    cv::Mat final;
    corrected.convertTo(final, CV_8UC3, 255.0);

    return new RgbImage(final);
}