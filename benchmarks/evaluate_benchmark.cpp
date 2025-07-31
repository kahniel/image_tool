//
// Created by ioann on 23.06.2025.
//

#include <fstream>
#include "file_comparer.h"
#include "file_quality_evaluator.h"
#include "rgb_image.h"
#include <filesystem>

#include "path_utils.h"

namespace fs = std::filesystem;

using Key = std::tuple<std::string, int, std::string>;

struct KeyHash {
    std::size_t operator()(const Key& k) const {
        return std::hash<std::string>{}(std::get<0>(k))
             ^ std::hash<int>{}(std::get<1>(k))
             ^ std::hash<std::string>{}(std::get<2>(k));
    }
};

void readTime(std::unordered_map<Key, double, KeyHash>& time_map) {
    std::ifstream time_file("results_time.csv");
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

        time_map[{image, scale, method}] = elapsed;
    }
}

int main(const int argc, char *argv[]) {
    std::unordered_map<Key, double, KeyHash> time_map;
    readTime(time_map);

    std::ofstream outCSV(getExecRootDir() / "results" / "results.csv");
    outCSV << "image;scale;method;MSE;SSIM;BRISQUE;Time(ms)\n";

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
            std::cout << "\nResults for image: " << image << " on scale: " << scale << std::endl;
            std::cout << std::left << std::setw(20) << "Method"
                      << std::setw(15) << "MSE"
                      << std::setw(15) << "SSIM"
                      << std::setw(15) << "BRISQUE"
                      << std::setw(15) << "Time(ms)" << std::endl;

            const std::string origFileName = DataOpener::getImagePath(image);

            for (const auto& method : methods) {
                std::string resFileName = OutOpener::getImagePath(image, method, scale);

                double mse = FileComparer::getMSE(resFileName, origFileName);
                double ssim = FileComparer::getSSIM(resFileName, origFileName);
                double brisque = FileQualityEvaluator::getBRISQUE(resFileName);

                double elapsed = 0.0;
                auto it = time_map.find({image, scale, method});
                if (it != time_map.end()) {
                    elapsed = it->second;
                }

                std::cout << std::left << std::setw(20) << method
                          << std::setw(15) << mse
                          << std::setw(15) << ssim
                          << std::setw(15) << brisque
                          << std::setw(15) << elapsed << std::endl;

                outCSV << image << ";" << scale << ";" << method << ";"
                       << mse << ";" << ssim << ";" << brisque << ";" << elapsed << "\n";
            }
        }
    }
    // outCSV.close();
    return 0;
}
