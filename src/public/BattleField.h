#pragma 

#include "Character.h"

class BattleField
{
public:
	
	BattleField();
	~BattleField();

	void Initialize();
	void UpdateBattleField();

private:

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
	std::shared_ptr<Grid> grid;
	Types::GameState gameState;
	std::vector<std::shared_ptr<Character>> allCharacters;
	int currentTurn;
	int numberOfPossibleTiles;
	int totalNumberOfPlayers;
	int totalNumberOfEnemies;
	std::bitset<FLAG_SIZE> winnerFlag;
};


