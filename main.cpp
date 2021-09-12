#include "image.h"
#include <iostream>
#include <string>

int main() {
    const int height = 100;
    const int width = 100;
    const int pps = 10;
    const int buffer = 20;

    int t = 1;
    int counter = 0;
    std::string s;
    bool newCells[width+2*buffer][height+2*buffer];
    bool oldCells[width+2*buffer][height+2*buffer];
    for (int x = 0; x < width+2*buffer; x++) {
        for (int y = 0; y < width+2*buffer; y++) {
            if (rand() % 10 == 1)
                oldCells[x][y] = true;
            else
                oldCells[x][y] = false;
        }
    }
    while (t > 0) {
        while (t > 0) {
            Image step(width*pps, height*pps);
            for (int x = 1; x < width+2*buffer-1; x++) {
                for (int y = 1; y < height+2*buffer-1; y++) {
                    int neighbours = 0;
                    for (int i = -1; i <= 1; i++) {
                        for (int j = -1; j <= 1; j++) {
                            if (oldCells[x+i][y+j])
                                neighbours++;
                        }
                    }
                    if ((oldCells[x][y] && (neighbours == 3 || neighbours == 4)) || (!oldCells[x][y] && neighbours == 3))
                        newCells[x][y] = true;
                    else
                        newCells[x][y] = false;
                }
            }
            for (int x = buffer; x < width+buffer; x++) {
                for (int y = buffer; y < height+buffer; y++) {
                    if (newCells[x][y]) {
                        for (int i = (x-buffer)*pps; i < (x-buffer+1)*pps; i++) {
                            for (int j = (y-buffer)*pps; j < (y-buffer+1)*pps; j++) {
                                step.setColour(Colour(0, 0, 0), i, j);
                            }
                        }
                    }
                }
            }
            std::copy(&newCells[0][0], &newCells[0][0]+(width+2*buffer)*(height+2*buffer), &oldCells[0][0]);
            step.Export(("images\\"+std::to_string(counter)+".bmp").c_str());
            counter++;
            t--;
        }    
        std::cout << "Steps: ";
        std::cin >> s;
        t = stoi(s);
    }
    return 0;
}