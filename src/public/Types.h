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
        UI_INTRO,
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

    enum class KeyState
    {
        NONE,
        UP,
        HOLD,
        DOWN
    };

    enum class Keys
    {
        CONSOLE_KEY_ESCAPE = 0x1B,
        CONSOLE_KEY_ENTER = 0x0D,
        CONSOLE_KEY_0 = 0x30,
        CONSOLE_KEY_1 = 0x31,
        CONSOLE_KEY_2 = 0x32,
        CONSOLE_KEY_3 = 0x33,
        CONSOLE_KEY_4 = 0x34,
        CONSOLE_KEY_5 = 0x35,
        CONSOLE_KEY_6 = 0x36,
        CONSOLE_KEY_7 = 0x37,
        CONSOLE_KEY_8 = 0x38,
        CONSOLE_KEY_9 = 0x39
    };
}

