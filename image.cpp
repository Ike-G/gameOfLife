#include "image.h"
#include <fstream>
#include <iostream>

Colour::Colour() : r(0), g(0), b(0) {}
Colour::Colour(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
Colour::~Colour() {}
Colour Colour::operator*(double x) const {
    return Colour(static_cast<uint8_t>((double)r*x), static_cast<uint8_t>((double)b*x), static_cast<uint8_t>((double)b*x));
}

Image::Image(uint16_t width, uint16_t height) 
    : m_width(width), m_height(height), m_colours(std::vector<Colour>(width*height, Colour(255, 255, 255))) {}
Image::~Image() {}

Colour Image::getColour(uint16_t x, uint16_t y) {
    return m_colours[x+m_width*y];
}

void Image::setColour(const Colour& colour, uint16_t x, uint16_t y) {
    m_colours[x+m_width*y].r = colour.r;
    m_colours[x+m_width*y].g = colour.g;
    m_colours[x+m_width*y].b = colour.b;
}

// void Image::drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
//     const Colour white = Colour(255, 255, 255);
//     uint16_t t, dx, dy, xpxl1, ypxl1, xpxl2, ypxl2;
//     double m, xend, yend, xgap, intery;
//     bool steep = abs(y2-y1) > abs(x2-x1);
//     if (steep) {
//         t = x1;
//         x1 = y1;
//         y1 = t;
//         t = x2;
//         x2 = y2;
//         y2 = t;
//     }
//     if (x1 > x2) {
//         t = x1;
//         x1 = x2;
//         x2 = t;
//         t = y1;
//         y1 = y2;
//         y2 = t;
//     }
//     
//     dy = y2-y1;
//     dx = x2-x1;
//     m = (double)dy/(double)dx;
//     if (dx == 0)
//         m = 1.;
//     
//     if (steep) {
//         setColour(white*0.5, y1, x1);
//         setColour(Colour(0, 0, 0), y1+1, x1);
//     } else {
//         setColour(white*0.5, x1, y1);
//         setColour(Colour(0, 0, 0), x1, y1+1);
//     }
//     intery = y1+m;
// 
//     if (steep) {
//         setColour(white*0.5, y2, x2);
//         setColour(Colour(0, 0, 0), y2+1, x2);
//     } else {
//         setColour(white*0.5, xpxl2, ypxl2);
//         setColour(Colour(0, 0, 0), x2, y2+1);
//     }
// 
//     uint16_t fintery;
//     double fpintery;
//     if (steep) {
//         for (uint16_t x = x1+1; x <= x2-1; ++x) {
//             fintery = (uint16_t)floor(fintery); 
//             fpintery = intery-floor(intery);
//             setColour(white*(1-fpintery), fintery, x);
//             setColour(white*fpintery, fintery+1, x);
//             intery += m;
//         }
//     } else {
//         for (uint16_t x = x1+1; x <= x2-1; ++x) {
//             fintery = (uint16_t)floor(intery);
//             fpintery = intery-floor(intery);
//             setColour(white*(1-fpintery), x, fintery);
//             setColour(white*fpintery, x, fintery+1);
//             intery += m;
//         }
//     }
// }

void Image::Export(const char* path) {
    std::ofstream f;
    f.open(path, std::ios::out | std::ios::binary);

    if (!f.is_open()) {
        std::cout << "File failed to open." << std::endl;
        return;
    }

    unsigned char bmpPad[3] = { 0, 0, 0 };
    const uint8_t paddingAmount = (4-(m_width*3)%4)%4;

    const uint8_t fileHeaderSize = 14;
    const uint8_t informationHeaderSize = 40;
    const uint16_t fileSize = fileHeaderSize + informationHeaderSize 
                              + m_height * m_width * 3 + paddingAmount * m_height;

    // File header
    unsigned char fileHeader[fileHeaderSize];

    // File type
    fileHeader[0] = 'B';
    fileHeader[1] = 'M';
    // Header size
    fileHeader[2] = fileHeaderSize;
    fileHeader[3] = 0;
    fileHeader[4] = 0;
    fileHeader[5] = 0;
    // Reserved
    fileHeader[6] = 0;
    fileHeader[7] = 0;
    fileHeader[8] = 0;
    fileHeader[9] = 0;
    // Offset
    fileHeader[10] = fileHeaderSize+informationHeaderSize;
    fileHeader[11] = 0;
    fileHeader[12] = 0;
    fileHeader[13] = 0;

    // BITMAPINFOHEADER
    unsigned char informationHeader[informationHeaderSize];

    // Information header size
    informationHeader[0] = informationHeaderSize;
    informationHeader[1] = 0;
    informationHeader[2] = 0;
    informationHeader[3] = 0;
    // Width
    informationHeader[4] = m_width;
    informationHeader[5] = m_width >> 8;
    informationHeader[6] = m_width >> 16;
    informationHeader[7] = m_width >> 24;
    // Height
    informationHeader[8] = m_height;
    informationHeader[9] = m_height >> 8;
    informationHeader[10] = m_height >> 16;
    informationHeader[11] = m_height >> 24;
    // Planes
    informationHeader[12] = 1;
    informationHeader[13] = 0;
    // BPP
    informationHeader[14] = 24;
    informationHeader[15] = 0;
    // Compression
    informationHeader[16] = 0;
    informationHeader[17] = 0;
    informationHeader[18] = 0;
    informationHeader[19] = 0;
    // Size (set to 0 for BI_RGB bitmaps) 
    informationHeader[20] = 0;
    informationHeader[21] = 0;
    informationHeader[22] = 0;
    informationHeader[23] = 0;
    // Pixels per metre (X)
    informationHeader[24] = 0;
    informationHeader[25] = 0;
    informationHeader[26] = 0;
    informationHeader[27] = 0;
    // Pixels per metre (Y)
    informationHeader[28] = 0;
    informationHeader[29] = 0;
    informationHeader[30] = 0;
    informationHeader[31] = 0;
    // Colours used (Colour palette unused)
    informationHeader[32] = 0;
    informationHeader[33] = 0;
    informationHeader[34] = 0;
    informationHeader[35] = 0;
    // Required colour indexes (0 indicates all colours are necessary)
    informationHeader[36] = 0;
    informationHeader[37] = 0;
    informationHeader[38] = 0;
    informationHeader[39] = 0;

    f.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
    f.write(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) { 
            uint8_t r = getColour(x, y).r;
            uint8_t g = getColour(x, y).g;
            uint8_t b = getColour(x, y).b;
            uint8_t colour[3] = {b, g, r};
            f.write(reinterpret_cast<char*>(colour), 3);
        }
        f.write(reinterpret_cast<char*>(bmpPad), paddingAmount);
    }

    f.close();
    std::cout << "File created." << std::endl;
}

