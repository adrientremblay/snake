#include <iostream>
#include <Windows.h>
using namespace std;

const int screenWidth = 120;
const int screenHeight = 40;

int main()
{
    wchar_t* screen = new wchar_t[screenWidth * screenHeight];
    for (int i = 0; i < screenWidth * screenHeight; i++) {
        screen[i] = '1';
    }

    cout << screen;

    HANDLE console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(console);
    DWORD bytesWritten = 0;

    while (true) 	{


        // draw screen to console 
        screen[screenWidth * screenHeight - 1] = '\0';
        WriteConsoleOutputCharacter(console, screen, screenWidth * screenHeight, { 0,0 }, &bytesWritten);
    }
}

