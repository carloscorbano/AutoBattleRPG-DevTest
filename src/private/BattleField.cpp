#include "../public/BattleField.h"
#include "../public/Console.h"

#include <sstream>
#include <random>
#include <iterator>
#include <algorithm>

BattleField::BattleField()
    :   totalNumberOfEnemies(0), totalNumberOfPlayers(0), currentTurn(0), internalControl(0),
        gameState(Types::GameState::UI_INTRO), gameStateInternal(Types::GameStateInternal::DrawUI),
        tempName(), tempClassID(), quit(false)
{
    grid = std::make_shared<Grid>();
}

BattleField::~BattleField()
{}

void BattleField::Initialize()
{
    GameInitializationAndCleanup();

    bool isRunning = true;
    while (isRunning)
    {
        //Update input data.
        console->UpdateInput();

        switch (gameState)
        {
        case Types::GameState::UI_INTRO:
        {
            UI_Intro();
        }
            break;
        case Types::GameState::Setup:
        {
            Setup();
        }
            break;
        case Types::GameState::CreatingCharacters:
        {
            CreateCharacters();
        }
            break;
        case Types::GameState::StartGame:
        {
            StartGame();
        }
            break;
        case Types::GameState::MainGameLoop_StartTurn:
        {
            StartTurn();
        }
            break;
        case Types::GameState::MainGameLoop_ResolvingTurn:
        {
            HandleTurn();
        }
            break;
        case Types::GameState::MainGameLoop_FinishedTurn:
        {
            FinishedTurn();
        }
            break;
        case Types::GameState::GameEnded:
        {
            isRunning = false;
        }
            break;
        default:
            break;
        }
        
        Helper::WaitForMilliseconds(1);
    }

    OnGameEnd();
}

void BattleField::UpdateLog(std::string message)
{
    console->DrawStringToRegion(message, Types::ConsoleRegions::BATTLEFIELD_LOG, true);
}

void BattleField::UpdateBattleField()
{
    console->ClearRegion(Types::ConsoleRegions::BATTLEFIELD);

    Types::ScreenRegionData data = console->GetRegionData(Types::ConsoleRegions::BATTLEFIELD);
    int bX, bY;
    bX = data.X - ((grid->xLength * 3) / 2);
    bY = data.Y - ((grid->yLength * 1) / 2);

    for (std::vector<Types::GridBox>::iterator it = grid->grids.begin(); it != grid->grids.end(); ++it)
    {
        Types::GridBox currentGridBox = *it;

        Types::ScreenRegionData temp{};
        temp.X = bX + (currentGridBox.xIndex * 3);
        temp.Y = bY + (currentGridBox.yIndex * 1);
        temp.width = 3;
        temp.height = 1;

        if (currentGridBox.occupiedID == EMPTY_GRID)
        {
            temp.wAttributes = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
            console->DrawString("[ ]", temp);
        }
        else
        {
            std::string str;
            std::stringstream ss(str);
            Types::CharacterInfo info = allCharacters[currentGridBox.occupiedID]->GetCharacterInfo();

            ss << "[" << info.icon << "]";

            if (info.flag.test(FLAG_OFFSET) == FLAG_PLAYER)
            {
                temp.wAttributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_BLUE;
            }
            else
            {
                temp.wAttributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
            }

            console->DrawString(ss.str(), temp);
        }
    }
}

void BattleField::UI_Intro()
{
    switch (gameStateInternal)
    {
    case Types::GameStateInternal::DrawUI:
    {
        //Draw intro
        Types::ScreenAreaData screenData = console->GetScreenData();
        std::string title = "Welcome to the auto-battle Turn RPG system!";
        std::string subTitle = "- Press [ENTER] to start or [ESC] to quit -";

        console->DrawStringToRegion(title, Types::ConsoleRegions::TITLE1);
        console->DrawStringToRegion(subTitle, Types::ConsoleRegions::AWNSER);

        gameStateInternal = Types::GameStateInternal::Logic;
    }
        break;
    case Types::GameStateInternal::Logic:
    {
        if (console->IsKeyDown(Types::Keys::CONSOLE_KEY_ENTER))
        {
            Types::ScreenAreaData screenData = console->GetScreenData();
            gameState = Types::GameState::Setup;
            console->ClearConsoleScreen();
            internalControl = 0;
        }
        else if (console->IsKeyDown(Types::Keys::CONSOLE_KEY_ESCAPE))
        {
            gameState = Types::GameState::GameEnded;
            quit = true;
        }
    }
        break;
    default:
        break;
    }
}

void BattleField::CreateConsoleRegions()
{
    //Create Regions
    Types::ScreenAreaData saData = console->GetScreenData();
    Types::ScreenRegionData srData{};

    //TITLE region
    srData.X = 0;
    srData.Y = saData.centerY - (saData.centerY / 2);
    srData.width = saData.width;
    srData.height = static_cast<short>(DEFAULT_STR_HEIGHT);
    srData.wAttributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    console->CreateRegion(Helper::GetRegionName(Types::ConsoleRegions::TITLE1), srData);

    //TITLE2
    srData.Y = srData.Y + 1;
    console->CreateRegion(Helper::GetRegionName(Types::ConsoleRegions::TITLE2), srData);

    //Question1
    srData.Y = srData.Y + 5;
    console->CreateRegion(Helper::GetRegionName(Types::ConsoleRegions::QUESTION1), srData);

    //Question2
    srData.Y = srData.Y + 1;
    console->CreateRegion(Helper::GetRegionName(Types::ConsoleRegions::QUESTION2), srData);

    //Awnser area
    srData.Y = srData.Y + 2;
    srData.wAttributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    console->CreateRegion(Helper::GetRegionName(Types::ConsoleRegions::AWNSER), srData);

    //Error area
    srData.Y = saData.height - 1;
    srData.wAttributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
    console->CreateRegion(Helper::GetRegionName(Types::ConsoleRegions::_ERROR), srData);

    //IN GAME AREAS
    //GRID_PLAYER_TITLE
    srData.X = 5;
    srData.Y = 1;
    srData.width = saData.width / 5;
    srData.height = 1;
    srData.wAttributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    console->CreateRegion(Helper::GetRegionName(Types::ConsoleRegions::GRID_PLAYER_TITLE), srData);
    
    //GRID_PLAYER_SUBTITLE
    srData.Y += 1;
    console->CreateRegion(Helper::GetRegionName(Types::ConsoleRegions::GRID_PLAYER_SUBTITLE), srData);

    //GRID_ENEMY_TITLE,
    srData.X = saData.width - srData.width - 5;
    srData.Y = 1;
    srData.wAttributes = FOREGROUND_RED;
    console->CreateRegion(Helper::GetRegionName(Types::ConsoleRegions::GRID_ENEMY_TITLE), srData);

    //GRID_ENEMY_SUBTITLE,
    srData.Y += 1;
    console->CreateRegion(Helper::GetRegionName(Types::ConsoleRegions::GRID_ENEMY_SUBTITLE), srData);

    //BATTLEFIELD_LOG_TITLE
    srData.width = saData.width / 2;
    srData.height = 1;
    srData.X = saData.centerX - (srData.width / 2);
    srData.Y = saData.height - (saData.height / 10);
    srData.wAttributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    console->CreateRegion(Helper::GetRegionName(Types::ConsoleRegions::BATTLEFIELD_LOG_TITLE), srData);

    //BATTLEFIELD_LOG,
    srData.Y += 2;
    srData.wAttributes = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY;
    console->CreateRegion(Helper::GetRegionName(Types::ConsoleRegions::BATTLEFIELD_LOG), srData);

    //BATTLEFIELD,
    srData.width = grid->xLength;
    srData.height = grid->yLength;
    srData.X = saData.centerX;
    srData.Y = saData.centerY;
    srData.wAttributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
    console->CreateRegion(Helper::GetRegionName(Types::ConsoleRegions::BATTLEFIELD), srData);

    //BATTLEFIELD_BORDER,
    srData.width = grid->maxXLength;
    srData.height = grid->maxYLength;
    srData.X = saData.centerX - (srData.width / 2);
    srData.Y = saData.height - (srData.height / 2);
    console->CreateRegion(Helper::GetRegionName(Types::ConsoleRegions::BATTLEFIELD_BORDER), srData);

    //TURN_TITLE
    srData.width = saData.width / 4;
    srData.height = 1;
    srData.X = saData.centerX - (srData.width / 2);
    srData.Y = 3;
    srData.wAttributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    console->CreateRegion(Helper::GetRegionName(Types::ConsoleRegions::TURN_TITLE), srData);
}

void BattleField::UpdateCharactersInfoUI()
{
    for (std::vector<std::shared_ptr<Character>>::iterator it = allCharacters.begin(); it != allCharacters.end(); ++it)
    {
        std::shared_ptr<Character> curCharacter = *it;
        Types::CharacterInfo cInfo = curCharacter->GetCharacterInfo();

        if (cInfo.flag.test(FLAG_OFFSET) == FLAG_PLAYER)
        {
            std::string str;
            std::stringstream ss(str);
            ss << "Player: " << cInfo.name << " [" << Helper::GetStringFromCharacterClass(cInfo.characterClass) << "]";
            console->DrawStringToRegion(ss.str(), Types::ConsoleRegions::GRID_PLAYER_TITLE, true);

            ss.str(std::string());
            ss << "Health: " << cInfo.health;
            console->DrawStringToRegion(ss.str(), Types::ConsoleRegions::GRID_PLAYER_SUBTITLE, true);
        }
        else
        {
            std::string str;
            std::stringstream ss(str);
            ss << "Enemy: " << cInfo.name << " [" << Helper::GetStringFromCharacterClass(cInfo.characterClass) << "]";
            console->DrawStringToRegion(ss.str(), Types::ConsoleRegions::GRID_ENEMY_TITLE, true);

            ss.str(std::string());
            ss << "Health: " << cInfo.health;
            console->DrawStringToRegion(ss.str(), Types::ConsoleRegions::GRID_ENEMY_SUBTITLE, true);
        }
    }
}

void BattleField::UpdateTurnUI()
{
    std::string str;
    std::stringstream ss(str);
    ss << "Current Turn: " << currentTurn;
    console->DrawStringToRegion(ss.str(), Types::ConsoleRegions::TURN_TITLE, true);
}

void BattleField::GameInitializationAndCleanup()
{
    if (allCharacters.size() > 0)
    {
        for (int i = 0; i < static_cast<int>(allCharacters.size()); ++i)
        {
            allCharacters[i].reset();
        }

        allCharacters.clear();
    }
    
    if (grid->grids.size() > 0)
    {
        grid->grids.clear();
    }

    internalControl = 0;
    totalNumberOfEnemies = totalNumberOfPlayers = 0;
    currentTurn = 0;
    gameStateInternal = Types::GameStateInternal::DrawUI;
    gameState = Types::GameState::UI_INTRO;
    quit = false;

    if (console.get() == nullptr)
    {
        console = std::make_shared<BattleFieldConsole>();
        console->SetupConsole();

        Types::ScreenAreaData data = console->GetScreenData();
        grid->minXLength = data.width / 10;
        grid->maxXLength = data.width / 4;
        grid->minYLength = data.height / 10;
        grid->maxYLength = data.height / 2;

        CreateConsoleRegions();
    }  

    console->ClearConsoleScreen();
}

void BattleField::Setup()
{   
    switch (internalControl)
    {
        case 0: //draw to screen
        {
            console->DrawStringToRegion("--[ SETUP ] --", Types::ConsoleRegions::TITLE1, false);
            
            std::string fieldA;
            std::stringstream ss(fieldA);
            ss << "Choose a grid size X (Minimum value is " << std::to_string(grid->minXLength) << ", Maximum value is " << std::to_string(grid->maxXLength) << ")";
            
            std::string subFieldA = "Leave blank for random number.";

            console->DrawStringToRegion(ss.str(), Types::ConsoleRegions::QUESTION1, false);
            console->DrawStringToRegion(subFieldA, Types::ConsoleRegions::QUESTION2, false);

            internalControl = 1;
        }
            break;
        case 1: //wait input
        {
            std::string val;
            if (WaitForInput(val, Types::KeyState::DOWN, 2, true))
            {
                if (val.empty())
                {
                    grid->xLength = Helper::GetRandomIntFromRange(grid->minXLength, grid->maxXLength);
                    internalControl = 2;

                    break;
                }

                std::stringstream ss(val);
                int x;
                if (ss >> x && x >= grid->minXLength && x <= grid->maxXLength)
                {
                    grid->xLength = x;
                    internalControl = 2;
                    break;
                }
                else
                {
                    //show error
                    std::string errorMsg;
                    std::stringstream ss(errorMsg);
                    ss << "[ERROR] Invalid number inserted. Please enter between " << std::to_string(grid->minXLength) << " - " << std::to_string(grid->maxXLength);
                    console->DrawStringToRegion(ss.str(), Types::ConsoleRegions::_ERROR, true);
                }
            }
        }
            break;
        case 2:
        {
            //Clear data before proceed
            console->ClearRegion(Types::ConsoleRegions::_ERROR);
            console->ClearRegion(Types::ConsoleRegions::AWNSER);
            console->ClearKeyboardInput();

            Types::ScreenAreaData data = console->GetScreenData();
            
            std::string fieldA;
            std::stringstream ss(fieldA);
            ss << "Choose a grid size Y (Minimum value is " << std::to_string(grid->minYLength) << ", Maximum value is " << std::to_string(grid->maxYLength) << ")";
            console->DrawStringToRegion(ss.str(), Types::ConsoleRegions::QUESTION1, true);
            
            internalControl = 3;
        }
            break;
        case 3: //read next input
        {
            std::string val;
            if (WaitForInput(val, Types::KeyState::DOWN, 2, true))
            {
                if (val.empty())
                {
                    grid->yLength = Helper::GetRandomIntFromRange(grid->minYLength, grid->maxYLength);
                    internalControl = 4;

                    break;
                }

                std::stringstream ss(val);
                int y;
                if (ss >> y && y >= grid->minYLength && y <= grid->maxYLength)
                {
                    grid->yLength = y;
                    internalControl = 4;
                    break;
                }
                else
                {
                    //show error
                    std::string errorMsg;
                    std::stringstream ss(errorMsg);
                    ss << "[ERROR] Invalid number inserted. Please enter between " << std::to_string(grid->minYLength) << " - " << std::to_string(grid->maxYLength);
                    console->DrawStringToRegion(ss.str(), Types::ConsoleRegions::_ERROR, true);
                }
            }
        }
            break;
        case 4: //create grid data
        {
            //Clear data before proceed
            console->ClearRegion(Types::ConsoleRegions::_ERROR);
            console->ClearRegion(Types::ConsoleRegions::AWNSER);
            console->ClearKeyboardInput();

            grid->CreateGrid();
            gameState = Types::GameState::CreatingCharacters;
            internalControl = 0;
            console->ClearConsoleScreen();

            std::string info;
            std::stringstream ss(info);
            ss << "Creating grid of size-> X:" << std::to_string(grid->xLength) << " | Y: " << std::to_string(grid->yLength) << ".";
            console->DrawStringToRegion(ss.str(), Types::ConsoleRegions::AWNSER);
            Helper::WaitForMilliseconds(3000);
        }
            break;
    default:
        break;
    }
}

void BattleField::CreateCharacters()
{
    switch (internalControl)
    {
        case 0: //show creation UI
        {
            console->ClearConsoleScreen();

            console->DrawStringToRegion("--[ CHARACTER CREATION ]--", Types::ConsoleRegions::TITLE1);

            console->DrawStringToRegion("Please enter the character name.", Types::ConsoleRegions::QUESTION1);
            console->DrawStringToRegion("[Leave blank for random generated name]", Types::ConsoleRegions::QUESTION1);

            internalControl = 1;
        }
            break;
        case 1: //Listen for input
        {
            std::string val;
            if (WaitForInput(val, Types::KeyState::DOWN, 12, false))
            {
                if (val.empty())
                {
                    tempName = Helper::GetRandomName();
                }
                else
                {
                    tempName = val;
                }

                internalControl = 2;
            }
        }
            break;
        case 2: //asks class
        {
            //Clear data before proceed
            console->ClearRegion(Types::ConsoleRegions::_ERROR);
            console->ClearRegion(Types::ConsoleRegions::AWNSER);
            console->ClearKeyboardInput();

            console->DrawStringToRegion("Please choose between the classes. [blank = random class]", Types::ConsoleRegions::QUESTION1, true);
            console->DrawStringToRegion("[1] Paladin, [2] Warrior, [3] Cleric, [4] Archer", Types::ConsoleRegions::QUESTION2, true);
            internalControl = 3;
        }
            break;
        case 3: //Wait for input
        {
            std::string val;
            if (WaitForInput(val, Types::KeyState::DOWN, 1, true))
            {
                if (val.empty())
                {
                    tempClassID = Helper::GetRandomIntFromRange(MINIMAL_CLASS_ID, MAX_CLASS_ID);
                    internalControl = 4;
                    break;
                }

                std::stringstream ss(val);
                if (ss >> tempClassID && tempClassID >= MINIMAL_CLASS_ID && tempClassID <= MAX_CLASS_ID)
                {
                    internalControl = 4;
                    break;
                }
                else
                {
                    std::string errorMsg;
                    std::stringstream ss(errorMsg);
                    ss << "[ERROR] Invalid number inserted. Please enter between " << std::to_string(MINIMAL_CLASS_ID) << " - " << std::to_string(MAX_CLASS_ID);
                    console->DrawStringToRegion(ss.str(), Types::ConsoleRegions::_ERROR, true);
                }
            }
        }
            break;
        case 4:
        {
            //Clear data before proceed
            console->ClearRegion(Types::ConsoleRegions::_ERROR);
            console->ClearRegion(Types::ConsoleRegions::AWNSER);
            console->ClearKeyboardInput();
            //create character obj.
            CreatePlayerCharacter(tempName, tempClassID);
            CreateEnemyCharacter();

            tempName = std::string();
            tempClassID = 0;

            internalControl = 0;

            gameState = Types::GameState::StartGame;

            console->ClearConsoleScreen();
        }
            break;
        default:
            break;
    }
}

bool BattleField::WaitForInput(std::string& val, Types::KeyState state, int strTotalSize, bool onlyNumbers)
{
    console->ListenKeyboardInput(state, strTotalSize, onlyNumbers);

    if (console->AnyKey(state))
    {
        std::string kInput = console->RetrieveKeyboardInput();
        console->ClearRegion(Types::ConsoleRegions::_ERROR);
        console->DrawStringToRegion(kInput, Types::ConsoleRegions::AWNSER, true);

        if (console->IsKeyDown(Types::Keys::CONSOLE_KEY_ENTER))
        {
            val = console->RetrieveKeyboardInput();
            return true;
        }
    }

    return false;
}

void BattleField::CreatePlayerCharacter(std::string name, int classIndex)
{
    //Get the character class
    Types::CharacterClass characterClass = static_cast<Types::CharacterClass>(classIndex);
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
    switch (internalControl)
    {
        case 0: //shuffle character list to make the turn sequence
        {
            //shuffle character list to make a random pickup
            std::random_device rd;
            std::mt19937 generator(rd());
            std::shuffle(allCharacters.begin(), allCharacters.end(), generator);

            internalControl = 1;
        }
            break;
        case 1: //spawn characters
        {
            SpawnCharacters();

            internalControl = 2;
        }
            break;
        case 2: //present Character list UI to client
        {
            currentTurn = 0;
            console->DrawStringToRegion("Game Log:", Types::ConsoleRegions::BATTLEFIELD_LOG_TITLE);
            UpdateCharactersInfoUI();
            UpdateBattleField();
            UpdateLog("Starting the game.");
            UpdateTurnUI();

            gameState = Types::GameState::MainGameLoop_StartTurn;
        }
            break;
        default:
            break;
    }
}

void BattleField::StartTurn()
{
    ////Increase turn.
    currentTurn++;

    UpdateTurnUI();

    gameState = Types::GameState::MainGameLoop_ResolvingTurn;
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

    gameState = Types::GameState::MainGameLoop_FinishedTurn;
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
    //clear screen
    console->ClearConsoleScreen();
    //Present the victorious character
    for (std::vector<std::shared_ptr<Character>>::iterator it = allCharacters.begin(); it != allCharacters.end(); ++it)
    {
        std::shared_ptr<Character> curCharacter = *it;
        Types::CharacterInfo info = curCharacter->GetCharacterInfo();
        if (!info.isDead)
        {
            std::string str;
            std::stringstream ss(str);
            ss << "Congratulations! " << info.name << " you have just won the match!";
            console->DrawStringToRegion(ss.str(), Types::ConsoleRegions::TITLE1);

            if (info.flag.test(0))
            {
                console->DrawStringToRegion("Victory of the Enemy!", Types::ConsoleRegions::TITLE2);
            }
            else
            {
                console->DrawStringToRegion("Victory of the Player!", Types::ConsoleRegions::TITLE2);
            }

            break;
        }
    }

    if (!quit)
    {
        Helper::WaitForMilliseconds(3000);

        Initialize();
    }
}