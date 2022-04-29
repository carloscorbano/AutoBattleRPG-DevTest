#pragma once

#include "Helper.h"

class Grid
{

public:

    Grid(int Lines, int Columns);
    ~Grid();

    int GetIndexFromColumnAndLine(int line, int column);
    void GetColumnAndLineFromIndex(int index, int& line, int& column);
    
    //Helper Method to Get a random position.
    Types::GridBox& GetRandomAvailableGridInQuad(int xMin, int yMin, int xMax, int yMax);
    std::vector<Types::GridBox*> GetAllBoxesAroundGridBoxQuadSearch(int centreX, int centreY, int quadSize, bool includeCentreBox, int mask);
    std::vector<Types::GridBox*> GetAllBoxesAroundGridBoxCircleSearch(int centreX, int centreY, int radius, bool includeCentreBox, int mask);

    std::vector<Types::GridBox> grids;
    int xLength;
    int yLength;
};

