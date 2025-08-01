//
// Created by ioann on 22.06.2025.
//

#pragma once
#include <filesystem>
#include <vector>
#include <string>
#include <set>

namespace fs = std::filesystem;

fs::path getExecRootDir();
fs::path getModelsDir();
fs::path getExternalDir();


class ImageFileIterator {
    static const std::set<std::string> supportedExtensions;

    static std::string toLower(const std::string& str);
    static bool isImageExtension(const std::string& extension);
public:
    static std::vector<fs::path> getImageFiles(const fs::path& directory);
};

int checkPath(const fs::path& path, const std::string& name, bool isDir);