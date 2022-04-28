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
        bool ocupied;
        int Index;

        GridBox(int x, int y, bool isOcupied, int index)
        {
            xIndex = x;
            yIndex = y;
            ocupied = isOcupied;
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
        Checking,
        SelectingTarget,
        CheckRange,
        Move,
        Attack
    };
}

