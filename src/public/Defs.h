#pragma once

#pragma region GRID_VARS
#define MIN_GRID_X 15
#define MIN_GRID_Y 10
#define MAX_GRID_X 30
#define MAX_GRID_Y 20
#define EMPTY_GRID -1
#pragma endregion GRID_VARS

#pragma region CHARACTER_VARS
#define MINIMAL_CLASS_ID 1
#define MAX_CLASS_ID 4

#define FLAG_OFFSET 0
#define FLAG_SIZE 1
#define FLAG_PLAYER 0
#define FLAG_ENEMY 1

//the radius to be checked while trying to spawn the character, to make sure that it's not random spawned nearby others.
#define SPAWN_CHARACTER_CHECK_RADIUS 5

#define PLAYER_CHARACTER_ICON 'P'
#define ENEMY_CHARACTER_ICON 'E'
#pragma endregion CHARACTER_VARS

#pragma region MASKS
#define SEARCH_MASK_ALL_BOXES 0
#define SEARCH_MASK_ONLY_OCCUPIED_BOXES 1
#define SEARCH_MASK_ONLY_UNOCCUPIED_BOXES 2
#pragma endregion MASKS