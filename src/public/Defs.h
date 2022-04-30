#pragma once

#pragma region CONSOLE_VARS

#define CONSOLE_WIDTH 1200
#define CONSOLE_HEIGHT 768
#define CONSOLE_TITLE L"Auto-battle Turn RPG system - [Kokku Software Engineer Applicant Test]"

#define KEY_DOWN 0x8000
#define MAX_STR_SIZE_TO_DRAW 200
#define DEFAULT_STR_HEIGHT 1
#pragma endregion CONSOLE_VARS

#pragma region GRID_VARS
#define EMPTY_GRID -1
#pragma endregion GRID_VARS

#pragma region CHARACTER_VARS
#define MINIMAL_CLASS_ID 1
#define MAX_CLASS_ID 4

#define FLAG_OFFSET 0
#define FLAG_SIZE 1
#define FLAG_PLAYER 0
#define FLAG_ENEMY 1

//how many times character can move in each turn (0 = limited only by the energy).
#define MAX_CHARACTER_MOVEMENTS_IN_EACH_TURN 1
//how many times character can attack in each turn (0 = limited only by the energy).
#define MAX_CHARACTER_ATTACKS_IN_EACH_TURN 1

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

#pragma region MACROS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define TRACE(...) OutputDebugString(__VA_ARGS__);
#pragma endregion MACROS