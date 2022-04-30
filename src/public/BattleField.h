#pragma 

#include "Character.h"

class BattleFieldConsole;

class BattleField
{
public:
	
	BattleField();
	~BattleField();

	void Initialize();
	void UpdateBattleField();

private:

	void UI_Intro();

	void Setup();
	int  GetPlayerChoice();
	void CreatePlayerCharacter(int classIndex);
	void CreateEnemyCharacter();
	void SpawnCharacters();
	void StartGame();
	void StartTurn();
	void HandleTurn();
	void FinishedTurn();
	void OnGameEnd();

	int ReadConsoleInt(std::string consoleMessage);
	std::string ReadConsoleString(std::string consoleMessage);

private:
	bool redrawnUI;
	std::shared_ptr<BattleFieldConsole> console;
	std::shared_ptr<Grid> grid;
	Types::GameState gameState;
	std::vector<std::shared_ptr<Character>> allCharacters;
	int currentTurn;
	int numberOfPossibleTiles;
	int totalNumberOfPlayers;
	int totalNumberOfEnemies;
	std::bitset<FLAG_SIZE> winnerFlag;
};


