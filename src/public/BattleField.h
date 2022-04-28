#pragma 

#include "Character.h"
#include <list>

#define MIN_GRID_X 4
#define MIN_GRID_Y 4
#define MAX_GRID_X 10
#define MAX_GRID_Y 10

class BattleField
{
public:
	
	BattleField();
	
	void Initialize();

	static void ClearConsole();

private:

	void Setup();

	int GetPlayerChoice();

	void CreatePlayerCharacter(int classIndex);

	void CreateEnemyCharacter();

	void StartGame();

	void StartTurn();

	void HandleTurn();

	int GetRandomInt(int min, int max);

	void AlocatePlayers();

	void AlocatePlayerCharacter();

	void AlocateEnemyCharacter();

private:
	std::unique_ptr<Grid> grid;
	Types::GameState gameState;
	std::list<std::unique_ptr<Character>> allCharacters;
	int currentTurn;
	int numberOfPossibleTiles;
};


