//
// Created by ioann on 21.06.2025.
//

#include "rgb_image.h"
#include "file_resizer.h"
#include <opencv2/opencv.hpp>

RgbImage* OpenCVResizer::upSize(const RgbImage* lowResImage, int scale, cv::InterpolationFlags flag) {
    cv::Mat final;
    cv::resize(*lowResImage, final, cv::Size(), scale, scale, flag);

    return new RgbImage(final);
}

RgbImage* OpenCVResizer::upSizeLinear(const RgbImage* lowResImage, int scale) {
    return OpenCVResizer::upSize(lowResImage, scale, cv::INTER_LINEAR);
}

RgbImage* OpenCVResizer::upSizeCubic(const RgbImage* lowResImage, int scale) {
    return OpenCVResizer::upSize(lowResImage, scale, cv::INTER_CUBIC);
}

RgbImage* OpenCVResizer::upSizeLanczos(const RgbImage* lowResImage, int scale) {
    return OpenCVResizer::upSize(lowResImage, scale, cv::INTER_LANCZOS4);
}
