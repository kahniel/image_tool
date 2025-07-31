//
// Created by ioann on 22.06.2025.
//

#pragma once
#include <filesystem>

std::filesystem::path getExecRootDir();
std::filesystem::path getModelsDir();
std::filesystem::path getExternalDir();

struct DataOpener {
    static std::filesystem::path getDir(int scale);
    static std::filesystem::path getDir();
    static std::filesystem::path getImagePath(const std::string& imageName, int scale);
    static std::filesystem::path getImagePath(const std::string& imageName);
};

struct OutOpener {
    static std::filesystem::path getDir(const std::string& imageName, int scale);
    static std::filesystem::path getImagePath(const std::string& imageName, const std::string& method, int scale);
};

std::filesystem::path getOutDir();