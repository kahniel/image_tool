//
// Created by ioann on 23.06.2025.
//

#include "file_quality_evaluator.h"
#include "rgb_image.h"
#include <iostream>

double FileQualityEvaluator::getBRISQUE(const std::string& file) {
    const RgbImage* image = RgbImage::read(file);

    double brisqueScore = RgbImage::getBRISQUE(image);
    delete image;

    return brisqueScore;
}

int FileQualityEvaluator::evaluate(const std::string& file) {
    try {
        std::cout << "BRISQUE score for " << file <<  ":\n\t" << getBRISQUE(file) << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}