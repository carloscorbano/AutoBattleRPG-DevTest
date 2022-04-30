#include "../public/Character.h"
#include "../public/BattleField.h"

#include <cmath>
#include <sstream>
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
    state = Types::CharacterTurnState::DecidingAction;

    int curEnergy = info.energy;
    int movements_left = MAX_CHARACTER_MOVEMENTS_IN_EACH_TURN;
    int attacks_left = MAX_CHARACTER_ATTACKS_IN_EACH_TURN;
    //Character AI state machine.

    while (curEnergy > 0)
    {
        switch (state)
        {
        case Types::CharacterTurnState::DecidingAction:
        {
            DecideAction(movements_left, attacks_left, curEnergy, *grid);
        }
            break;
        case Types::CharacterTurnState::SelectingTarget:
        {
            SelectTarget(allCharacters);

            if (target == nullptr)
            {
                curEnergy = 0;
                return;
            }

            state = Types::CharacterTurnState::DecidingAction;
        }
            break;
        case Types::CharacterTurnState::Move:
        {
            WalkTo(grid);
            battlefield->UpdateBattleField();
            
            if (target != nullptr)
            {
                std::string msgLog;
                std::stringstream ss(msgLog);
                ss << info.name << " moved towards " << target->GetCharacterInfo().name;
                battlefield->UpdateLog(ss.str());
            }

            movements_left--;
            curEnergy -= Helper::GetEnergyCostFromCharacterTurnState(state);
            state = Types::CharacterTurnState::DecidingAction;
            Helper::WaitForMilliseconds(250);
        }
            break;
        case Types::CharacterTurnState::Attack:
        {
            float damage = Attack();

            if (target != nullptr)
            {
                std::string msgLog;
                std::stringstream ss(msgLog);
                ss << info.name << " attacked " << target->GetCharacterInfo().name << " dealing " << damage;
                battlefield->UpdateLog(ss.str());
                battlefield->UpdateCharactersInfoUI();
            }

            attacks_left--;
            curEnergy -= Helper::GetEnergyCostFromCharacterTurnState(state);
            state = Types::CharacterTurnState::DecidingAction;
            Helper::WaitForMilliseconds(250);
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

void Character::DecideAction(int& movements_left, int& attacks_left, int& curEnergy, Grid& grid)
{
    //Check if the player needs a new target.
    if (target == nullptr || target->info.isDead)
    {
        state = Types::CharacterTurnState::SelectingTarget;
        return;
    }

    //Get the cost of actions.
    int moveEnergyCost = Helper::GetEnergyCostFromCharacterTurnState(Types::CharacterTurnState::Move);
    int attackEnergyCost = Helper::GetEnergyCostFromCharacterTurnState(Types::CharacterTurnState::Attack);

    bool targetIsInRange = CheckTargetIsWithinAttackRange(&grid);

    //Check if the player can attack (priority to attack over movement)
    switch (MAX_CHARACTER_ATTACKS_IN_EACH_TURN)
    {
    case 0: //0 means that is not limited by the attacks left, just limited by the energy.
    {
        if (targetIsInRange && curEnergy - attackEnergyCost >= 0)
        {
            state = Types::CharacterTurnState::Attack;
            return;
        }
    }
        break;
    case 1: //is limited by attacks left.
    {
        if (targetIsInRange && attacks_left > 0)
        {
            state = Types::CharacterTurnState::Attack;
            return;
        }
    }
        break;
    default:
        break;
    }

    switch (MAX_CHARACTER_MOVEMENTS_IN_EACH_TURN)
    {
    case 0: //0 means that is not limited by the movements left, just limited by the energy.
    {
        if (!targetIsInRange && curEnergy - moveEnergyCost >= 0)
        {
            state = Types::CharacterTurnState::Move;
            return;
        }
    }
        break;
    case 1: //is limited by movements left.
    {
        if (!targetIsInRange && movements_left > 0)
        {
            state = Types::CharacterTurnState::Move;
            return;
        }
    }
        break;
    default:
        break;
    }

    //if there is any action to do (all of above) then just exit turn.
    curEnergy = 0;
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

    std::vector<Types::GridBox*> boxesAround = grid->GetAllBoxesAroundGridBoxQuadSearch(currentBox->xIndex, currentBox->yIndex, 1, false, SEARCH_MASK_ONLY_UNOCCUPIED_BOXES);

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

float Character::Attack() 
{
    if (target == nullptr) return 0;

    float damage = info.baseDamage + (info.baseDamage * info.damageMultiplier);
    target->TakeDamage(damage);

    return damage;
}

void Character::Die()
{
    //Set dead state, the system will handle this over.
    info.isDead = true;
}


