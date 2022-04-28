#pragma once

#include "Types.h"
#include <iostream>
#include <random>

namespace Helper
{
	inline void ClearConsole()
	{
		//std::cout << "\x1B[2J\x1B[H";
        system("cls");
	}

    inline int GetRandomIntFromRange(int min, int max)
    {
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> uni(min, max);

        return uni(rng);
    }

    inline std::string GetStringFromCharacterClass(Types::CharacterClass classID)
    {
        switch (classID)
        {
        case Types::CharacterClass::Paladin:    return "Paladin";
        case Types::CharacterClass::Warrior:    return "Warrior";
        case Types::CharacterClass::Cleric:     return "Cleric";
        case Types::CharacterClass::Archer:     return "Archer";
        default:                                return "Invalid Class";
        }
    }

    inline void BaseStatusFromCharacterClass(Types::CharacterClass classID, float& outHealth, float& outDamage, int& outEnergy)
    {
        switch (classID)
        {
        case Types::CharacterClass::Paladin:
        {
            outHealth = 100;
            outDamage = 20;
            outEnergy = 1;
        }
            break;
        case Types::CharacterClass::Warrior:
        {
            outHealth = 120;
            outDamage = 20;
            outEnergy = 1;
        }
            break;
        case Types::CharacterClass::Cleric:
        {
            outHealth = 80;
            outDamage = 20;
            outEnergy = 1;
        }
            break;
        case Types::CharacterClass::Archer:
        {
            outHealth = 50;
            outDamage = 20;
            outEnergy = 1;
        }
            break;
        default:
        {
            outHealth = 100;
            outDamage = 20;
            outEnergy = 1;
        }
            break;
        }
    }

    inline int GetEnergyCostFromCharacterTurnState(Types::CharacterTurnState state)
    {
        switch (state)
        {
        case Types::CharacterTurnState::SelectingTarget:    return 0;
        case Types::CharacterTurnState::Move:               return 1;
        case Types::CharacterTurnState::Attack:             return 1;
        default:                                            return 0;
        }
    }

    inline int GetCharacterAttackRangeFromClass(Types::CharacterClass classID)
    {
        switch (classID)
        {
        case Types::CharacterClass::Paladin:    return 1;
        case Types::CharacterClass::Warrior:    return 1;
        case Types::CharacterClass::Cleric:     return 1;
        case Types::CharacterClass::Archer:     return 2;
        default:                                return 0;
        }
    }
}