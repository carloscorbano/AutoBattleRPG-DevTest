#include "../public/BattleField.h"

#include <iostream>
#include <sstream>
#include <random>
#include <iterator>
#include <algorithm>

BattleField::BattleField()
{
    currentTurn = 0;
    gameState = Types::GameState::Setup;
    numberOfPossibleTiles = 0;
}

void BattleField::Initialize()
{
    gameState = Types::GameState::Setup;

    bool isRunning = true;
    while (isRunning)
    {
        switch (gameState)
        {
        case Types::GameState::Setup:
        {
            Setup();
            gameState = Types::GameState::CreatingCharacters;
        }
            break;
        case Types::GameState::CreatingCharacters:
        {
            int choice = GetPlayerChoice();
            CreatePlayerCharacter(choice);
            CreateEnemyCharacter();
            gameState = Types::GameState::StartGame;
        }
            break;
        case Types::GameState::StartGame:
        {
            StartGame();
            gameState = Types::GameState::MainGameLoop_StartTurn;
        }
            break;
            ///MAIN GAME LOOP AREA
        case Types::GameState::MainGameLoop_StartTurn:
        {
            StartTurn();
            gameState = Types::GameState::MainGameLoop_ResolvingTurn;
        }
            break;
        case Types::GameState::MainGameLoop_ResolvingTurn:
        {
            HandleTurn();
            gameState = Types::GameState::MainGameLoop_FinishedTurn;
        }
            break;
        case Types::GameState::MainGameLoop_FinishedTurn:
        {
            FinishedTurn();
        }
            break;
            ///----------- MAIN GAME LOOP AREA END
        case Types::GameState::GameEnded:
        {
            isRunning = false;
            OnGameEnd();
        }
            break;
        default:
            break;
        }
    }
}

void BattleField::Setup()
{    
    int x, y;

    while (true)
    {
        //asks for the player to choose the grid size
        printf("Choose a grid size, the limit for x is min: %i max: %i and y is min: %i max: %i. Example: x y\n", MIN_GRID_X, MAX_GRID_X, MIN_GRID_Y, MAX_GRID_Y);
        std::string line;

        //read line
        std::getline(std::cin, line);

        std::stringstream ss(line);
        if (ss >> x && ss >> y &&
            (x >= MIN_GRID_X && x <= MAX_GRID_X && y >= MIN_GRID_Y && y <= MAX_GRID_Y))
        {
            break;
        }

        printf("Invalid data, please follow the example format.\n");
    }

    currentTurn = 0;

    //Create Grid with the given x and y values.
    grid = std::make_shared<Grid>(x, y);
    printf("Grid size defined to: [%i, %i]\n", x, y);
    numberOfPossibleTiles = grid->grids.size();
}

int BattleField::GetPlayerChoice()
{
    //store the selected class.
    int classID = 0;
    while (true)
    {
        //asks for the player to choose between for possible classes via console.
        printf("Choose Between One of this Classes:\n");
        printf("[1] Paladin, [2] Warrior, [3] Cleric, [4] Archer\n");

        //store the player choice in a variable
        std::string choice;

        std::getline(std::cin, choice);
        std::stringstream ss(choice);
        //check input data
        if (ss >> classID &&
            (classID >= MINIMAL_CLASS_ID && classID <= MAX_CLASS_ID))
        {
            break;
        }

        printf("Error, your choice was invalid, please enter the correct number.\n");
    }

    return classID;
}

void BattleField::CreatePlayerCharacter(int classIndex)
{
    //Get the character class
    Types::CharacterClass characterClass = static_cast<Types::CharacterClass>(classIndex);
    printf("Player Class Choice: %s\n", Helper::GetStringFromCharacterClass(characterClass).c_str());
    //get character status based on the class
    float health, baseDamage;
    int energy;
    Helper::BaseStatusFromCharacterClass(characterClass, health, baseDamage, energy);

    //Choose a random location to spawn the player, this location must not have any other characters.
    Types::GridBox spawnLocation = grid->GetRandomAvailableGridBoxInQuad(0, 0, grid->xLenght / 2, grid->yLength / 2);

    //Create character object.
    allCharacters.push_back(std::make_shared<Character>(Character(characterClass, health, baseDamage, allCharacters.size(),
        PLAYER_CHARACTER_ICON, energy, spawnLocation, Types::CharacterFlag::Player)));
    //Increase total players
    ++totalNumberOfPlayers;
}

void BattleField::CreateEnemyCharacter()
{
    //randomly choose the enemy class and set up vital variables
    int randomInteger = Helper::GetRandomIntFromRange(MINIMAL_CLASS_ID, MAX_CLASS_ID);
    Types::CharacterClass enemyClass = (Types::CharacterClass)randomInteger;
    printf("Enemy Class Choice: %s\n", Helper::GetStringFromCharacterClass(enemyClass).c_str());
    
    //Get status based on the class.
    float health, baseDamage;
    int energy;
    Helper::BaseStatusFromCharacterClass(enemyClass, health, baseDamage, energy);

    //Choose a random location to spawn the enemy, this location must not have any other characters.
    Types::GridBox spawnLocation = grid->GetRandomAvailableGridBoxInQuad(grid->xLenght / 2, grid->yLength / 2, grid->xLenght - 1, grid->yLength - 1);
    
    //Create character obj
    allCharacters.push_back(std::make_shared<Character>(Character(enemyClass, health, baseDamage, allCharacters.size(),
        ENEMY_CHARACTER_ICON, energy, spawnLocation, Types::CharacterFlag::Enemy)));

    //increase total enemies
    ++totalNumberOfEnemies;
}

void BattleField::StartGame()
{
    //shuffle character list to make a random pickup
    std::random_device rd;
    std::mt19937 generator(rd());
    std::shuffle(allCharacters.begin(), allCharacters.end(), generator);

    //populates the character variables and targets
    //enemyCharacter->SetTarget(playerCharacter);
    //playerCharacter->SetTarget(enemyCharacter);
    //allPlayers->push_back(playerCharacter);
    //allPlayers->push_back(enemyCharacter);
    //AlocatePlayers();
    //StartTurn();

}

void BattleField::StartTurn()
{
    //Increase turn.
    ++currentTurn;


    //if (currentTurn == 0)
    //{
    //    //AllPlayers.Sort();  
    //}
    //std::list<Character>::iterator it;
    //
    ////for (it = allPlayers->begin(); it != allPlayers->end(); ++it) {
    ////    it->StartTurn(grid);
    ////}
    //
    //currentTurn++;
    //HandleTurn();
}

void BattleField::HandleTurn()
{
    for (auto it = allCharacters.begin(); it != allCharacters.end(); ++it)
    {
        auto current = *it;
        if (!current->IsDead())
        {
            current->StartTurn(grid.get());
        }
    }

    //if (playerCharacter->IsDead())
    //{
    //    return;
    //}
    //else if (enemyCharacter->IsDead())
    //{
    //    printf("\n");
    //
    //    // endgame?
    //
    //    printf("\n");
    //
    //    return;
    //}
    //else
    //{
    //    printf("\n");
    //    printf("Click on any key to start the next turn...\n");
    //    printf("\n");
    //
    //    //TODO
    //    //ConsoleKeyInfo key = Console.ReadKey();
    //    StartTurn();
    //}
}

void BattleField::FinishedTurn()
{
    //update count number
    int playerCount = totalNumberOfPlayers;
    int enemyCount = totalNumberOfEnemies;

    for (auto it = allCharacters.begin(); it != allCharacters.end(); ++it)
    {
        auto current = *it;
        if (current->IsDead())
        {
            //0 == PLAYER, 1 == ENEMY
            if (current->GetFlag().test(0))
            {
                --enemyCount;
            }
            else
            {
                --playerCount;
            }
        }
    }

    if (playerCount == 0 || enemyCount == 0)
    {
        gameState = Types::GameState::GameEnded;
    }
    else
    {
        //wait for the player enter any key.
        printf("Press any key to start the next turn...\n");
        std::cin.get();

        gameState = Types::GameState::MainGameLoop_StartTurn;
    }
}

void BattleField::AlocatePlayers()
{
    AlocatePlayerCharacter();

}

void BattleField::AlocatePlayerCharacter()
{
    //int random = 0;
    //auto l_front = grid->grids.begin();
    //advance(l_front, random);
    //Types::GridBox* RandomLocation = &*l_front;
    //
    //if (!RandomLocation->ocupied)
    //{
    //    //Types::GridBox* PlayerCurrentLocation = RandomLocation;
    //    playerCurrentLocation = &*l_front;
    //    l_front->ocupied = true;
    //    playerCharacter->currentBox = *l_front;
    //    AlocateEnemyCharacter();
    //}
    //else
    //{
    //    AlocatePlayerCharacter();
    //}
}

void BattleField::AlocateEnemyCharacter()
{
    
    //int random = 24;
    //auto l_front = grid->grids.begin();
    //advance(l_front, random);
    //Types::GridBox* RandomLocation = &*l_front;
    //
    //if (!RandomLocation->ocupied)
    //{
    //    enemyCurrentLocation = &*l_front;
    //    l_front->ocupied = true;
    //    enemyCharacter->currentBox = *l_front;
    //    grid->drawBattlefield(5, 5);
    //}
    //else
    //{
    //    AlocateEnemyCharacter();
    //}


}

void BattleField::OnGameEnd()
{
    //TODO: Do something here.
    printf("Game ended\n");
}