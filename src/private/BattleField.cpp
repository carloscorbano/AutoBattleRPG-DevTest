#include "../public/BattleField.h"

#include <iostream>
#include <sstream>

BattleField::BattleField()
{
    currentTurn = 0;
    gameState = Types::GameState::Setup;
    numberOfPossibleTiles = 0;
}

void BattleField::Initialize()
{
    gameState = Types::GameState::Setup;

    while (true)
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
            ClearConsole();
            CreatePlayerCharacter(choice);
            CreateEnemyCharacter();
            gameState = Types::GameState::GameEnded; //TEMP: change GameEnded to SelectingCharacter state.
        }
            break;
            ///MAIN GAME LOOP AREA
        case Types::GameState::MainGameLoop_SelectingCharacter:
            break;
        case Types::GameState::MainGameLoop_ResolvingCharacterTurn:
            break;
        case Types::GameState::MainGameLoop_FinishedTurn:
            break;
        case Types::GameState::MainGameLoop_CheckEndGame:
            break;
            ///----------- MAIN GAME LOOP AREA END
        case Types::GameState::Cleanup:
            break;
        case Types::GameState::GameEnded:
            break;
        default:
            break;
        }

        if (gameState == Types::GameState::GameEnded)
        {
            break;
        }
    }
}

void BattleField::ClearConsole()
{
    std::cout << "\x1B[2J\x1B[H";
}

void BattleField::Setup()
{
    //grid = new Grid(5, 5);
    //allCharacters = std::list<std::unique_ptr<Character>>();

    //asks for the player to choose the grid size
    printf("Choose a grid size, the limit for x is min: %i max: %i and y is min: %i max: %i. Example: x,y\n", MIN_GRID_X, MAX_GRID_X, MIN_GRID_Y, MAX_GRID_Y);
    std::string choice;
    int x, y;
    x = y = 0;
    
    while (std::getline(std::cin, choice))
    {
        //read line to get x and y
        std::stringstream ss;
        for (int i = 0; i < choice.size(); ++i)
        {
            //delim
            if (choice[i] == ',')
            {
                ss >> x;
                ss.str(std::string());
                ss.clear();
                ++i;
            }

            ss << choice[i];
        }

        ss >> y;

        //check value inserted
        if (x < MIN_GRID_X || x > MAX_GRID_X || y < MIN_GRID_Y || y > MAX_GRID_Y)
        {
            ClearConsole();
            printf("Invalid grid size inserted, please follow the example to setup correctly.\n");
            printf("Choose a grid size, the limit for x is min: %i max: %i and y is min: %i max: %i. Example: x,y\n", MIN_GRID_X, MAX_GRID_X, MIN_GRID_Y, MAX_GRID_Y);
        }
        else
        {
            break;
        }
    }

    currentTurn = 0;
    //numberOfPossibleTiles = grid->grids.size();
}

int BattleField::GetPlayerChoice()
{
    ClearConsole();
    //asks for the player to choose between for possible classes via console.
    printf("Choose Between One of this Classes:\n");

    printf("[1] Paladin, [2] Warrior, [3] Cleric, [4] Archer\n");
    //store the player choice in a variable
    std::string choice;
    int classID = 0;
    while (std::getline(std::cin, choice))
    {
        std::stringstream ss(choice);
        if (ss >> classID)
        {
            if (ss.eof() && (classID >= static_cast<int>(Types::CharacterClass::Paladin) && classID <= static_cast<int>(Types::CharacterClass::Archer)))
            {
                break;
            }
        }

        ClearConsole();
        printf("Error, your choice was invalid, please enter the correct number.\n");
        printf("Choose Between One of this Classes:\n");

        printf("[1] Paladin, [2] Warrior, [3] Cleric, [4] Archer\n");
    }

    return classID;
}

void BattleField::CreatePlayerCharacter(int classIndex)
{
    Types::CharacterClass characterClass = static_cast<Types::CharacterClass>(classIndex);
    printf("Player Class Choice: %s\n", Types::GetStringFromCharacterClass(characterClass).c_str());
    
    //playerCharacter = std::make_shared<Character>(characterClass);
    //playerCharacter->SetData(100, 20, 0);
}

void BattleField::CreateEnemyCharacter()
{
    //randomly choose the enemy class and set up vital variables
    
    int randomInteger = GetRandomInt(1, 4);
    Types::CharacterClass enemyClass = (Types::CharacterClass)randomInteger;
    printf("Enemy Class Choice: %s\n", Types::GetStringFromCharacterClass(enemyClass).c_str());
    //enemyCharacter = std::make_shared<Character>(enemyClass);
    //enemyCharacter->SetData(100, 20, 1);
}

void BattleField::StartGame()
{
    //populates the character variables and targets
    //enemyCharacter->SetTarget(playerCharacter);
    //playerCharacter->SetTarget(enemyCharacter);
    //allPlayers->push_back(playerCharacter);
    //allPlayers->push_back(enemyCharacter);
    //AlocatePlayers();
    //StartTurn();

}

void BattleField::StartTurn() {

    if (currentTurn == 0)
    {
        //AllPlayers.Sort();  
    }
    std::list<Character>::iterator it;

    //for (it = allPlayers->begin(); it != allPlayers->end(); ++it) {
    //    it->StartTurn(grid);
    //}

    currentTurn++;
    HandleTurn();
}

void BattleField::HandleTurn()
{
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

int BattleField::GetRandomInt(int min, int max)
{
    //Infinite loop
    //int index = GetRandomInt(min, max);
    //return index;
    return 0;
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