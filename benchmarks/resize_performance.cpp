//
// Created by ioann on 21.06.2025.
//

#include "file_resizer.h"
#include "rgb_image.h"
#include "path_utils.h"
#include "results.h"
#include <filesystem>
#include <chrono>
#include <fstream>
#include <CLI/CLI.hpp>

namespace fs = std::filesystem;

int main(const int argc, char *argv[]) {
    CLI::App app{"Resize Performance"};


    fs::path downsizedDirPath, upDirPath, resultsDirPath;
    app.add_option(
        "-d,--down,--downsized", downsizedDirPath,
        "Downsized images directory"
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

    if (checkPath(downsizedDirPath, "downsized", true)) {
        return 1;
    }

    std::map<ResultKey, double> timeResults;

    std::vector<std::string> methods = {
        "my-up", "linear", "cubic", "lanczos", "edsr", "realesrgan"
    };


    for (const auto& entry : fs::directory_iterator(downsizedDirPath)) {
        if (!entry.is_directory()) continue;
        auto imageName = entry.path().filename().string();

        for (int scale = 2; scale <= 4; ++scale) {
            auto downImagePath = entry.path() / (std::to_string(scale) + ".png");
            if (!fs::exists(downImagePath)) {
                std::cerr << "Downsized image not found for path: " << downImagePath << std::endl;
                continue;
            }

            for (const auto& method : methods) {
                fs::path upImagePath = upDirPath / imageName / std::to_string(scale) / (method + ".png");
                fs::create_directories(upImagePath.parent_path());

                std::cout << "Processing image:\t" << imageName
                                << "\n\ton scale: \t" << scale
                                << "\n\twith method: \t" << method << std::endl;

                auto start = std::chrono::high_resolution_clock::now();
                FileResizer::resize(downImagePath, upImagePath, scale, method);
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> elapsed = end - start;

                timeResults[ResultKey{imageName, scale, method}] = elapsed.count();
            }
        }
    }


    if (!fs::exists(resultsDirPath)) {
        std::cout << "!!!";
        fs::create_directories(resultsDirPath);
    }
    if (!fs::is_directory(resultsDirPath)) {
        std::cerr << "Incorrect results path: Given path is not a directory" << std::endl;
        return 1;
    }

    std::ofstream csvTimeResults(resultsDirPath / "results_time.csv");
    csvTimeResults << "image,scale,method,elapsed_ms\n";
    for (const auto&[resultKey, elapsed_ms] : timeResults) {
        csvTimeResults << resultKey.imageName << "," << resultKey.scale << "," << resultKey.method << "," << elapsed_ms << "\n";
    }

    csvTimeResults.close();
    return 0;
}
