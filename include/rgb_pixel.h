//
// Created by ioann on 22.06.2025.
//

#pragma once

struct RgbPixel {
private:
    short r, g, b;

public:
    RgbPixel(short r, short g, short b);
    RgbPixel(const RgbPixel &other);

    ~RgbPixel();

    [[nodiscard]] short getR() const;
    [[nodiscard]] short getG() const;
    [[nodiscard]] short getB() const;
};