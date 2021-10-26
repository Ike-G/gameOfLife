#include "image.h"
#include <array>
#include <bitset>
#include <cstdint>
#define SIZE 80
#define ITERATIONS 100
#define PPS 10

void generateAutomata(std::bitset<SIZE> s, uint8_t rule) {
    Image im(SIZE*PPS, ITERATIONS*PPS);
    Colour set[2] = { Colour(255, 255, 255), Colour(0, 0, 0) };
    std::bitset<SIZE> t;
    for (int y = 0; y < ITERATIONS; y++) {
        for (int x = 0; x < SIZE; x++) {
            for (int i = x*PPS; i < (x+1)*PPS; i++) {
                for (int j = y*PPS; j < (y+1)*PPS; j++) {
                    im.setColour(set[s[x]], i, j);
                }
            }
        }
        t[0] = rule & 1 << (s[SIZE-1] << 2 | s[0] << 1 | s[1]);
        t[SIZE-1] = rule & 1 << (s[SIZE-2] << 2 | s[SIZE-1] << 1 | s[0]);
        for (int i = 1; i < SIZE-1; i++) t[i] = rule & 1 << (s[i-1] << 2 | s[i] << 1 | s[i+1]);
        for (int i = 0; i < SIZE; i++) s[i] = t[i];
    }
    im.Export(("elem\\"+std::to_string(rule)+".bmp").c_str());
}

int main() { 
    uint8_t r = 0;
    std::bitset<SIZE> s;
    do {
        for (int i = 0; i < SIZE; i++) s[i] = rand()&1;
        generateAutomata(s, r);
    } while (++r != 0);
    return 0;
}