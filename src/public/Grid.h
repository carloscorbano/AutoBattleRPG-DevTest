#pragma once

#include "Helper.h"
#include <vector>

class Grid
{

public:

    Grid(int Lines, int Columns);
    ~Grid();

    std::vector<Types::GridBox> grids;

    int xLenght;
    int yLength;
    
    // prints the matrix that indicates the tiles of the battlefield
    void drawBattlefield(int Lines, int Columns);

    //Helper Method to Get a random position.
    Types::GridBox& GetRandomAvailableGridBoxInQuad(int xMin, int yMin, int xMax, int yMax);
};

