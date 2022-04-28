#pragma 

#include "Character.h"

#define MIN_GRID_X 4
#define MIN_GRID_Y 4
#define MAX_GRID_X 10
#define MAX_GRID_Y 10

class BattleField
{
public:
	
	BattleField();
	
	void Initialize();

	void DrawBattleField();

private:

	void Setup();

	int GetPlayerChoice();

	void CreatePlayerCharacter(int classIndex);

	void CreateEnemyCharacter();

	void StartGame();

	void StartTurn();

	void HandleTurn();

	void FinishedTurn();

	void AlocatePlayers();

	void AlocatePlayerCharacter();

	void AlocateEnemyCharacter();

	void OnGameEnd();

private:
	std::shared_ptr<Grid> grid;
	Types::GameState gameState;
	std::vector<std::shared_ptr<Character>> allCharacters;
	int currentTurn;
	int numberOfPossibleTiles;
	int totalNumberOfPlayers;
	int totalNumberOfEnemies;
};


