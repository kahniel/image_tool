#include "file_resizer.h"
#include "rgb_image.h"
#include "path_utils.h"
#include <iostream>
#include <string>

RgbImage* resizeImage(const RgbImage *rgbImage, int scale, const std::string& method) {
    if (scale == 1) {
        return new RgbImage(*rgbImage);
    }
    RgbImage *upsized = nullptr;

    if (method == "my-up") {
        upsized = MyResizer::upSize(rgbImage, scale);
    } else if (method == "my-down") {
        upsized = MyResizer::downSize(rgbImage, scale);
    } else if (method == "linear") {
        upsized = OpenCVResizer::upSizeLinear(rgbImage, scale);
    } else if (method == "cubic") {
        upsized = OpenCVResizer::upSizeCubic(rgbImage, scale);
    } else if (method == "lanczos") {
        upsized = OpenCVResizer::upSizeLanczos(rgbImage, scale);
    } else if (method == "edsr") {
        upsized = EDSRNeuralResizer::upSize(rgbImage, scale);
    } else if (method == "realesrgan") {
        upsized = RealESRGANNeuralResizer::upSize(rgbImage, scale);
    } else {
        std::cerr << "Unsupported resize method: " << method << ", using MyResizer::upSize\n";
        upsized = MyResizer::upSize(rgbImage, scale);
    }

    return upsized;
}

int FileResizer::resize(const std::string& inputFile, const std::string& outputFile, int scale, const std::string &method) {
    if ((method == "edsr" || method == "realesrgan") && scale > 4) {
        std::cerr << "Unsupported scale for neural resizers: " << scale << std::endl;
        return 1;
    }

    try {
        const RgbImage *image = RgbImage::read(inputFile);

        image = resizeImage(image, scale, method);
        std::cout << getExecRootDir() << " " << outputFile << std::endl;
        image->write(outputFile);

        delete image;
    } catch(const std::runtime_error& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}
