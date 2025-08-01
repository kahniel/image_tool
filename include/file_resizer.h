//
// Created by ioann on 22.06.2025.
//

#pragma once
#include "rgb_image.h"
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

struct FileResizer {
    static int resize(const fs::path& inputFile, const fs::path& outputFile, int scale, const std::string &method);
};

struct MyResizer {
    static RgbImage* upSize(const RgbImage *lowResImage, int scale);
    static RgbImage* downSize(const RgbImage *image, int scale);
};

struct OpenCVResizer {
    static RgbImage* upSize(const RgbImage *lowResImage, int scale, cv::InterpolationFlags flag);
    static RgbImage* upSizeLinear(const RgbImage *lowResImage, int scale);
    static RgbImage* upSizeCubic(const RgbImage *lowResImage, int scale);
    static RgbImage* upSizeLanczos(const RgbImage *lowResImage, int scale);
};

struct EDSRNeuralResizer {
    static RgbImage* upSize(const RgbImage *lowResImage, int scale);
};

struct RealESRGANNeuralResizer {
    static RgbImage* upSize(const RgbImage *lowResImage, int scale);
};