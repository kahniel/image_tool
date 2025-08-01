//
// Created by ioann on 22.06.2025.
//

#include "path_utils.h"
#include <filesystem>
#include <stdexcept>
#include <mach-o/dyld.h>
#include <iostream>
#include <algorithm>
#include <opencv2/opencv.hpp>

#define PATH_MAX 1024

namespace fs = std::filesystem;

fs::path getExecRootDir() {
    char result[PATH_MAX];
    uint32_t size = sizeof(result);
    if (_NSGetExecutablePath(result, &size) != 0)
        throw std::runtime_error("Buffer too small for executable path");
    return std::filesystem::path(result).parent_path().parent_path().parent_path();
}
fs::path getModelsDir() {
    return getExecRootDir() / "models";
}
fs::path getExternalDir() {
    return getExecRootDir() / "external";
}

std::string ImageFileIterator::toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

bool ImageFileIterator::isImageExtension(const std::string& extension) {
    std::string ext = toLower(extension);

    if (!ext.empty() && ext[0] == '.') {
        ext = ext.substr(1);
    }
    return supportedExtensions.find(ext) != supportedExtensions.end();
}

std::vector<fs::path> ImageFileIterator::getImageFiles(const fs::path& directory) {
    std::vector<fs::path> imageFiles;

    if (!fs::exists(directory) || !fs::is_directory(directory)) {
        std::cerr << "Directory does not exist: " << directory << std::endl;
        return imageFiles;
    }

    try {
        for (const auto& entry : fs::directory_iterator(directory)) {
            if (entry.is_regular_file()) {
                const auto& path = entry.path();
                if (isImageExtension(path.extension().string())) {
                    imageFiles.push_back(path);
                }
            }
        }
    } catch (const fs::filesystem_error& ex) {
        std::cerr << "Filesystem error: " << ex.what() << std::endl;
    }

    std::sort(imageFiles.begin(), imageFiles.end());
    return imageFiles;
}

const std::set<std::string> ImageFileIterator::supportedExtensions = {
    "bmp", "dib",
    "jpeg", "jpg", "jpe", "jp2",
    "png",
    "webp",
    "pbm", "pgm", "ppm", "pxm", "pnm",
    "sr", "ras",
    "exr",
    "hdr", "pic"
};

int checkPath(const fs::path& path, const std::string& name, const bool isDir) {
    if (!fs::exists(path)) {
        std::cerr << "Incorrect " << name << " path: Given path does not exist" << std::endl;
        return 1;
    }
    if (isDir && !fs::is_directory(path)) {
        std::cerr << "Incorrect " << name << " path: Given path is not a directory" << std::endl;
        return 1;
    }
    return 0;
}
