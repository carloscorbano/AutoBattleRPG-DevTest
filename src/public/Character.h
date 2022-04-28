#pragma once
#include "Grid.h"

#include <memory>
#include <bitset>

#define FLAG_SIZE 1
#define FLAG_PLAYER 0
#define FLAG_ENEMY 1

#define PLAYER_CHARACTER_ICON 'P'
#define ENEMY_CHARACTER_ICON 'E'

class Character
{
public:

    Character(Types::CharacterClass characterClass, float health, float baseDamage, int index, char icon, int energy, Types::GridBox spawnLocation, Types::CharacterFlag flag);
    ~Character();

    void StartTurn(Grid* battlefield);
    bool TakeDamage(float amount);

    //TODO: ANALYSE THIS METHOD AND IT'S DEFINITION.
    int GetIndex(std::vector<Types::GridBox*> v, int index);
    bool IsDead() const;

    std::bitset<FLAG_SIZE> GetFlag() const;

private:
    void WalkTo(bool CanWalk);

    bool CheckCloseTargets(Grid* battlefield);

    void Attack(Character* target);

    void SetTarget(Character* target);

    void Die();

private:
    Types::CharacterClass characterClass;
    float health;
    float baseDamage;
    float damageMultiplier;
    int energy;
    Types::GridBox currentBox;
    int playerIndex;
    std::bitset<FLAG_SIZE> flag;

    bool isDead;
    char icon;
};

