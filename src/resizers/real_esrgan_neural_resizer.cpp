//
// Created by ioann on 22.06.2025.
//
#include "file_resizer.h"
#include "path_utils.h"
#include <filesystem>
#include <stdexcept>
#include <cstdlib>
#include <iomanip>
#include <mach-o/dyld.h>

RgbImage* RealESRGANNeuralResizer::upSize(const RgbImage *lowResImage, int scale) {
    if (scale <= 1 || scale > 4) {
        throw std::invalid_argument("Unsupported scale: " + std::to_string(scale));
    }

    std::string model = "realesr-animevideov3";
    if (scale == 4) {
        model = "realesrgan-x4plus";
    }

    auto tmpDir = std::filesystem::temp_directory_path();
    auto uid = std::to_string(std::rand());
    auto tempInput = tmpDir / ("input_" + uid + ".png");
    auto tempOutput = tmpDir / ("output_" + uid + ".png");

    lowResImage->write(tempInput.string());

    std::filesystem::path realEsrganBin = getExternalDir() / "realesrgan-ncnn-vulkan";

    std::filesystem::path prevPath = std::filesystem::current_path();
    std::filesystem::current_path(getExecRootDir());

    std::ostringstream cmd;

    cmd << std::quoted(realEsrganBin.string())
    << " -i " << std::quoted(tempInput.string())
    << " -o " << std::quoted(tempOutput.string())
    << " -s " << scale
    << " -n " << model;


    int ret = std::system(cmd.str().c_str());
    std::filesystem::current_path(prevPath);
    if (ret != 0) {
        throw std::runtime_error("realesrgan-ncnn-vulkan failed");
    }

    RgbImage *restored = RgbImage::read(tempOutput.string());

    std::filesystem::remove(tempInput);
    std::filesystem::remove(tempOutput);

    return restored;
}
