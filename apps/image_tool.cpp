//
// Created by ioann on 31.07.2025.
//
#include "file_resizer.h"
#include "file_comparer.h"
#include "file_quality_evaluator.h"
#include <CLI/CLI.hpp>
#include <set>

int main(int argc, const char** argv) {
    CLI::App app{"Image Tool"};

    CLI::App* resize = app.add_subcommand(
        "resize", "Resize the image"
        );
    CLI::App* compare = app.add_subcommand(
        "compare", "Compare images")
    ;
    CLI::App* quality = app.add_subcommand(
        "quality", "Evaluate image quality"
        );


    std::string inputFile, outputFile;
    int scale = 1;
    std::string resizeMethod = "my-up";
    std::set<std::string> resizeMethods = {
        "my-up", "my-down", "linear", "cubic",
        "lanczos", "edsr", "realesrgan"
    };
    resize->add_option("-i,--input", inputFile, "Input image path")->required();
    resize->add_option("-o,--output", outputFile, "Output image path")->required();
    resize->add_option("-s,--scale", scale, "Scale factor")->check(CLI::PositiveNumber);
    resize->add_option("-m,--method", resizeMethod, "Resize method")
          ->check(CLI::IsMember(resizeMethods))
          ->default_str("my-up");

    std::string image1, image2;
    std::string compareMethod = "mse";
    std::set<std::string> compareMethods = {
        "mse", "ssim"
    };
    compare->add_option("-1,--first", image1, "First image path")->required();
    compare->add_option("-2,--second", image2, "Second image path")->required();
    compare->add_option(
        "-m,--method", compareMethod, "Comparison method"
        )->check(CLI::IsMember(compareMethods))->default_str("mse");

    std::string qualityInput;
    quality->add_option("-i,--input", qualityInput, "Input image path")->required();

    CLI11_PARSE(app, argc, argv);
    if (app.got_subcommand("resize")) {
        return FileResizer::resize(inputFile, outputFile, scale, resizeMethod);
    }
    if (app.got_subcommand("compare")) {
        return FileComparer::compare(image1, image2, compareMethod);
    }
    if (app.got_subcommand("quality")) {
        return FileQualityEvaluator::evaluate(qualityInput);
    }
}