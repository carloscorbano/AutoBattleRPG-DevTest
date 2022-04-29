#pragma once
#include "Grid.h"

#include <memory>
#include <bitset>

#define FLAG_SIZE 1
#define FLAG_PLAYER 0
#define FLAG_ENEMY 1

#define PLAYER_CHARACTER_ICON 'P'
#define ENEMY_CHARACTER_ICON 'E'

class BattleField;

class Character
{
public:

    Character(std::string name, Types::CharacterClass characterClass, float health, float baseDamage, int index, char icon, int energy,
        Types::GridBox* spawnLocation, Types::CharacterFlag flag);
    ~Character();

    void StartTurn(BattleField* battlefield, Grid* grid, std::vector<std::shared_ptr<Character>> allCharacters);
    void TakeDamage(float amount);

    int GetIndex() const;
    void SetPlayerIndex(int newIndex);
    char GetIcon() const;
    bool IsDead() const;

    std::bitset<FLAG_SIZE> GetFlag() const;

private:
    bool CheckTargetIsWithinAttackRange(Grid* grid);
    void SelectTarget(std::vector<std::shared_ptr<Character>> allCharacters);
    void WalkTo(Grid* grid);
    void Attack();
    void Die();
    float CalculateDistance(const Types::GridBox& a, const Types::GridBox& b);

private:
    Types::CharacterClass characterClass;
    std::string name;
    float health;
    float baseDamage;
    float damageMultiplier;
    int energy;
    int playerIndex;
    std::bitset<FLAG_SIZE> flag;
    Types::GridBox* currentBox;
    Character* target;
    Types::CharacterTurnState state;
    bool isDead;
    char icon;
};

