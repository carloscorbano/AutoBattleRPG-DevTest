#pragma 

#include "Character.h"

class BattleFieldConsole;

class BattleField
{
public:
	
	BattleField();
	~BattleField();

	void Initialize();
	void UpdateLog(std::string message);
	void UpdateBattleField();
	void UpdateCharactersInfoUI();

private:

	void UI_Intro();
	void CreateConsoleRegions();
	void UpdateTurnUI();

	void GameInitializationAndCleanup();
	void Setup();
	void CreateCharacters();

	bool WaitForInput(std::string& val, Types::KeyState state, int strTotalSize, bool onlyNumbers);
	void CreatePlayerCharacter(std::string name, int classIndex);
	void CreateEnemyCharacter();
	void SpawnCharacters();
	void StartGame();
	void StartTurn();
	void HandleTurn();
	void FinishedTurn();
	void OnGameEnd();

private:
	
	std::shared_ptr<BattleFieldConsole> console;
	std::shared_ptr<Grid> grid;
	Types::GameState gameState;
	Types::GameStateInternal gameStateInternal;
	std::vector<std::shared_ptr<Character>> allCharacters;
	int currentTurn;
	int internalControl;
	int totalNumberOfPlayers;
	int totalNumberOfEnemies;
	std::bitset<FLAG_SIZE> winnerFlag;

	bool quit;
	std::string tempName;
	int tempClassID;
};


