#include "rgb_pixel.h"

RgbPixel::RgbPixel(const short r, const short g, const short b) : r(r), g(g), b(b) {}
RgbPixel::RgbPixel(const RgbPixel &other): r(other.getR()), g(other.getG()), b(other.getB()) {}
RgbPixel::~RgbPixel() = default;

short RgbPixel::getR() const { return r; }
short RgbPixel::getG() const { return g; }
short RgbPixel::getB() const { return b; }