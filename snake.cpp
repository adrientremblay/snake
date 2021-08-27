#include <iostream>
#include <chrono>
#include <Windows.h>
#include <queue>
using namespace std;

const int screenWidth = 120;
const int screenHeight = 40;

struct point {
    int x;
    int y;
};

const float speed = 1.5f;
queue<point> *snake = new queue<point>();

int headX = 12;
int headY = 12;

enum Direction {UP, DOWN, LEFT, RIGHT};
Direction playerDirection = UP;

int main()
{
    snake->push({ headX, headY });

    HANDLE console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(console);
    DWORD bytesWritten = 0;

    // creating screen
    wchar_t* screen = new wchar_t[screenWidth * screenHeight];
	for (int i = 0; i < screenWidth * screenHeight; i++)
		screen[i] = ' ';

	auto lastTime = chrono::system_clock::now();
    float movementDelayCounter = 0.0f;
    float timeDelta;
    // game loop
    while (true) 	{
        // calculating time delta 
        auto timeNow = chrono::system_clock::now();
        timeDelta = (timeNow - lastTime).count();
        lastTime = timeNow;

        // movement controls
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
            playerDirection = UP;
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
            playerDirection = DOWN;
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
            playerDirection = LEFT;
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
            playerDirection = RIGHT;

        // move head according to direction
        if (movementDelayCounter >= 1000000 * speed) { // every <speed> seconds
            switch (playerDirection) {
                case UP:
                    headY -= 1;
                    break;
                case DOWN:
                    headY += 1;
                    break;
                case LEFT:
                    headX -= 1;
                    break;
                case RIGHT:
                    headX += 1;
                    break;
            }
            movementDelayCounter = 0;
        } else {
            movementDelayCounter += timeDelta;
        }

        // handle wall looping(collision)
        if (headX >= screenWidth)
            headX = 0;
        if (headX < 0)
            headX = screenWidth- 1;
        if (headY >= screenHeight)
            headY = 0;
        if (headY < 0)
            headY = screenHeight- 1;

        // handle track removal
        point p = snake->front();
        if (! (GetAsyncKeyState((unsigned short)'I') & 0x8000)) {
            snake->pop();
        }
        screen[(p.y * screenWidth) + p.x] = ' ';
        snake->push({ headX, headY });

        // draw head
        screen[(headY * screenWidth) + headX] = 'X';

        // render screen to console 
        screen[screenWidth * screenHeight - 1] = '\0';
        WriteConsoleOutputCharacter(console, screen, screenWidth * screenHeight, { 0,0 }, &bytesWritten);
    }
}

