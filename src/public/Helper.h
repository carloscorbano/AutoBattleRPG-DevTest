#pragma once

#include "Types.h"
#include <iostream>
#include <random>
#include <chrono>
#include <thread>

namespace Helper
{
	/// <summary>
	/// Clear the console window.
	/// </summary>
	inline void ClearConsole()
	{
		//std::cout << "\x1B[2J\x1B[H";
        system("cls");
	}

    /// <summary>
    /// Returns a random integer from given range.
    /// </summary>
    inline int GetRandomIntFromRange(int min, int max)
    {
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> uni(min, max);

        return uni(rng);
    }

    /// <summary>
    /// Makes the thread wait for the amount of milliseconds.
    /// </summary>
    inline void WaitForMilliseconds(int milliseconds)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }

    /// <summary>
    /// Get a random name value.
    /// </summary>
    inline std::string GetRandomName()
    {
        //all the names here was random generated from the website https://www.name-generator.org.uk/fantasy/
        std::vector<std::string> NAMES = {  "Frank Henry", "Samantha Gutierrez", "Grover Robinson", "Jamie Mason", "Annie Stephens", "April Murray",
                                            "Charley Henderson", "Tommy Medina", "Nannie Ortiz", "Joshua Robertson", "Rebecca Lewis", "Kevin Lawrence",
                                            "Maxwell Nelson", "Tia Reed" };

        int index = GetRandomIntFromRange(0, static_cast<int>(NAMES.size() - 1));

        return NAMES[index];
    }

    /// <summary>
    /// Returns the string representation of the character class enum
    /// </summary>
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

    /// <summary>
    /// Returns the base status of the given character class.
    /// </summary>
    inline void BaseStatusFromCharacterClass(Types::CharacterClass classID, float& outHealth, float& outDamage, int& outEnergy)
    {
        switch (classID)
        {
        case Types::CharacterClass::Paladin:
        {
            outHealth = 100;
            outDamage = 20;
            outEnergy = 2;
        }
            break;
        case Types::CharacterClass::Warrior:
        {
            outHealth = 120;
            outDamage = 20;
            outEnergy = 2;
        }
            break;
        case Types::CharacterClass::Cleric:
        {
            outHealth = 80;
            outDamage = 20;
            outEnergy = 2;
        }
            break;
        case Types::CharacterClass::Archer:
        {
            outHealth = 50;
            outDamage = 20;
            outEnergy = 2;
        }
            break;
        default:
        {
            outHealth = 100;
            outDamage = 20;
            outEnergy = 2;
        }
            break;
        }
    }

    /// <summary>
    /// Get the energy cost of each turn state value.
    /// </summary>
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

    /// <summary>
    /// Get the value of the attack range from the given class.
    /// </summary>
    inline int GetCharacterAttackRangeFromClass(Types::CharacterClass classID)
    {
        switch (classID)
        {
        case Types::CharacterClass::Paladin:    return 1;
        case Types::CharacterClass::Warrior:    return 1;
        case Types::CharacterClass::Cleric:     return 1;
        case Types::CharacterClass::Archer:     return 3;
        default:                                return 0;
        }
    }

    inline int CalculateManhattanDist(int fromX, int fromY, int toX, int toY)
    {
        int deltaX = toX - fromX;
        int deltaY = toY - fromY;

        //Manhattan distance
        return (abs(deltaX) + abs(deltaY));
    }
}