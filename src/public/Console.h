#pragma once

#include "Types.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <map>

class BattleFieldConsole
{
public:
	

	BattleFieldConsole();
	~BattleFieldConsole();

	bool SetupConsole();
	void UpdateInput();

	Types::KeyState GetKey(Types::Keys key);

	void ClearKeyboardInput();
	void ListenKeyboardInput();
	std::string RetrieveKeyboardInput();

	char AnyKey();

	void DrawString(std::string str, WORD attributes, short posX, short posY);
	void DrawChar(char c, WORD attributes, short posX, short posY);
	/// <summary>
	/// Draw to the screen, DO NOT exceed charBufSize x*y=100!!! characterPos must be within 0 and x*y=100!
	/// </summary>
	void DrawToScreen(CHAR_INFO* charInfo, COORD& charBufSize, COORD& charPos, SMALL_RECT& writeArea);
	
	RECT GetScreenSize();

private:
	std::map<int, bool> old_keys;
	std::map<int, bool> keys;

	HWND hwnd;
	HANDLE inHandle;
	HANDLE outHandle;
	HACCEL hAccelTable;
	MSG msg;
	RECT screenSize;

	std::string tempKeyboardInput;
};