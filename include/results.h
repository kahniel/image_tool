//
// Created by ioann on 01.08.2025.
//

#pragma once
#include <string>

struct ResultKey {
    std::string imageName;
    int scale;
    std::string method;

    bool operator<(const ResultKey& other) const {
        return std::tie(imageName, scale, method) < std::tie(other.imageName, other.scale, other.method);
    }
};

struct ResultData {
    double mse;
    double ssim;
    double brisque;
    double elapsed;
};