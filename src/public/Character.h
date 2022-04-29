#pragma once
#include "Grid.h"

class BattleField;

class Character
{
public:

    Character(Types::CharacterInfo& charInfo);
    ~Character();

    void StartTurn(BattleField* battlefield, Grid* grid, std::vector<std::shared_ptr<Character>> allCharacters);
    void TakeDamage(float amount);

    Types::CharacterInfo& GetCharacterInfo();
    void SetGridBox(Types::GridBox* box);
private:

    void DecideAction(int& movements_left, int& attacks_left, int& curEnergy, Grid& grid);
    bool CheckTargetIsWithinAttackRange(Grid* grid);
    void SelectTarget(std::vector<std::shared_ptr<Character>> allCharacters);
    void WalkTo(Grid* grid);
    void Attack();
    void Die();

private:

    Types::GridBox* currentBox;
    Character* target;
    Types::CharacterTurnState state;    
    Types::CharacterInfo info;
};

