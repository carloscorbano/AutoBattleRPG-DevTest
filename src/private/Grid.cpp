#include "../public/Grid.h" 

Grid::Grid(int Lines, int Columns)
{
    xLength = Lines;
    yLength = Columns;

    //Little otimization, reserving the size instead of always resizing it.
    grids.reserve(static_cast<size_t>(xLength * yLength));

    for (int y = 0, index = 0; y < Columns; ++y)
    {
        for (int x = 0; x < Lines; ++x, ++index)
        {
            Types::GridBox newBox(x, y, index);
            grids.push_back(newBox);
        }
    }
}

Grid::~Grid() 
{}

int Grid::GetIndexFromColumnAndLine(int line, int column)
{
    return xLength * column + line;
}

void Grid::GetColumnAndLineFromIndex(int index, int& line, int& column)
{
    line = index % xLength;
    column = (index - line) / xLength;
}

Types::GridBox& Grid::GetRandomAvailableGridInQuad(int xMin, int yMin, int xMax, int yMax)
{
    if (xMin < 0) xMin = 0;
    if (yMin < 0) yMin = 0;
    if (xMax >= xLength) xMax = xLength - 1;
    if (yMax >= yLength) yMax = yLength - 1;

    int index, x, y;
    while (true)
    {
        x = Helper::GetRandomIntFromRange(xMin, xMax);
        y = Helper::GetRandomIntFromRange(yMin, yMax);
        index = GetIndexFromColumnAndLine(x, y);

        if (grids[index].occupiedID == EMPTY_GRID)
        {
            break;
        }
    }

    return grids[index];
}

std::vector<Types::GridBox*> Grid::GetAllBoxesAroundGridBoxQuadSearch(int centreX, int centreY, int quadSize, bool includeCentreBox, int mask)
{
    if (quadSize <= 0 || centreX < 0 || centreX >= xLength || centreY < 0 || centreY >= yLength) return std::vector<Types::GridBox*>();

    std::vector<Types::GridBox*> boxes;
        
    for (int y = (centreY - quadSize); y <= (centreY + quadSize); ++y)
    {
        for (int x = (centreX - quadSize); x <= (centreX + quadSize); ++x)
        {
            int index = GetIndexFromColumnAndLine(x, y);
            //Check if is outside of grid or it's itself
            if (x < 0 || y < 0 || x >= (xLength) || (y >= yLength) || (!includeCentreBox && x == centreX && y == centreY) || index < 0 || index >= grids.size() ||
                (((mask & SEARCH_MASK_ONLY_OCCUPIED_BOXES) == SEARCH_MASK_ONLY_OCCUPIED_BOXES) && grids[index].occupiedID == EMPTY_GRID) ||
                (((mask & SEARCH_MASK_ONLY_UNOCCUPIED_BOXES) == SEARCH_MASK_ONLY_UNOCCUPIED_BOXES) && grids[index].occupiedID != EMPTY_GRID)
                ) continue;

            boxes.push_back(&grids[index]);
        }
    }


    return boxes;
}

std::vector<Types::GridBox*> Grid::GetAllBoxesAroundGridBoxCircleSearch(int centreX, int centreY, int radius, bool includeCentreBox, int mask)
{
    if (radius <= 0 || centreX < 0 || centreX >= xLength || centreY < 0 || centreY >= yLength) return std::vector<Types::GridBox*>();

    std::vector<Types::GridBox*> boxes;
    
    for (int y = (centreY - radius); y <= (centreY + radius); ++y)
    {
        for (int x = (centreX - radius); x <= (centreX + radius); ++x)
        {
            int index = GetIndexFromColumnAndLine(x, y);

            //Check if is outside of grid or it's itself
            if (x < 0 || y < 0 || x >= xLength || y >= yLength || (!includeCentreBox && x == centreX && y == centreY) || index < 0 || index >= grids.size() ||
                (((mask & SEARCH_MASK_ONLY_OCCUPIED_BOXES) == SEARCH_MASK_ONLY_OCCUPIED_BOXES) && grids[index].occupiedID == EMPTY_GRID) ||
                (((mask & SEARCH_MASK_ONLY_UNOCCUPIED_BOXES) == SEARCH_MASK_ONLY_UNOCCUPIED_BOXES) && grids[index].occupiedID != EMPTY_GRID) ||
                (Helper::CalculateManhattanDist(x, y, centreX, centreY) > radius)
                ) continue;

            boxes.push_back(&grids[index]);
        }
    }

    return boxes;
}
