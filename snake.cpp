#include <iostream>
#include <chrono>
#include <Windows.h>
using namespace std;

const int screenWidth = 120;
const int screenHeight = 40;

int headX = 12;
int headY = 12;

int main()
{
    HANDLE console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(console);
    DWORD bytesWritten = 0;

    // creating screen
    wchar_t* screen = new wchar_t[screenWidth * screenHeight];
	for (int i = 0; i < screenWidth * screenHeight; i++)
		screen[i] = ' ';

    // game loop
    while (true) 	{
        // movement controls
        auto timeLastMovement = chrono::system_clock::now();
        if ((chrono::system_clock::now() - timeLastMovement).count() < 1000) {
            if (GetAsyncKeyState((unsigned short)'W') & 0x8000) {
				headY -= 1;
                timeLastMovement = chrono::system_clock::now();
            }
        }

        // draw head
        screen[(headY * screenWidth) + headX] = 'X';

        // render screen to console 
        screen[screenWidth * screenHeight - 1] = '\0';
        WriteConsoleOutputCharacter(console, screen, screenWidth * screenHeight, { 0,0 }, &bytesWritten);
    }
}

