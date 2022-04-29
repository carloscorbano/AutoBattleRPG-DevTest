#include "../public/Character.h"
#include "../public/BattleField.h"

#include <cmath>
#include <limits>
#include <assert.h>

Character::Character(std::string name, Types::CharacterClass characterClass, float health, float baseDamage, int index, char icon, int energy,
    Types::GridBox* spawnLocation, Types::CharacterFlag flag)
    :   name(name), characterClass(characterClass), health(health), baseDamage(baseDamage), energy(energy), currentBox(spawnLocation),
        playerIndex(index), isDead(false), icon(icon), target(nullptr), state(Types::CharacterTurnState::SelectingTarget)
{
    this->flag.set(0, static_cast<int>(flag));
    damageMultiplier = static_cast<float>(Helper::GetRandomIntFromRange(1, 100)) * 0.01f;
}

Character::~Character()
{}

void Character::StartTurn(BattleField* battlefield, Grid* grid, std::vector<std::shared_ptr<Character>> allCharacters)
{
    //if this character is dead, then just return.
    if (isDead) return;

    //Set the initial state of the machine.
    state = Types::CharacterTurnState::SelectingTarget;

    int curEnergy = energy;
    //Character AI state machine.

    while (curEnergy > 0)
    {
        switch (state)
        {
        case Types::CharacterTurnState::SelectingTarget:
        {
            SelectTarget(allCharacters);
            if (CheckTargetIsWithinAttackRange(grid))
            {
                state = Types::CharacterTurnState::Attack;
            }
            else
            {
                state = Types::CharacterTurnState::Move;
            }
        }
            break;
        case Types::CharacterTurnState::Move:
        {
            WalkTo(grid);
            battlefield->DrawBattleField();
            curEnergy -= Helper::GetEnergyCostFromCharacterTurnState(state);
            state = Types::CharacterTurnState::SelectingTarget;
        }
            break;
        case Types::CharacterTurnState::Attack:
        {
            Attack();
            curEnergy -= Helper::GetEnergyCostFromCharacterTurnState(state);
            state = Types::CharacterTurnState::SelectingTarget;
        }
            break;
        default:
            break;
        }
    }
}

void Character::TakeDamage(float amount) 
{
    health -= baseDamage;

	if (health <= 0)
	{
		Die();
	}
}

int Character::GetIndex() const
{
    return playerIndex;
}

void Character::SetPlayerIndex(int newIndex)
{
    playerIndex = newIndex;
    currentBox->ocupiedID = newIndex;
}

char Character::GetIcon() const
{
    return icon;
}

bool Character::IsDead() const
{
    return isDead;
}

std::bitset<FLAG_SIZE> Character::GetFlag() const
{
    return flag;
}

bool Character::CheckTargetIsWithinAttackRange(Grid* grid)
{
    assert(target != nullptr);

    //Check if is within the attack range (8 directions)
    int attackRange = Helper::GetCharacterAttackRangeFromClass(characterClass);
    
    int curX = currentBox->xIndex;
    int curY = currentBox->yIndex;

    int targetX = target->currentBox->xIndex;
    int targetY = target->currentBox->yIndex;

    return (targetX >= (curX - attackRange) && targetX <= (curX + attackRange) &&
            targetY >= (curY - attackRange) && targetY <= (curY + attackRange));
}

void Character::SelectTarget(std::vector<std::shared_ptr<Character>> allCharacters)
{
    float closestDistance = static_cast<float>(INT_MAX);
    Character* closestCharacter = nullptr;

    //Check the nearest target
    for (auto it = allCharacters.begin(); it != allCharacters.end(); ++it)
    {
        //Don't check itself, and don't aim for the same type and don't aim for dead targets
        if (it->get() == this || ((*it)->GetFlag() == GetFlag()) || (*it)->IsDead()) continue;

        //Calculate distance
        float mDistance = CalculateDistance(*currentBox, *(*it)->currentBox);

        if (closestDistance > mDistance)
        {
            closestDistance = mDistance;
            closestCharacter = it->get();
        }
    }

    target = closestCharacter;
}

void Character::WalkTo(Grid* grid)
{
    assert(target != nullptr);

    //find the fastest way to get to the target
    int curX = currentBox->xIndex;
    int curY = currentBox->yIndex;

    float nearestDistance = static_cast<float>(INT_MAX);
    int nextIndex = -1;

    //Iterate with neightbours gridboxes.
    for (int y = (curY - 1); y <= (curY + 1); ++y)
    {
        for (int x = (curX - 1); x <= (curX + 1); ++x)
        {
            //check if it is within the grid length and don't get the current grid.
            if (x < 0 || y < 0 || x >= (grid->xLength) || y >= (grid->yLength) || (x == curX && y == curY)) continue;

            //Get the cur box data.
            int index = grid->GetIndexFromColumnAndLine(x, y);

            //If the neightbour grid box is already beeing ocupied by another character.
            if (grid->grids[index].ocupiedID != -1) continue;

            float neightbourDistanceToTarget = CalculateDistance(grid->grids[index], *(target->currentBox));

            if (nearestDistance > neightbourDistanceToTarget)
            {
                nearestDistance = neightbourDistanceToTarget;
                nextIndex = index;
            }
        }
    }

    //Move
    currentBox->ocupiedID = -1;
    grid->grids[nextIndex].ocupiedID = playerIndex;
    currentBox = &grid->grids[nextIndex];
}

void Character::Attack() 
{
    assert(target != nullptr);

    target->TakeDamage(baseDamage + (baseDamage * damageMultiplier));
}

void Character::Die()
{
    //Set dead state, the system will handle this over.
    isDead = true;
}

float Character::CalculateDistance(const Types::GridBox& a, const Types::GridBox& b)
{
    //Calculate distance
    int otherX = b.xIndex;
    int otherY = b.yIndex;

    float deltaX = static_cast<float>(otherX) - static_cast<float>(a.xIndex);
    float deltaY = static_cast<float>(otherY) - static_cast<float>(a.yIndex);

    //Manhattan distance
    return (fabs(deltaX) + fabs(deltaY));
}


