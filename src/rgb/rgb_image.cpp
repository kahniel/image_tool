#include "rgb_image.h"
#include "rgb_pixel.h"
#include "path_utils.h"

#include <opencv2/quality.hpp>
#include <opencv2/quality/qualitybrisque.hpp>

RgbImage::RgbImage() {}
RgbImage::RgbImage(int rows, int cols): Mat(rows, cols, CV_8UC3) {}
RgbImage::RgbImage(const Mat& other): Mat(other) {}

short RgbImage::getR(int i, int j) const { return at<cv::Vec3b>(i, j)[2]; }
short RgbImage::getG(int i, int j) const { return at<cv::Vec3b>(i, j)[1]; }
short RgbImage::getB(int i, int j) const { return at<cv::Vec3b>(i, j)[0]; }


void RgbImage::setR(const int i, const int j, const short v) {
    at<cv::Vec3b>(i, j)[2] = v;
}
void RgbImage::setG(const int i, const int j, const short v) {
    at<cv::Vec3b>(i, j)[1] = v;
}
void RgbImage::setB(const int i, const int j, const short v) {
    at<cv::Vec3b>(i, j)[0] = v;
}

RgbPixel RgbImage::getPixel(int i, int j) const {
    return RgbPixel(getR(i, j), getG(i, j), getB(i, j));
}

void RgbImage::write(const std::string &outputFileName) const {
    if (!cv::imwrite(outputFileName, *this)) {
        throw std::runtime_error("Failed to write PNG file: " + outputFileName);
    }
}

RgbImage* RgbImage::read(const std::string &inputFileName) {
    RgbImage* image = new RgbImage(cv::imread(inputFileName, cv::IMREAD_COLOR));

    if (image->empty()) {
        throw std::runtime_error("Cannot open PNG file: " + inputFileName);
    }
    return image;
}

double RgbImage::getMSE(const RgbImage* image1, const RgbImage* image2) {
    if (image1->size() != image2->size()) {
        throw std::invalid_argument("Images must have the same dimensions for MSE calculation.");
    }

    Mat diff;
    cv::absdiff(*image1, *image2, diff);

    diff.convertTo(diff, CV_32F);

    diff = diff.mul(diff);

    cv::Scalar sumSq = cv::mean(diff);

    return (sumSq[0] + sumSq[1] + sumSq[2]) / 3.0;
}

double RgbImage::getSSIM(const RgbImage* image1, const RgbImage* image2) {
    if (image1->size() != image2->size()) {
        throw std::invalid_argument("Images must have the same dimensions for SSIM calculation.");
    }

    Mat ssim_map;
    cv::Scalar ssim = cv::quality::QualitySSIM::compute(*image1, *image2, ssim_map);
    return (ssim[0] + ssim[1] + ssim[2]) / 3.0;
}

double RgbImage::getBRISQUE(const RgbImage* image) {
    std::filesystem::path model_file_path = getExecRootDir() / "models" / "brisque_model_live.yml";
    std::filesystem::path range_file_path = getExecRootDir() / "models" / "brisque_range_live.yml";
    cv::Ptr<cv::quality::QualityBRISQUE> br =
        cv::quality::QualityBRISQUE::create(model_file_path.string(), range_file_path.string());
    cv::Scalar score = br->compute(*image);
    return score[0];
}
