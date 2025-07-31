//
// Created by ioann on 23.06.2025.
//

#pragma once
#include <string>
#include <unordered_map>

struct FileQualityEvaluator {
    static double getBRISQUE(const std::string& file);
    static int evaluate(const std::string& file);
};
