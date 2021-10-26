#include "image.h"
#include <iostream>
#include <fileapi.h>
#include <errhandlingapi.h>
#include <winerror.h>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <math.h>
#include <cstdint>
#include <bitset>

#define SIZE 64
#define PPS 7

std::vector<std::bitset<SIZE*SIZE>> generateStates(uint16_t rule, std::bitset<SIZE*SIZE> s, int iterations) {
    std::vector<std::bitset<SIZE*SIZE>> states(iterations);
    std::bitset<SIZE*SIZE> sp;
    uint8_t n;
    for (int i = 0; i < iterations; i++) {
        for (int p = 0; p < SIZE*SIZE; ++p) {
            n = -s[p];
            for (int yd = -SIZE; yd <= SIZE; yd+=SIZE) {
                for (int xd = -1; xd <= 1; ++xd) {
                    if (s[(SIZE*SIZE+p+yd+xd)%(SIZE*SIZE)])
                        ++n;
                }
            }
            sp[p] = rule & 1 << (s[p] << 3 | (n & 7));
        }
        s = sp;
        states[i] = sp;
    }
    return states;
}

void processRule(uint16_t r, int iterations = 100) { 
    std::cout << r << std::endl;
    std::stringstream stream;
    stream << std::hex << std::uppercase << r; 
    if (!(CreateDirectoryA(("lifeVars\\"+stream.str()).c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError()))
        return;

    std::bitset<SIZE*SIZE> s;
    std::vector<std::bitset<SIZE*SIZE>> states;
    for (int i = 0; i < SIZE*SIZE; i++) {
        s[i] = rand()&1;
    }

    states = generateStates(r, s, iterations);
    Colour c[2] = {Colour(0, 0, 0), Colour(255, 255, 255)};
    div_t d;
    for (int i = 0; i < iterations; i++) {
        Image im(SIZE*PPS, SIZE*PPS);
        for (int j = 0; j < SIZE*SIZE; j++) {
            d = div(j,SIZE);
            for (int x = d.rem*PPS; x < (d.rem+1)*PPS; ++x) {
                for (int y = d.quot*PPS; y < (d.quot+1)*PPS; ++y) {
                    im.setColour(c[states[i][j]], x, y);
                }
            }
        }
        im.Export(("lifeVars\\"+stream.str()+"\\"+std::to_string(i)+".bmp").c_str());
    }

    return;
}

int main() {
    std::string s;
    uint16_t r;
    do {
        r = rand()&255 | (rand()&255) << 8;
        processRule(r);
        std::cout << "Quit? ";
        std::getline(std::cin, s);
    } while (s != "q");
    return 0;
}