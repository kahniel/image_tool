//
// Created by ioann on 21.06.2025.
//

#include "file_resizer.h"
#include "rgb_image.h"
#include "path_utils.h"
#include <filesystem>
#include <chrono>
#include <fstream>

namespace fs = std::filesystem;

int main(const int argc, char *argv[]) {
    struct Result {
        std::string image;
        int scale;
        std::string method;
        double elapsed_ms;
    };
    std::vector<Result> results;

    std::vector<std::string> images = {"kyle", "david", "ponyo", "cat"};
    if (argc > 1) {
        images.resize(argc - 1);
        for (int i = 1; i < argc; ++i) {
            images[i - 1] = argv[i];
        }
    }
    std::vector<std::string> methods = {
        "my-up", "linear", "cubic", "lanczos", "edsr", "realesrgan"
    };

    for (const auto& image : images) {
        for (int scale = 2; scale <= 4; ++scale) {

            const fs::path inputFilePath = DataOpener::getImagePath(image, scale);

            for (const auto& method : methods) {
                fs::create_directories(OutOpener::getDir(image, scale));
                std::string outputFilePath = OutOpener::getImagePath(image, method, scale);

                std::cout << "Processing image:\t" << image
                                << "\n\ton scale: \t" << scale
                                << "\n\twith method: \t" << method << std::endl;

                auto start = std::chrono::high_resolution_clock::now();
                FileResizer::resize(inputFilePath, outputFilePath, scale, method);
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> elapsed = end - start;

                results.push_back(Result{image, scale, method, elapsed.count()});
            }
        }
    }

    std::ofstream csv_file("results_time.csv");
    csv_file << "image,scale,method,elapsed_ms\n";
    for (const auto& res : results) {
        csv_file << res.image << "," << res.scale << "," << res.method << "," << res.elapsed_ms << "\n";
    }

    return 0;
}
