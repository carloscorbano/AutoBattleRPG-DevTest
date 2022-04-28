#include "../public/Character.h"

Character::Character(Types::CharacterClass characterClass, float health, float baseDamage, int index, char icon, int energy,
    Types::GridBox spawnLocation, Types::CharacterFlag flag)
    :   characterClass(characterClass), health(health), baseDamage(baseDamage), damageMultiplier(0), energy(energy), currentBox(spawnLocation),
        playerIndex(index), isDead(false), icon(icon)
{
    this->flag.set(0, static_cast<int>(flag));
}

Character::~Character()
{}

void Character::StartTurn(Grid* battlefield)
{
    //if this character is dead, then just return.
    if (isDead) return;

    //Set the initial state of the machine.
    Types::CharacterTurnState state = Types::CharacterTurnState::Checking;

    int curEnergy = energy;
    //Character AI state machine.
    while (curEnergy > 0)
    {
        switch (state)
        {
        case Types::CharacterTurnState::Checking:
        {
            curEnergy -= Helper::GetEnergyCostFromCharacterTurnState(state);
        }
            break;
        case Types::CharacterTurnState::SelectingTarget:
        {
            curEnergy -= Helper::GetEnergyCostFromCharacterTurnState(state);
        }
            break;
        case Types::CharacterTurnState::CheckRange:
        {
            curEnergy -= Helper::GetEnergyCostFromCharacterTurnState(state);
        }
            break;
        case Types::CharacterTurnState::Move:
        {
            curEnergy -= Helper::GetEnergyCostFromCharacterTurnState(state);
        }
            break;
        case Types::CharacterTurnState::Attack:
        {
            curEnergy -= Helper::GetEnergyCostFromCharacterTurnState(state);
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

bool Character::TakeDamage(float amount) 
{
	if ((health -= baseDamage) <= 0)
	{
		Die();
		return true;
	}
	return false;
}

int Character::GetIndex(std::vector<Types::GridBox*> v, int index)
{
    //return playerIndaex;
    return 0;
}

bool Character::IsDead() const
{
    return isDead;
}

std::bitset<FLAG_SIZE> Character::GetFlag() const
{
    return flag;
}

void Character::WalkTo(bool CanWalk) 
{

}

bool Character::CheckCloseTargets(Grid* battlefield)
{
    return false;
}

void Character::Attack(Character* target) 
{

}

void Character::SetTarget(Character* target)
{
    if (target != nullptr)
        target = target;
}

void Character::Die()
{
    //Set dead state.
    isDead = true;
    // TODO >> kill
    //TODO >> end the game?
}


