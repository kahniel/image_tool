//
// Created by ioann on 22.06.2025.
//

#pragma once
#include <string>
#include <unordered_map>

struct FileComparer {
    static int compare(const std::string& file1, const std::string& file2, std::string method);
    static double getMSE(const std::string& file1, const std::string& file2);
    static double getSSIM(const std::string& file1, const std::string& file2);
};