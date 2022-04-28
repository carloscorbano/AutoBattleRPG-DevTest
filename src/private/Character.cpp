#include "../public/Character.h"
#include "../public/BattleField.h"

#include <cmath>
#include <limits>

Character::Character(Types::CharacterClass characterClass, float health, float baseDamage, int index, char icon, int energy,
    Types::GridBox spawnLocation, Types::CharacterFlag flag)
    :   characterClass(characterClass), health(health), baseDamage(baseDamage), energy(energy), currentBox(spawnLocation),
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

    /* {

        if (CheckCloseTargets(battlefield))
        {
            Attack(Character::target);


            return;
        }
        else
        {   // if there is no target close enough, calculates in wich direction this character should move to be closer to a possible target


            if (currentBox.xIndex > target->currentBox.xIndex)
            {
                if (find(battlefield->grids.begin(), battlefield->grids.end(), currentBox.Index - 1) != battlefield->grids.end())

                {
                    currentBox.ocupied = false;
                    battlefield->grids[currentBox.Index] = currentBox;

                    currentBox = (battlefield->grids[currentBox.Index - 1]);
                    currentBox.ocupied = true;
                    battlefield->grids[currentBox.Index] = currentBox;
                    //Console.WriteLine($"Player {PlayerIndex} walked left\n");
                    battlefield->drawBattlefield(5, 5);

                    return;
                }
            }
            else if (currentBox.xIndex < target->currentBox.xIndex)
            {
                currentBox.ocupied = false;
                battlefield->grids[currentBox.Index] = currentBox;
                currentBox = (battlefield->grids[currentBox.Index + 1]);
                return;
                battlefield->grids[currentBox.Index] = currentBox;
                //Console.WriteLine($"Player {PlayerIndex} walked right\n");
                battlefield->drawBattlefield(5, 5);
            }

            if (currentBox.yIndex > target->currentBox.yIndex)
            {
                battlefield->drawBattlefield(5, 5);
                currentBox.ocupied = false;
                battlefield->grids[currentBox.Index] = currentBox;
                currentBox = battlefield->grids[(currentBox.Index - battlefield->xLenght)];
                currentBox.ocupied = true;
                battlefield->grids[currentBox.Index] = currentBox;
                //Console.WriteLine($"PlayerB {PlayerIndex} walked up\n");
                return;
            }
            else if (currentBox.yIndex < target->currentBox.yIndex)
            {
                currentBox.ocupied = true;
                battlefield->grids[currentBox.Index] = currentBox;
                currentBox = battlefield->grids[currentBox.Index + battlefield->xLenght];
                currentBox.ocupied = false;
                battlefield->grids[currentBox.Index] = currentBox;
                //Console.WriteLine($"Player {PlayerIndex} walked down\n");
                battlefield->drawBattlefield(5, 5);

                return;
            }
        }
    }
    */
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
    //Check if is within the attack range (8 directions)
    int attackRange = Helper::GetCharacterAttackRangeFromClass(characterClass);
    
    int curX = currentBox.xIndex;
    int curY = currentBox.yIndex;

    int targetX = target->currentBox.xIndex;
    int targetY = target->currentBox.yIndex;

    return (targetX >= (curX - attackRange) && targetX <= (curX + attackRange) &&
            targetY >= (curY - attackRange) && targetY <= (curY + attackRange));
}

void Character::SelectTarget(std::vector<std::shared_ptr<Character>> allCharacters)
{
    float closestDistance = INT_MAX;
    Character* closestCharacter = nullptr;

    //Check the nearest target
    for (auto it = allCharacters.begin(); it != allCharacters.end(); ++it)
    {
        //Don't check itself, and don't aim for the same type and don't aim for dead targets
        if (it->get() == this || ((*it)->GetFlag() == GetFlag()) || (*it)->IsDead()) continue;

        //Calculate distance
        float mDistance = CalculateDistance(currentBox, (*it)->currentBox);

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
    //find the fastest way to get to the target
    int curX = currentBox.xIndex;
    int curY = currentBox.yIndex;

    float nearestDistance = INT_MAX;
    int nextIndex = -1;

    //Iterate with neightbours gridboxes.
    for (int y = (curY - 1); y <= (curY + 1); ++y)
    {
        for (int x = (curX - 1); x <= (curX + 1); ++x)
        {
            //check if it is within the grid length and don't get the current grid.
            if (x < 0 || y < 0 || x >= (grid->xLength) || y >= (grid->yLength) || x == curX || y == curY) continue;

            //Get the cur box data.
            int index = grid->GetIndexFromColumnAndLine(x, y);

            //If the neightbour grid box is already beeing ocupied by another character.
            if (grid->grids[index].ocupiedID != -1) continue;

            float neightbourDistanceToTarget = CalculateDistance(grid->grids[index], target->currentBox);

            if (nearestDistance > neightbourDistanceToTarget)
            {
                nearestDistance = neightbourDistanceToTarget;
                nextIndex = index;
            }
        }
    }

    //Move
    grid->grids[currentBox.Index].ocupiedID = -1;
    grid->grids[nextIndex].ocupiedID = playerIndex;
    currentBox = grid->grids[nextIndex];
}

void Character::Attack() 
{
    if (target != nullptr)
    {
        target->TakeDamage(baseDamage + (baseDamage * damageMultiplier));
    }
}

void Character::Die()
{
    //Set dead state.
    isDead = true;
    // TODO >> kill
    //TODO >> end the game?
}

float Character::CalculateDistance(Types::GridBox a, Types::GridBox b)
{
    //Calculate distance
    int otherX = b.xIndex;
    int otherY = b.yIndex;

    float deltaX = static_cast<float>(otherX) - static_cast<float>(a.xIndex);
    float deltaY = static_cast<float>(otherY) - static_cast<float>(a.yIndex);

    //Manhattan distance
    return (fabs(deltaX) + fabs(deltaY));
}


