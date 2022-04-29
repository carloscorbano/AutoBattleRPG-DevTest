#include "../public/Character.h"
#include "../public/BattleField.h"

#include <cmath>
#include <limits>

Character::Character(Types::CharacterInfo& charInfo)
    :   info(charInfo), currentBox(nullptr), target(nullptr), state(Types::CharacterTurnState::SelectingTarget)
{}

Character::~Character()
{}

void Character::StartTurn(BattleField* battlefield, Grid* grid, std::vector<std::shared_ptr<Character>> allCharacters)
{
    //if this character is dead, then just return.
    if (info.isDead) return;

    //Set the initial state of the machine.
    state = Types::CharacterTurnState::SelectingTarget;

    int curEnergy = info.energy;
    int moveEnergyCost = Helper::GetEnergyCostFromCharacterTurnState(Types::CharacterTurnState::Move);
    int attackEnergyCost = Helper::GetEnergyCostFromCharacterTurnState(Types::CharacterTurnState::Attack);
    //Character AI state machine.

    while (curEnergy > 0)
    {
        switch (state)
        {
        case Types::CharacterTurnState::SelectingTarget:
        {
            if (target == nullptr || target->info.isDead)
            {
                SelectTarget(allCharacters);
            }

            if (CheckTargetIsWithinAttackRange(grid) && curEnergy - attackEnergyCost >= 0)
            {
                state = Types::CharacterTurnState::Attack;
            }
            else if(curEnergy - moveEnergyCost >= 0)
            {
                state = Types::CharacterTurnState::Move;
            }
            else
            {
                //to break the loop.
                curEnergy = 0;
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
	if ((info.health -= amount) <= 0)
	{
		Die();
	}
}

Types::CharacterInfo& Character::GetCharacterInfo()
{
    return info;
}

void Character::SetGridBox(Types::GridBox* box)
{
    if (box == nullptr) return;

    //If the character was in other box, change it to empty.
    if (currentBox != nullptr)
    {
        currentBox->occupiedID = EMPTY_GRID;
    }

    //Set the new data.
    currentBox = box;
    currentBox->occupiedID = info.playerIndex;
}

bool Character::CheckTargetIsWithinAttackRange(Grid* grid)
{
    if (target == nullptr) return false;

    //Check if is within the attack range (8 directions)
    int attackRange = Helper::GetCharacterAttackRangeFromClass(info.characterClass);
    
    int curX = currentBox->xIndex;
    int curY = currentBox->yIndex;

    int targetX = target->currentBox->xIndex;
    int targetY = target->currentBox->yIndex;

    return (targetX >= (curX - attackRange) && targetX <= (curX + attackRange) &&
            targetY >= (curY - attackRange) && targetY <= (curY + attackRange));
}

void Character::SelectTarget(std::vector<std::shared_ptr<Character>> allCharacters)
{
    int closestDistance = INT_MAX;
    Character* closestCharacter = nullptr;

    //Check the nearest target
    for (std::vector<std::shared_ptr<Character>>::iterator it = allCharacters.begin(); it != allCharacters.end(); ++it)
    {
        //Don't check itself, and don't aim for the same type and don't aim for dead targets
        if (it->get() == this || ((*it)->info.flag == info.flag) || (*it)->info.isDead) continue;

        std::shared_ptr<Character> otherCharacter = *it;
        
        //Calculate distance
        int mDistance = Helper::CalculateManhattanDist(     currentBox->xIndex,
                                                            currentBox->yIndex,
                                                            otherCharacter->currentBox->xIndex,
                                                            otherCharacter->currentBox->yIndex);

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
    if (target == nullptr) return;

    std::vector<Types::GridBox*> boxesAround = grid->GetAllBoxesAroundGridBoxQuadSearch(currentBox->xIndex, currentBox->yIndex, 1, SEARCH_MASK_ONLY_UNOCCUPIED_BOXES);

    if (boxesAround.size() == 0)
    {
        //wait for a box to be unoccupied
        return;
    }

    int selectedBoxDist = INT_MAX;
    Types::GridBox* selectedBox = nullptr;

    for (std::vector<Types::GridBox*>::iterator it = boxesAround.begin(); it != boxesAround.end(); ++it)
    {
        Types::GridBox* itrBox = *it;
        int boxDistanceToTarget = Helper::CalculateManhattanDist(itrBox->xIndex, itrBox->yIndex, target->currentBox->xIndex, target->currentBox->yIndex);

        if (selectedBoxDist > boxDistanceToTarget)
        {
            selectedBoxDist = boxDistanceToTarget;
            selectedBox = itrBox;
        }
    }

    //Move
    SetGridBox(selectedBox);
}

void Character::Attack() 
{
    if (target == nullptr) return;

    target->TakeDamage(info.baseDamage + (info.baseDamage * info.damageMultiplier));
}

void Character::Die()
{
    //Set dead state, the system will handle this over.
    info.isDead = true;
}


