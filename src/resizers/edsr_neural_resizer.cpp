//
// Created by ioann on 22.06.2025.
//

#include "file_resizer.h"
#include "path_utils.h"
#include <opencv2/dnn_superres.hpp>
#include <filesystem>

using namespace cv;
using namespace dnn;
using namespace dnn_superres;

RgbImage *EDSRNeuralResizer::upSize(const RgbImage *lowResImage, int scale) {
    cv::setNumThreads(1);

    std::string modelFilename = "EDSR_x" + std::to_string(scale) + ".pb";
    std::filesystem::path modelPath = getModelsDir() / modelFilename;

    DnnSuperResImpl sr;
    sr.readModel(modelPath.string());
    sr.setModel("edsr", scale);

    sr.setPreferableBackend(DNN_BACKEND_OPENCV);
    sr.setPreferableTarget(DNN_TARGET_CPU);

    RgbImage *highResImage = new RgbImage();
    sr.upsample(*lowResImage, *highResImage);

    return highResImage;
}