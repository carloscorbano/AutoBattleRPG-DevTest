#pragma once

#include "Defs.h"
#include <string>
#include <vector>
#include <bitset>

namespace Types
{
    struct GridBox
    {
        int xIndex;
        int yIndex;
        int occupiedID;
        int Index;

        GridBox(int x, int y, int index)
        {
            xIndex = x;
            yIndex = y;
            occupiedID = EMPTY_GRID;
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
        DecidingAction,
        SelectingTarget,
        Move,
        Attack
    };

    struct CharacterInfo
    {
        Types::CharacterClass characterClass;
        std::string name;
        std::bitset<FLAG_SIZE> flag;
        float health;
        float baseDamage;
        float damageMultiplier;
        int energy;
        int playerIndex;
        bool isDead;
        char icon;
    };
}

