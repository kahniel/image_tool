//
// Created by ioann on 21.06.2025.
//

#include "file_resizer.h"
#include "rgb_image.h"
#include "path_utils.h"
#include <filesystem>

namespace fs = std::filesystem;

int main(const int argc, char *argv[]) {
    std::vector<std::string> images = {"kyle", "david", "ponyo", "cat"};
    if (argc > 1) {
        images.resize(argc - 1);
        for (int i = 1; i < argc; ++i) {
            images[i - 1] = argv[i];
        }
    }

    for (const auto& image : images) {
        for (int scale = 2; scale <= 4; ++scale) {
            const fs::path inputFilePath = DataOpener::getImagePath(image);

            fs::create_directories(DataOpener::getDir(scale));

            fs::path outputFilePath = DataOpener::getImagePath(image, scale);

            FileResizer::resize(inputFilePath, outputFilePath, scale, "my-down");
        }
    }
    return 0;
}
