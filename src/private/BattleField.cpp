#include "../public/BattleField.h"

#include <iostream>
#include <sstream>
#include <random>
#include <iterator>
#include <algorithm>

BattleField::BattleField()
{
    totalNumberOfEnemies = 0;
    totalNumberOfPlayers = 0;
    currentTurn = 0;
    gameState = Types::GameState::Setup;
    numberOfPossibleTiles = 0;
}

BattleField::~BattleField()
{
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

    int input = 0;
    while (true)
    {
        //asks if the player wanna play again
        printf("What do you wanna do? [0] play again or [1] quit.\n");

        //store the player choice in a variable
        std::string choice;

        std::getline(std::cin, choice);
        std::stringstream ss(choice);
        //check input data
        if (ss >> input)
        {
            break;
        }

        printf("Error, your choice was invalid, please enter the correct number.\n");
    }

    if (input == 0)
    {
        //cleanup
        grid.reset();

        for (auto it = allCharacters.begin(); it != allCharacters.end(); ++it)
        {
            it->reset();
        }

        //Clear
        allCharacters.clear();

        Helper::ClearConsole();

        //initialize
        Initialize();
    }
}

void BattleField::DrawBattleField()
{
    Helper::ClearConsole();
    
    for (std::vector<Types::GridBox>::iterator it = grid->grids.begin(); it != grid->grids.end(); ++it)
    {
        Types::GridBox current = *it;
        
        if ((current.xIndex % grid->xLength) == 0 && current.Index != 0)
        {
            printf("\n");
        }

        if (current.ocupiedID == -1)
        {
            printf("[ ]");
        }
        else
        {
            printf("[%c]", allCharacters[current.ocupiedID]->GetIcon());
        }
    }

    printf("\n");
}

void BattleField::Setup()
{    
    int x, y;

    while (true)
    {
        //asks for the player to choose the grid size
        printf("Choose a grid size, the limit for x is min: %i max: %i and y is min: %i max: %i. Example: x y\n", MIN_GRID_X, MAX_GRID_X, MIN_GRID_Y, MAX_GRID_Y);
        printf("If you want random sizes, just leave this empty.\n");
        std::string line;

        //read line
        std::getline(std::cin, line);

        if (line.empty())
        {
            x = Helper::GetRandomIntFromRange(MIN_GRID_X, MAX_GRID_X);
            y = Helper::GetRandomIntFromRange(MIN_GRID_Y, MAX_GRID_Y);

            break;
        }

        std::stringstream ss(line);
        if (ss >> x && ss >> y &&
            (x >= MIN_GRID_X && x <= MAX_GRID_X && y >= MIN_GRID_Y && y <= MAX_GRID_Y))
        {
            break;
        }

        printf("Invalid data, please follow the example format.\n");
    }

    //reset variables.
    currentTurn = 0;
    totalNumberOfEnemies = totalNumberOfPlayers = 0;

    //Create Grid with the given x and y values.
    grid = std::make_shared<Grid>(x, y);
    printf("Grid size defined to: [%i, %i]\n", x, y);
    numberOfPossibleTiles = static_cast<int>(grid->grids.size());
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
        printf("Leave empty for a random class.\n");

        //store the player choice in a variable
        std::string choice;

        std::getline(std::cin, choice);

        if (choice.empty())
        {
            classID = Helper::GetRandomIntFromRange(MINIMAL_CLASS_ID, MAX_CLASS_ID);
            break;
        }

        std::stringstream ss(choice);
        //check input data
        if (ss >> classID &&
            (classID >= MINIMAL_CLASS_ID && classID <= MAX_CLASS_ID))
        {
            break;
        }

        printf("Error, your choice was invalid, please enter the correct number.\n");
    }

    printf("You are an almight %s!\n", Helper::GetStringFromCharacterClass(static_cast<Types::CharacterClass>(classID)).c_str());

    return classID;
}

void BattleField::CreatePlayerCharacter(int classIndex)
{
    //Get the player name here
    printf("What's the name of the character? (Leave empty for random name).\n");
    std::string name;
    std::getline(std::cin, name);

    if (name.empty())
    {
        name = Helper::GetRandomName();
    }

    printf("The character name is: %s\n", name.c_str());
    system("pause");

    //Get the character class
    Types::CharacterClass characterClass = static_cast<Types::CharacterClass>(classIndex);
    printf("Player Class Choice: %s\n", Helper::GetStringFromCharacterClass(characterClass).c_str());
    //get character status based on the class
    float health, baseDamage;
    int energy;
    Helper::BaseStatusFromCharacterClass(characterClass, health, baseDamage, energy);

    //Choose a random location to spawn the player, this location must not have any other characters.
    int playerID = static_cast<int>(allCharacters.size());
    //Spawn player at the left side of the field
    int spawnLocation = grid->GetRandomAvailableIndexInQuad(0, 0, grid->xLength / 4, grid->yLength - 1);
    grid->grids[spawnLocation].ocupiedID = playerID;

    //Create character object.
    allCharacters.push_back(std::make_shared<Character>(Character(name, characterClass, health, baseDamage, playerID,
        PLAYER_CHARACTER_ICON, energy, &grid->grids[spawnLocation], Types::CharacterFlag::Player)));
    //Increase total players
    totalNumberOfPlayers++;
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
    int playerID = static_cast<int>(allCharacters.size());
    //Spawn enemy at right side of the field
    int spawnLocation = grid->GetRandomAvailableIndexInQuad(grid->xLength - (grid->xLength / 4), 0, grid->xLength - 1, grid->yLength - 1);
    grid->grids[spawnLocation].ocupiedID = playerID;

    //Create character obj
    allCharacters.push_back(std::make_shared<Character>(Character(Helper::GetRandomName(), enemyClass, health, baseDamage, playerID,
        ENEMY_CHARACTER_ICON, energy, &grid->grids[spawnLocation], Types::CharacterFlag::Enemy)));

    //increase total enemies
    totalNumberOfEnemies++;
}

void BattleField::StartGame()
{
    //shuffle character list to make a random pickup
    std::random_device rd;
    std::mt19937 generator(rd());
    std::shuffle(allCharacters.begin(), allCharacters.end(), generator);

    //update characters index after shuffle.
    for (int i = 0; i < allCharacters.size(); ++i)
    {
        allCharacters[i]->SetPlayerIndex(i);
    }

    DrawBattleField();
}

void BattleField::StartTurn()
{
    //Increase turn.
    currentTurn++;
}

void BattleField::HandleTurn()
{
    for (std::vector<std::shared_ptr<Character>>::iterator it = allCharacters.begin(); it != allCharacters.end(); ++it)
    {
        std::shared_ptr<Character> current = *it;
        if (!current->IsDead())
        {
            current->StartTurn(this, grid.get(), allCharacters);
        }
    }
}

void BattleField::FinishedTurn()
{
    //update count number
    int playerCount = totalNumberOfPlayers;
    int enemyCount = totalNumberOfEnemies;

    for (std::vector<std::shared_ptr<Character>>::iterator it = allCharacters.begin(); it != allCharacters.end(); ++it)
    {
        std::shared_ptr<Character> current = *it;
        if (current->IsDead())
        {
            //0 == PLAYER, 1 == ENEMY
            if (current->GetFlag().test(0))
            {
                enemyCount--;
            }
            else
            {
                playerCount--;
            }
        }
    }

    if (playerCount == 0 || enemyCount == 0)
    {
        //Set the winner flag.
        if (playerCount == 0)
        {
            winnerFlag.set(0, FLAG_ENEMY);
        }
        else
        {
            winnerFlag.set(0, FLAG_PLAYER);
        }

        gameState = Types::GameState::GameEnded;
    }
    else
    {
        Helper::WaitForMilliseconds(500);
        gameState = Types::GameState::MainGameLoop_StartTurn;
    }
}

void BattleField::OnGameEnd()
{
    printf("Game ended!!!\n");
    //Present the victorious character
    if (winnerFlag.test(0))
    {
        printf("The enemies won this game! Yay?\n");
    }
    else
    {
        printf("The player won this game! Yay!\n");
    }
}