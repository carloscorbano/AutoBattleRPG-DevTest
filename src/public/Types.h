#pragma once

#include <string>

class Types
{
public:

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

    enum class CharacterClass
    {
        Paladin = 1,
        Warrior = 2,
        Cleric = 3,
        Archer = 4
    };

    inline static std::string GetStringFromCharacterClass(CharacterClass classID)
    {
        switch (classID)
        {
        case Types::CharacterClass::Paladin: return "Paladin";
        case Types::CharacterClass::Warrior: return "Warrior";
        case Types::CharacterClass::Cleric: return "Cleric";
        case Types::CharacterClass::Archer: return "Archer";
        default: return "Invalid Class";
        }
    }

    enum class GameState
    {
        Setup,
        CreatingCharacters,
        MainGameLoop_SelectingCharacter,
        MainGameLoop_ResolvingCharacterTurn,
        MainGameLoop_FinishedTurn,
        MainGameLoop_CheckEndGame,
        Cleanup,
        GameEnded
    };

    enum class CharacterTurnState
    {
        SelectingTarget,
        CheckRange,
        Move,
        Attack
    };
};

