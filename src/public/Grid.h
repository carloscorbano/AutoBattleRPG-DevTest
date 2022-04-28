#pragma once

#include "Helper.h"
#include <vector>

class Grid
{

public:

    Grid(int Lines, int Columns);
    ~Grid();

    std::vector<Types::GridBox> grids;

    int xLength;
    int yLength;

    int GetIndexFromColumnAndLine(int line, int column);
    void GetColumnAndLineFromIndex(int index, int& line, int& column);
    
    //Helper Method to Get a random position.
    int GetRandomAvailableIndexInQuad(int xMin, int yMin, int xMax, int yMax);
};

