#pragma 

#include "Character.h"

#define MIN_GRID_X 15
#define MIN_GRID_Y 10
#define MAX_GRID_X 30
#define MAX_GRID_Y 25

class BattleField
{
public:
	
	BattleField();
	~BattleField();

	void Initialize();
	void DrawBattleField();

private:

	void Setup();
	int  GetPlayerChoice();
	void CreatePlayerCharacter(int classIndex);
	void CreateEnemyCharacter();
	void StartGame();
	void StartTurn();
	void HandleTurn();
	void FinishedTurn();
	void OnGameEnd();

private:
	std::shared_ptr<Grid> grid;
	Types::GameState gameState;
	std::vector<std::shared_ptr<Character>> allCharacters;
	int currentTurn;
	int numberOfPossibleTiles;
	int totalNumberOfPlayers;
	int totalNumberOfEnemies;
	std::bitset<1> winnerFlag;
};


