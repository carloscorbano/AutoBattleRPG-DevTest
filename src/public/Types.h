#pragma once

#include <string>

#define MINIMAL_CLASS_ID 1
#define MAX_CLASS_ID 4

namespace Types
{
    struct GridBox
    {
        int xIndex;
        int yIndex;
        int ocupiedID;
        int Index;

        GridBox(int x, int y, int index)
        {
            xIndex = x;
            yIndex = y;
            ocupiedID = -1;
            Index = index;
        }
    };

    enum class CharacterFlag
    {
        Player,
        Enemy
    };

    enum class CharacterClass
    {
        Paladin = 1,
        Warrior = 2,
        Cleric = 3,
        Archer = 4
    };

    enum class GameState
    {
        Setup,
        CreatingCharacters,
        StartGame,
        MainGameLoop_StartTurn,
        MainGameLoop_ResolvingTurn,
        MainGameLoop_FinishedTurn,        
        GameEnded
    };

    enum class CharacterTurnState
    {
        SelectingTarget,
        Move,
        Attack
    };
}

