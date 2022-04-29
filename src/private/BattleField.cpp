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
            Helper::WaitForMilliseconds(1000);
            gameState = Types::GameState::CreatingCharacters;
        }
            break;
        case Types::GameState::CreatingCharacters:
        {
            int choice = GetPlayerChoice();
            Helper::WaitForMilliseconds(1500);
            CreatePlayerCharacter(choice);
            CreateEnemyCharacter();

            printf("Starting the game...\n");
            Helper::WaitForMilliseconds(5000);
            gameState = Types::GameState::StartGame;
        }
            break;
        case Types::GameState::StartGame:
        {
            StartGame();
            SpawnCharacters();
            UpdateBattleField();
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
        }
            break;
        default:
            break;
        }
    }

    //Handles Game ending
    OnGameEnd();
}

void BattleField::UpdateBattleField()
{
    Helper::ClearConsole();
    
    for (std::vector<Types::GridBox>::iterator it = grid->grids.begin(); it != grid->grids.end(); ++it)
    {
        Types::GridBox current = *it;
        
        if ((current.xIndex % grid->xLength) == 0 && current.Index != 0)
        {
            printf("\n");
        }

        if (current.occupiedID == EMPTY_GRID)
        {
            printf("[ ]");
        }
        else
        {
            printf("[%c]", allCharacters[current.occupiedID]->GetCharacterInfo().icon);
        }
    }

    printf("\n");
}

void BattleField::Setup()
{    
    int x, y;
    x = y = -1;
    printf("> Choose a grid size, the limit for x is min: %i max: %i and y is min: %i max: %i.\n", MIN_GRID_X, MAX_GRID_X, MIN_GRID_Y, MAX_GRID_Y);
    printf("> For random values, just submit the field as blank field.\n");
    while (true)
    {
        x = ReadConsoleInt("X: ");

        if (x == -1)
        {
            x = Helper::GetRandomIntFromRange(MIN_GRID_X, MAX_GRID_X);
            break;
        }

        if (x >= MIN_GRID_X && x <= MAX_GRID_X)
        {
            break;
        }

        printf("[ERROR] Invalid value inserted for X. Please insert a valid value.\n");
    }
    
    while (true)
    {
        y = ReadConsoleInt("Y: ");

        if (y == -1)
        {
            y = Helper::GetRandomIntFromRange(MIN_GRID_Y, MAX_GRID_Y);
            break;
        }

        if (y >= MIN_GRID_Y && y <= MAX_GRID_Y)
        {
            break;
        }

        printf("[ERROR] Invalid value inserted for Y. Please insert a valid value.\n");
    }

    //reset variables.
    currentTurn = 0;
    totalNumberOfEnemies = totalNumberOfPlayers = 0;

    //Create Grid with the given x and y values.
    grid = std::make_shared<Grid>(x, y);
    printf("> Grid size defined to: [%i, %i]\n", x, y);
    printf("\n");
    numberOfPossibleTiles = static_cast<int>(grid->grids.size());
}

int BattleField::GetPlayerChoice()
{
    //asks for the player to choose between for possible classes via console.
    printf("> Choose Between One of this Classes:\n");
    printf("> [1] Paladin, [2] Warrior, [3] Cleric, [4] Archer\n");
    printf("> Leave empty for a random class.\n");
    int classID = 0;

    //store the selected class.
    while (true)
    {
         classID = ReadConsoleInt("Class number: ");

         if (classID == -1)
         {
             classID = Helper::GetRandomIntFromRange(MINIMAL_CLASS_ID, MAX_CLASS_ID);
             break;
         }

         if (classID >= MINIMAL_CLASS_ID && classID <= MAX_CLASS_ID)
         {
             break;
         }

         printf("[ERROR] Your choice was invalid, please enter the correct number.\n");
    }

    printf("> You are an almight %s!\n", Helper::GetStringFromCharacterClass(static_cast<Types::CharacterClass>(classID)).c_str());
    printf("\n");

    return classID;
}

void BattleField::CreatePlayerCharacter(int classIndex)
{
    printf("> What's the name of the character? (Leave empty for random name).\n");
    std::string name = ReadConsoleString("Name: ");

    if (name.empty())
    {
        name = Helper::GetRandomName();
    }
    
    printf("> The character name is: %s\n", name.c_str());
    printf("\n");

    //Get the character class
    Types::CharacterClass characterClass = static_cast<Types::CharacterClass>(classIndex);
    printf(">> Player Class Choice: %s\n", Helper::GetStringFromCharacterClass(characterClass).c_str());
    //get character status based on the class
    float health, baseDamage;
    int energy;
    Helper::BaseStatusFromCharacterClass(characterClass, health, baseDamage, energy);

    Types::CharacterInfo info{};

    info.name = name;
    info.health = health;
    info.baseDamage = baseDamage;
    info.energy = energy;
    info.flag.set(FLAG_OFFSET, FLAG_PLAYER);
    info.icon = PLAYER_CHARACTER_ICON;
    info.isDead = false;
    info.characterClass = characterClass;
    info.damageMultiplier = static_cast<float>(Helper::GetRandomIntFromRange(10, 100)) * 0.001f;

    allCharacters.push_back(std::make_shared<Character>(info));
    
    //Increase total players
    totalNumberOfPlayers++;
}

void BattleField::CreateEnemyCharacter()
{
    //randomly choose the enemy class and set up vital variables
    int randomInteger = Helper::GetRandomIntFromRange(MINIMAL_CLASS_ID, MAX_CLASS_ID);
    Types::CharacterClass enemyClass = (Types::CharacterClass)randomInteger;
    
    //Get status based on the class.
    float health, baseDamage;
    int energy;
    Helper::BaseStatusFromCharacterClass(enemyClass, health, baseDamage, energy);

    Types::CharacterInfo info{};

    info.name = Helper::GetRandomName();
    info.health = health;
    info.baseDamage = baseDamage;
    info.energy = energy;
    info.flag.set(FLAG_OFFSET, FLAG_ENEMY);
    info.icon = ENEMY_CHARACTER_ICON;
    info.isDead = false;
    info.characterClass = enemyClass;
    info.damageMultiplier = static_cast<float>(Helper::GetRandomIntFromRange(10, 100)) * 0.001f;

    allCharacters.push_back(std::make_shared<Character>(info));

    printf(">> Enemy Name: %s\n", info.name.c_str());
    printf(">> Enemy Class Choice: %s\n", Helper::GetStringFromCharacterClass(enemyClass).c_str());
    printf("\n");

    //increase total enemies
    totalNumberOfEnemies++;
}

void BattleField::SpawnCharacters()
{
    int index = 0;
    for (std::vector<std::shared_ptr<Character>>::iterator it = allCharacters.begin(); it != allCharacters.end(); ++it, ++index)
    {
        std::shared_ptr<Character> currentCharacter = *it;
        currentCharacter->GetCharacterInfo().playerIndex = index;

        while (true)
        {
            //Look for a place to spawn character that is not ocupied and 
            Types::GridBox& gb = grid->GetRandomAvailableGridInQuad(0, 0, grid->xLength - 1, grid->yLength - 1);

            //Check if there is not a character already spawned nearby
            size_t numberOfCharactersInRadius = grid->GetAllBoxesAroundGridBoxCircleSearch(gb.xIndex, gb.yIndex, SPAWN_CHARACTER_CHECK_RADIUS, false, SEARCH_MASK_ONLY_OCCUPIED_BOXES).size();

            if (numberOfCharactersInRadius == 0)
            {
                //spawn the character there
                currentCharacter->SetGridBox(&gb);
                break;
            }
            //If contains characters nearby, then try again.
        }
    }
}

void BattleField::StartGame()
{
    //shuffle character list to make a random pickup
    std::random_device rd;
    std::mt19937 generator(rd());
    std::shuffle(allCharacters.begin(), allCharacters.end(), generator);
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
        if (!current->GetCharacterInfo().isDead)
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
        if (current->GetCharacterInfo().isDead)
        {
            //0 == PLAYER, 1 == ENEMY
            if (current->GetCharacterInfo().flag.test(0))
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
        if (playerCount > 0)
        {
            winnerFlag.set(FLAG_OFFSET, FLAG_PLAYER);
        }
        else
        {
            winnerFlag.set(FLAG_OFFSET, FLAG_ENEMY);
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
    printf(">> Game ended!!!\n");
    //Present the victorious character
    if (winnerFlag.test(0))
    {
        printf("> The enemies won this game! Yay?\n");
    }
    else
    {
        printf("> The player won this game! Yay!\n");
    }

    int input = 0;

    //asks if the player wanna play again
    printf("> What do you wanna do? [0] play again or [1] quit.\n");

    while (true)
    {
        input = ReadConsoleInt("Choice: ");
        
        if (input == 0 || input == 1)
        {
            break;
        }

        printf("[ERROR] Your choice was invalid, please enter the correct number.\n");
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

int BattleField::ReadConsoleInt(std::string consoleMessage)
{
    //asks if the player wanna play again
    printf("%s", consoleMessage.c_str());
    //store the player choice in a variable
    std::string choice;
    int val;
    std::getline(std::cin, choice);

    if (!choice.empty())
    {
        std::stringstream ss(choice);
        if (ss >> val)
        {
            return val;
        }

        return -2;
    }

    return -1;
}

std::string BattleField::ReadConsoleString(std::string consoleMessage)
{
    //asks if the player wanna play again
    printf("%s", consoleMessage.c_str());
    //store the player choice in a variable
    std::string choice;
    std::getline(std::cin, choice);

    if (choice.empty())
    {
        return "";
    }

    return choice;
}
