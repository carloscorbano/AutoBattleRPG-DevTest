#include "../public/Console.h"

BattleFieldConsole::BattleFieldConsole()
	: hwnd(NULL), hAccelTable(NULL), msg(), inHandle(NULL), outHandle(NULL)
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

void BattleFieldConsole::DrawString(std::string str, WORD attributes, short posX, short posY)
{
	for (int i = 0; i < str.size(); ++i)
	{
		DrawChar(str[i], attributes, posX + i, posY);
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

RECT BattleFieldConsole::GetScreenSize()
{
	return screenSize;
}

bool BattleFieldConsole::SetupConsole()
{
	//Get the console window.
	hwnd = GetConsoleWindow();

	if (!hwnd)
	{
		printf("[ERROR] Failed to get console handler.\n");
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
		printf("Failed to get console handle");
		return false;
	}

	//Disable quick edit mode
	DWORD previous_mode;
	if (!GetConsoleMode(inHandle, &previous_mode))
	{
		printf("Failed to get console mode\n");
		return false;
	}

	previous_mode &= ~ENABLE_QUICK_EDIT_MODE;

	if (!SetConsoleMode(inHandle, ENABLE_EXTENDED_FLAGS | previous_mode))
	{
		printf("Failed to set console mode\n");
		return false;
	}

	//Disable maximize button
	DWORD style = GetWindowLong(hwnd, GWL_STYLE);
	SetWindowLong(hwnd, GWL_STYLE, (style & ~WS_MAXIMIZEBOX));
	SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);

	//Set console screen buffer size
	CONSOLE_SCREEN_BUFFER_INFOEX sbInfoEx{};
	sbInfoEx.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	if (!GetConsoleScreenBufferInfoEx(outHandle, &sbInfoEx))
	{
		printf("Failed to get console screen buffer.\n");
		return false;
	}

	short winWidth = sbInfoEx.srWindow.Right - sbInfoEx.srWindow.Left + 1;
	short winHeight = sbInfoEx.srWindow.Bottom - sbInfoEx.srWindow.Top + 1;

	short sbBufferWidth = sbInfoEx.dwSize.X;
	short sbBufferHeight = sbInfoEx.dwSize.Y;

	COORD newSize{};
	newSize.X = sbBufferWidth;
	newSize.Y = winHeight;

	//set the screenSize var
	screenSize.left = 0;
	screenSize.right = newSize.X;
	screenSize.bottom = 0;
	screenSize.top = newSize.Y;

	if (!SetConsoleScreenBufferSize(outHandle, newSize))
	{
		printf("Failed to set console screen buffer.\n");
		return false;
	}

	DWORD mode;
	if (!GetConsoleMode(outHandle, &mode))
	{
		printf("Failed to get console mode from out handle.\n");
		return false;
	}

	mode &= ~ENABLE_WRAP_AT_EOL_OUTPUT;

	if (!SetConsoleMode(outHandle, mode))
	{
		printf("Failed to set console mode outHandle.\n");
		return false;
	}

	//Hide cursor
	CONSOLE_CURSOR_INFO cursorInfo;
	if (!GetConsoleCursorInfo(outHandle, &cursorInfo))
	{
		printf("Failed to get cursor information.\n");
		return false;
	}
	cursorInfo.bVisible = FALSE;

	if (!SetConsoleCursorInfo(outHandle, &cursorInfo))
	{
		printf("Failed to set cursor information.\n");
		return false;
	}

	return true;
}

void BattleFieldConsole::UpdateInput()
{
	//make a backup from keys.
	for (int i = 0; i < keys.size(); ++i)
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

Types::KeyState BattleFieldConsole::GetKey(Types::Keys key)
{
	int k = static_cast<int>(key);
	if (keys[k] && old_keys[k])
	{
		return Types::KeyState::HOLD;
	}
	else if (keys[k] && !old_keys[k])
	{
		return Types::KeyState::DOWN;
	}
	else if(!keys[k] && old_keys[k])
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

void BattleFieldConsole::ListenKeyboardInput()
{
	tempKeyboardInput += AnyKey();
}

std::string BattleFieldConsole::RetrieveKeyboardInput()
{
	return tempKeyboardInput;
}

char BattleFieldConsole::AnyKey()
{
	for (std::map<int, bool>::iterator it = keys.begin(); it != keys.end(); ++it)
	{
		if (it->second)
		{
			return static_cast<char>(it->first);
		}
	}
}
