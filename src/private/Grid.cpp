#include "../public/Grid.h" 

Grid::Grid(int Lines, int Columns)
{
    xLength = Lines;
    yLength = Columns;

    for (int y = 0, index = 0; y < Columns; y++)
    {
        for (int x = 0; x < Lines; x++, index++)
        {
            Types::GridBox newBox(x, y, index);
            grids.push_back(newBox);
            //Console.Write($"{newBox.Index}\n");
        }
    }
	//drawBattlefield(Lines, Columns);
}

Grid::~Grid() 
{

}

int Grid::GetIndexFromColumnAndLine(int line, int column)
{
    return xLength * column + line;
}

void Grid::GetColumnAndLineFromIndex(int index, int& line, int& column)
{
    line = index % xLength;
    column = (index - line) / xLength;
}

//void Grid::DrawBattlefield()
//{
//    //for (int i = 0; i < Lines; i++)
//    //{
//    //    for (int j = 0; j < Columns; j++)
//    //    {
//    //        Types::GridBox* currentgrid = new Types::GridBox();
//    //        if (currentgrid->ocupied)
//    //        {
//    //            //if()
//    //            printf("[X]\t");
//    //        }
//    //        else
//    //        {
//    //            printf("[ ]\t");
//    //        }
//    //    }
//    //    printf("\n");
//    //}
//    //printf("\n");
//}

int Grid::GetRandomAvailableIndexInQuad(int xMin, int yMin, int xMax, int yMax)
{
    int index, x, y;
    while (true)
    {
        x = Helper::GetRandomIntFromRange(xMin, xMax);
        y = Helper::GetRandomIntFromRange(yMin, yMax);
        index = GetIndexFromColumnAndLine(x, y);

        if (grids[index].ocupiedID == -1)
        {
            break;
        }
    }

    return index;
}
