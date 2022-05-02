#include "../public/Console.h"
#include "../public/Helper.h"

BattleFieldConsole::BattleFieldConsole()
	: hwnd(NULL), inHandle(NULL), outHandle(NULL), csBufferInfo(), tempKeyboardInput()
{
	keys = std::map<int, bool>();
	for (int i = 0; i < 256; ++i)
	{
		keys.emplace(i, false);
	}

	old_keys = std::map<int, bool>(keys);
}

BattleFieldConsole::~BattleFieldConsole()
{}

void BattleFieldConsole::DrawString(std::string str, Types::ScreenRegionData data)
{
	if (str.empty() || str.size() > MAX_STR_SIZE_TO_DRAW) return;

	short size = static_cast<short>(str.size());

	int cX = data.X + (data.width / 2) - (size / 2);
	int cY = data.Y + (data.height / 2);

	for (short i = 0; i < size; ++i)
	{
		DrawChar(str[i], data.wAttributes, cX + i, cY);
	}
}

void BattleFieldConsole::DrawChar(char c, WORD attributes, short posX, short posY)
{
	SMALL_RECT position = { posX, posY, posX + 1, posY + 1 };
	COORD charBufSize = { 1, 1 };
	COORD charPos = { 0, 0 };
	CHAR_INFO info{};
	info.Char.UnicodeChar = c;
	info.Attributes = attributes;

	DrawToScreen(&info, charBufSize, charPos, position);
}

void BattleFieldConsole::DrawToScreen(CHAR_INFO* charInfo, COORD& charBufSize, COORD& charPos, SMALL_RECT& writeArea)
{
	//safe guard
	if (charBufSize.X * charBufSize.Y > 100 || charPos.X * charPos.Y > 100)
	{
		return;
	}

	WriteConsoleOutput(outHandle, charInfo, charBufSize, charPos, &writeArea);
}

Types::ScreenAreaData& BattleFieldConsole::GetScreenData()
{
	return screenData;
}

bool BattleFieldConsole::SetupConsole()
{
	//Get the console window.
	hwnd = GetConsoleWindow();

	if (!hwnd)
	{
		TRACE(L"[ERROR] Failed to get console handler.\n");
		return false;
	}

	//Hide scrollbar
	ShowScrollBar(hwnd, SB_VERT, FALSE);
	ShowScrollBar(hwnd, SB_HORZ, FALSE);

	//Set console size
	//center the console screen in the screen area
	int posX = (GetSystemMetrics(SM_CXSCREEN) / 2) - (CONSOLE_WIDTH / 2);
	int posY = (GetSystemMetrics(SM_CYSCREEN) / 2) - (CONSOLE_HEIGHT / 2);

	MoveWindow(hwnd, posX, posY, CONSOLE_WIDTH, CONSOLE_HEIGHT, TRUE);

	//remove maximize
	HMENU hmenu = GetSystemMenu(hwnd, FALSE);
	EnableMenuItem(hmenu, SC_MAXIMIZE, FALSE);

	//Set title
	SetConsoleTitle(CONSOLE_TITLE);

	//Get the console handle
	inHandle = GetStdHandle(STD_INPUT_HANDLE);
	outHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	if (!inHandle || !outHandle)
	{
		TRACE(L"Failed to get console handle.\n");
		return false;
	}

	//Disable quick edit mode
	DWORD previous_mode;
	if (!GetConsoleMode(inHandle, &previous_mode))
	{
		TRACE(L"Failed to get console mode.\n");
		return false;
	}

	previous_mode &= ~ENABLE_QUICK_EDIT_MODE;

	if (!SetConsoleMode(inHandle, ENABLE_EXTENDED_FLAGS | previous_mode))
	{
		TRACE(L"Failed to set console mode.\n");
		return false;
	}

	//Disable maximize button
	DWORD style = GetWindowLong(hwnd, GWL_STYLE);
	SetWindowLong(hwnd, GWL_STYLE, (style & ~WS_MAXIMIZEBOX));
	SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);

	//Set console screen buffer size
	csBufferInfo.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	if (!GetConsoleScreenBufferInfoEx(outHandle, &csBufferInfo))
	{
		TRACE(L"Failed to get console screen buffer.\n");
		return false;
	}

	short winWidth = csBufferInfo.srWindow.Right - csBufferInfo.srWindow.Left + 1;
	short winHeight = csBufferInfo.srWindow.Bottom - csBufferInfo.srWindow.Top + 1;

	short sbBufferWidth = csBufferInfo.dwSize.X;
	short sbBufferHeight = csBufferInfo.dwSize.Y;

	COORD newSize{};
	newSize.X = sbBufferWidth;
	newSize.Y = winHeight;

	//set the screenSize var
	screenData.width = newSize.X;
	screenData.height = newSize.Y;
	screenData.centerX = newSize.X / 2;
	screenData.centerY = newSize.Y / 2;

	if (!SetConsoleScreenBufferSize(outHandle, newSize))
	{
		TRACE(L"Failed to set console screen buffer.\n");
		return false;
	}

	DWORD mode;
	if (!GetConsoleMode(outHandle, &mode))
	{
		TRACE(L"Failed to get console mode from out handle.\n");
		return false;
	}

	mode &= ~ENABLE_WRAP_AT_EOL_OUTPUT;

	if (!SetConsoleMode(outHandle, mode))
	{
		TRACE(L"Failed to set console mode outHandle.\n");
		return false;
	}

	//Hide cursor
	CONSOLE_CURSOR_INFO cursorInfo;
	if (!GetConsoleCursorInfo(outHandle, &cursorInfo))
	{
		TRACE(L"Failed to get cursor information.\n");
		return false;
	}
	cursorInfo.bVisible = FALSE;

	if (!SetConsoleCursorInfo(outHandle, &cursorInfo))
	{
		TRACE(L"Failed to set cursor information.\n");
		return false;
	}

	return true;
}

void BattleFieldConsole::UpdateInput()
{
	//make a backup from keys.
	for (int i = 0; i < static_cast<int>(keys.size()); ++i)
	{
		old_keys[i] = keys[i];
		keys[i] = false; //reset
	}

	//update value.
	for (std::map<int, bool>::iterator it = keys.begin(); it != keys.end(); ++it)
	{
		it->second = (GetKeyState(it->first) & KEY_DOWN);
	}
}

void BattleFieldConsole::ClearConsoleScreen()
{
	Types::ScreenRegionData area{};
	area.X = 0;
	area.Y = 0;
	area.width = screenData.width;
	area.height = screenData.height;
	ClearConsoleArea(area);
}

void BattleFieldConsole::ClearConsoleArea(Types::ScreenRegionData area)
{
	//flush cout buffer
	std::cout.flush();
	DWORD written;
	DWORD length = area.width * area.height;
	COORD screenCoord = { area.X, area.Y };
	if (!FillConsoleOutputCharacter(outHandle, TEXT(' '), length, screenCoord, &written))
	{
		TRACE(L"Failed to fill console output characters.\n");
		return;
	}

	if (!FillConsoleOutputAttribute(outHandle, csBufferInfo.wAttributes, length, screenCoord, &written))
	{
		TRACE(L"Failed to fill console output attribute.\n");
		return;
	}

	if (!SetConsoleCursorPosition(outHandle, { 0,0 }))
	{
		TRACE(L"Failed to set cursor position.\n");
		return;
	}
}

Types::ScreenRegionData BattleFieldConsole::GetRegionData(Types::ConsoleRegions region)
{
	std::string name = Helper::GetRegionName(region);
	if (ContainsRegion(name))
	{
		return regions[name];
	}

	Types::ScreenRegionData srd{};
	return srd;
}

void BattleFieldConsole::CreateRegion(std::string name, Types::ScreenRegionData data)
{
	if (!ContainsRegion(name))
	{
		regions.emplace(name, data);
	}
}

void BattleFieldConsole::DrawStringToRegion(std::string str, Types::ConsoleRegions region, bool clear)
{
	std::string regionName = Helper::GetRegionName(region);
	if (clear)
	{
		ClearRegion(region);
	}

	if (ContainsRegion(regionName))
	{
		Types::ScreenRegionData data = regions[regionName];
		DrawString(str, data);
	}
}

void BattleFieldConsole::ClearRegion(Types::ConsoleRegions region)
{
	std::string regionName = Helper::GetRegionName(region);
	if (ContainsRegion(regionName))
	{
		ClearConsoleArea(regions[regionName]);
	}
}

bool BattleFieldConsole::ContainsRegion(std::string regionName)
{
	for (std::map<std::string, Types::ScreenRegionData>::iterator it = regions.begin(); it != regions.end(); ++it)
	{
		if (it->first == regionName)
		{
			return true;
		}
	}

	return false;
}

bool BattleFieldConsole::IsKeyDown(Types::Keys key)
{
	return GetKey(static_cast<int>(key)) == Types::KeyState::DOWN;
}

bool BattleFieldConsole::IsKeyUp(Types::Keys key)
{
	return GetKey(static_cast<int>(key)) == Types::KeyState::UP;
}

bool BattleFieldConsole::IsKeyHold(Types::Keys key)
{
	return GetKey(static_cast<int>(key)) == Types::KeyState::HOLD;
}

bool BattleFieldConsole::AnyKey(Types::KeyState state)
{
	for (std::map<int, bool>::iterator it = keys.begin(); it != keys.end(); ++it)
	{
		if (GetKey(it->first) == state)
		{
			return true;
		}
	}
	return false;
}

Types::KeyState BattleFieldConsole::GetKey(int key)
{
	if (keys[key] && old_keys[key])
	{
		return Types::KeyState::HOLD;
	}
	else if (keys[key] && !old_keys[key])
	{
		return Types::KeyState::DOWN;
	}
	else if(!keys[key] && old_keys[key])
	{
		return Types::KeyState::UP;
	}
	else
	{
		return Types::KeyState::NONE;
	}
}

void BattleFieldConsole::ClearKeyboardInput()
{
	tempKeyboardInput = std::string();
}

void BattleFieldConsole::ListenKeyboardInput(Types::KeyState inputState, size_t maxLength, bool onlyNumbers)
{
	for (std::map<int, bool>::iterator it = keys.begin(); it != keys.end(); ++it)
	{
		int val = it->first;
		
		if (onlyNumbers &&
			((val < static_cast<int>(Types::Keys::CONSOLE_KEY_0)		||
			  val > static_cast<int>(Types::Keys::CONSOLE_KEY_9)))		&&
			  val != static_cast<int>(Types::Keys::CONSOLE_KEY_BACK)	&&
			  val != static_cast<int>(Types::Keys::CONSOLE_KEY_ENTER)
			) continue;

		if (GetKey(val) == inputState)
		{

			switch (val)
			{
			case static_cast<int>(Types::Keys::CONSOLE_KEY_BACK):
			{
				if (tempKeyboardInput.length() > 0)
				{
					tempKeyboardInput.pop_back();
				}
			}
				break;
			case static_cast<int>(Types::Keys::CONSOLE_KEY_ENTER):
				break;
			default:
				if (tempKeyboardInput.length() + 1 <= maxLength)
				{
					tempKeyboardInput += static_cast<char>(val);
				}
				break;
			}
		}
	}
}

std::string BattleFieldConsole::RetrieveKeyboardInput()
{
	return tempKeyboardInput;
}
