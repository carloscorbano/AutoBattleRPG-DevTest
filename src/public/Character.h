#pragma once
#include "Grid.h"

#include <memory>
#include <bitset>

#define FLAG_SIZE 1
#define FLAG_PLAYER 0
#define FLAG_ENEMY 1

class Character
{
public:

    Character(Types::CharacterClass characterClass);
    ~Character();

    void SetData(float health, float baseDamage, int index);
    void StartTurn(Grid* battlefield);
    bool TakeDamage(float amount);

    //TODO: ANALYSE THIS METHOD AND IT'S DEFINITION.
    int GetIndex(std::vector<Types::GridBox*> v, int index);
    bool IsDead() const;

private:
    void WalkTo(bool CanWalk);

    bool CheckCloseTargets(Grid* battlefield);

    void Attack(Character* target);

    void SetTarget(Character* target);

    void Die();

private:
    float health;
    float baseDamage;
    float damageMultiplier;
    //Types::GridBox currentBox;
    int playerIndex;
    std::bitset<FLAG_SIZE> flag;

    Character* target;

    bool isDead;
    char icon;
};

