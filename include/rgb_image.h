//
// Created by ioann on 22.06.2025.
//

#pragma once
#include "rgb_pixel.h"
#include <string>
#include <opencv2/opencv.hpp>

struct RgbImage: cv::Mat {
    RgbImage();
    RgbImage(int rows, int cols);
    explicit RgbImage(const Mat& other);

    [[nodiscard]] short getR(int i, int j) const;
    [[nodiscard]] short getG(int i, int j) const;
    [[nodiscard]] short getB(int i, int j) const;

    void setR(int i, int j, short v);
    void setG(int i, int j, short v);
    void setB(int i, int j, short v);

    [[nodiscard]] RgbPixel getPixel(int i, int j) const;

    void write(const std::string &outputFileName) const;
    static RgbImage *read(const std::string &inputFileName);
    static double getMSE(const RgbImage *image1, const RgbImage *image2);
    static double getSSIM(const RgbImage* image1, const RgbImage* image2);
    static double getBRISQUE(const RgbImage* image);
};
