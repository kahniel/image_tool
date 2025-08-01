//
// Created by ioann on 23.06.2025.
//

#include <fstream>
#include "file_comparer.h"
#include "file_quality_evaluator.h"
#include "rgb_image.h"
#include "results.h"
#include <filesystem>
#include <CLI/CLI.hpp>

#include "path_utils.h"

namespace fs = std::filesystem;

void readTime(std::map<ResultKey, double>& timeResults, const fs::path& resultsDirPath) {
    std::ifstream time_file(resultsDirPath / "results_time.csv");
    std::string line;
    std::getline(time_file, line);

    while (std::getline(time_file, line)) {
        std::stringstream ss(line);
        std::string image, scale_str, method, elapsed_str;

        std::getline(ss, image, ',');
        std::getline(ss, scale_str, ',');
        std::getline(ss, method, ',');
        std::getline(ss, elapsed_str, ',');

        int scale = std::stoi(scale_str);
        double elapsed = std::stod(elapsed_str);

        timeResults[{image, scale, method}] = elapsed;
    }
}

int main(const int argc, char *argv[]) {
    CLI::App app{"Quality evaluation"};

    fs::path origPath, upDirPath, resultsDirPath;
    bool dir_flag{false};
    app.add_flag("--dir", dir_flag, "Directory mode");
    app.add_option(
        "-o,--orig,--original", origPath,
        "Original file/directory"
        )->required();
    app.add_option(
        "-u,--up,--upsized", upDirPath,
        "Upsized images directory"
        )->required();
    app.add_option(
        "-r,--res,--results", resultsDirPath,
        "Measured data directory"
        )->required();

    CLI11_PARSE(app, argc, argv);

    if (checkPath(origPath, "orig", dir_flag)) {
        return 1;
    }
    if (checkPath(upDirPath, "upsized", true)) {
        return 1;
    }
    if (checkPath(resultsDirPath, "results", true)) {
        return 1;
    }

    std::map<ResultKey, double> timeResults;
    try {
        readTime(timeResults, resultsDirPath);
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error while reading time results: " << e.what() << '\n';
    }

    std::vector<std::string> methods = {
        "my-up", "linear", "cubic", "lanczos", "edsr", "realesrgan"
    };

    std::vector<fs::path> images;
    if (dir_flag) {
        images = ImageFileIterator::getImageFiles(origPath);
    } else {
        images = {origPath};
    }

    std::map<ResultKey, ResultData> results;

    std::cout << std::left << std::setw(20) << "image"
                          << std::setw(15) << "scale"
                          << std::setw(15) << "method"
                          << std::setw(15) << "MSE"
                          << std::setw(15) << "SSIM"
                          << std::setw(15) << "BRISQUE"
                          << std::setw(15) << "Time (ms)" << std::endl;
    for (const auto& origImagePath : images) {
        std::string imageName = origImagePath.stem().string();

        for (int scale = 2; scale <= 4; ++scale) {
            for (const auto& method : methods) {
                fs::path upImagePath = upDirPath / imageName / std::to_string(scale) / (method + ".png");
                if (!fs::exists(upImagePath)) {
                    std::cerr << "Upsized image not found for path: " << upImagePath << std::endl;
                    continue;
                }

                double mse = FileComparer::getMSE(upImagePath, origImagePath);
                double ssim = FileComparer::getSSIM(upImagePath, origImagePath);
                double brisque = FileQualityEvaluator::getBRISQUE(upImagePath);
                double elapsed = 0.0;
                auto it = timeResults.find({imageName, scale, method});
                if (it != timeResults.end()) {
                    elapsed = it->second;
                }

                std::cout << std::left << std::setw(20) << imageName
                          << std::setw(15) << scale
                          << std::setw(15) << method
                          << std::setw(15) << mse
                          << std::setw(15) << ssim
                          << std::setw(15) << brisque
                          << std::setw(15) << elapsed << std::endl;

                results[ResultKey{imageName, scale, method}] = ResultData{mse, ssim, brisque, elapsed};
            }
        }
    }

    std::ofstream csvResults(resultsDirPath / "results.csv");
    csvResults << "image;scale;method;MSE;SSIM;BRISQUE;Time (ms)\n";
    for (const auto&[resultKey, resultData] : results) {
        csvResults << resultKey.imageName << "," << resultKey.scale << "," << resultKey.method << ","
            << resultData.mse << "," << resultData.ssim << "," << resultData.brisque << ","
            << resultData.elapsed << std::endl;
    }

    csvResults.close();
    return 0;
}
