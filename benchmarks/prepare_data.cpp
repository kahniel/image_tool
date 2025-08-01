//
// Created by ioann on 21.06.2025.
//

#include "file_resizer.h"
#include "path_utils.h"
#include <CLI/CLI.hpp>
#include <filesystem>

namespace fs = std::filesystem;

void generateDownsizes(const fs::path& origImagePath, const fs::path& outputDirPath) {
    for (int scale = 2; scale <= 4; ++scale) {
        try {
            fs::create_directories(outputDirPath / origImagePath.stem());
            fs::path outputFilePath = outputDirPath / origImagePath.stem() / (std::to_string(scale) + ".png");

            FileResizer::resize(origImagePath, outputFilePath, scale, "my-down");
        } catch (const fs::filesystem_error& ex) {
            std::cerr << "Filesystem error: " << ex.what() << std::endl;
        }
    }
}

int main(const int argc, char *argv[]) {
    CLI::App app{"Prepare Test Data"};

    bool dir_flag{false};
    app.add_flag("--dir", dir_flag, "Directory mode");

    fs::path origPath, downDirPath;
    app.add_option(
        "-o,--orig,--original", origPath,
        "Original image[s] file/directory"
        )->required();
    app.add_option(
        "--down,--downsized", downDirPath,
        "Downsized images directory"
        )->required();

    CLI11_PARSE(app, argc, argv);

    if (checkPath(origPath, "orig", dir_flag)) {
        return 1;
    }

    if (dir_flag) {
        const auto images = ImageFileIterator::getImageFiles(origPath);
        for (const auto& origImagePath : images) {
            generateDownsizes(origImagePath, downDirPath);
        }
    } else {
        generateDownsizes(origPath, downDirPath);
    }

    return 0;
}
