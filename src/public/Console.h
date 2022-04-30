#pragma once

#include "Types.h"

#include <iostream>
#include <map>

class BattleFieldConsole
{
public:
	

	BattleFieldConsole();
	~BattleFieldConsole();

	bool SetupConsole();
	void UpdateInput();
	void ClearConsoleScreen();
	void ClearConsoleArea(Types::ScreenRegionData area);

	Types::ScreenRegionData GetRegionData(Types::ConsoleRegions region);
	void CreateRegion(std::string name, Types::ScreenRegionData data);
	void DrawStringToRegion(std::string str, Types::ConsoleRegions region, bool clear = false);
	void ClearRegion(Types::ConsoleRegions region);
	bool ContainsRegion(std::string regionName);

	bool IsKeyDown(Types::Keys key);
	bool IsKeyUp(Types::Keys key);
	bool IsKeyHold(Types::Keys key);
	bool AnyKey(Types::KeyState state);

	void ClearKeyboardInput();
	void ListenKeyboardInput(Types::KeyState inputState, size_t maxLength, bool onlyNumbers = false);
	std::string RetrieveKeyboardInput();

	void DrawString(std::string str, Types::ScreenRegionData data);
	void DrawChar(char c, WORD attributes, short posX, short posY);
	/// <summary>
	/// Draw to the screen, DO NOT exceed charBufSize x*y=100!!! characterPos must be within 0 and x*y=100!
	/// </summary>
	void DrawToScreen(CHAR_INFO* charInfo, COORD& charBufSize, COORD& charPos, SMALL_RECT& writeArea);
	
	Types::ScreenAreaData& GetScreenData();

private:
	Types::KeyState GetKey(int key);

private:
	std::map<int, bool> old_keys;
	std::map<int, bool> keys;

	HWND hwnd;
	HANDLE inHandle;
	HANDLE outHandle;
	CONSOLE_SCREEN_BUFFER_INFOEX csBufferInfo;
	HACCEL hAccelTable;
	MSG msg;
	Types::ScreenAreaData screenData;

	std::map<std::string, Types::ScreenRegionData> regions;

	std::string tempKeyboardInput;
};