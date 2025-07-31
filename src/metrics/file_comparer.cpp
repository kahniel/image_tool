#include "file_comparer.h"
#include "rgb_image.h"
#include <iostream>

void getArgs(std::string &file1, std::string &file2, std::string &method,
        const std::unordered_map<std::string, std::string>& args
        ) {
    try {
        file1 = args.at("--file-1");
        file2 = args.at("--file-2");
        method = args.count("--method") ? args.at("--method") : "mse";
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Error while parsing arguments: ") + e.what());
    }
}

double FileComparer::getMSE(const std::string& file1, const std::string& file2) {
    const RgbImage *image1 = RgbImage::read(file1);
    const RgbImage *image2 = RgbImage::read(file2);
    return RgbImage::getMSE(image1, image2);
}

double FileComparer::getSSIM(const std::string& file1, const std::string& file2) {
    const RgbImage *image1 = RgbImage::read(file1);
    const RgbImage *image2 = RgbImage::read(file2);
    return RgbImage::getSSIM(image1, image2);
}

int FileComparer::compare(const std::string& file1, const std::string& file2, std::string method) {
    if (method != "mse" && method != "ssim") {
        std::cerr << "Unsupported comparison method: " << method << ", using MSE\n";
        method = "mse";
    }

    try {
        if (method == "mse") {
            std::cout << "Mean Squared Error (MSE) between\n\t"
                      << file1 << " & " << file2 << ":\n\t"
                      << getMSE(file1, file2)
                      << std::endl;
        } else if (method == "ssim") {
            std::cout << "Structural Similarity Index (SSIM) between\n\t"
                      << file1 << " & " << file2 << ":\n\t"
                      << getSSIM(file1, file2)
                      << std::endl;
        }
    } catch(const std::runtime_error& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}
