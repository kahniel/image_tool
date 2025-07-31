//
// Created by ioann on 22.06.2025.
//

#include "path_utils.h"
#include <filesystem>
#include <stdexcept>
#include <cstdlib>
#include <mach-o/dyld.h>

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

fs::path DataOpener::getDir(const int scale) {
    return getDir() / std::to_string(scale);
}
fs::path DataOpener::getDir() {
    return getExecRootDir() / "data";
}
fs::path DataOpener::getImagePath(const std::string& imageName, const int scale) {
    return getDir(scale) / (imageName + ".png");
}
fs::path DataOpener::getImagePath(const std::string& imageName) {
    return getDir() / (imageName + ".png");
}

fs::path OutOpener::getDir(const std::string& imageName, int scale) {
    return getExecRootDir() / "out" / std::to_string(scale) / imageName;
}
fs::path OutOpener::getImagePath(const std::string& imageName, const std::string& method, int scale) {
    return getDir(imageName, scale) / (method + ".png");
}

fs::path getOutDir() {
    return getExecRootDir() / "out";
}
