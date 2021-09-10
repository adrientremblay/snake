#include <iostream>
#include <chrono>
#include <Windows.h>
#include <queue>
#include <stdlib.h>
using namespace std;

int calculateScreenPosition(int x, int y);
void spawnApple();
void setupGame();

wchar_t* screen;

const int screenWidth = 120;
const int screenHeight = 40;

struct point {
    int x;
    int y;
};

const float speed = 1.25f;
queue<point>* snake;

int headX;
int headY; 
enum Direction {UP, DOWN, LEFT, RIGHT};
Direction playerDirection;

int lvl;

bool alive;

int main()
{
    HANDLE console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(console);
    DWORD bytesWritten = 0;

    // creating screen
    screen = new wchar_t[screenWidth * screenHeight];

    // setting up game
    setupGame();

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
		if (GetAsyncKeyState(VK_ESCAPE) & 0x01)
			PostMessage(GetConsoleWindow(), WM_CLOSE, 0, 0);


        
        if (! alive) {
            if (GetAsyncKeyState((unsigned short)' ') & 0x8000) {
                setupGame();
            }
            continue;
        }

        // every <speed> seconds
        if (movementDelayCounter < 1000000 * speed) {
			movementDelayCounter += timeDelta;
            continue;
        }
		movementDelayCounter = 0;
		// move head according to direction
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

		// wall handling
		if (headX >= screenWidth)
			headX = 0;
		if (headX < 0)
			headX = screenWidth- 1;
		if (headY >= screenHeight)
			headY = 0;
		if (headY < 1)
			headY = screenHeight- 1;

		// handle track removal
		point p = snake->front();
		screen[calculateScreenPosition(p.x, p.y)] = ' ';
		snake->push({ headX, headY });

		// apple handling
		if (screen[calculateScreenPosition(headX, headY)] == 'a')
			spawnApple();
		else 
			snake->pop();

		// draw score
		wsprintf(&screen[1], L"snake x:%d y:%d", headX, headY);

		// death handling
		if (screen[calculateScreenPosition(headX, headY)] == 's') {
			for (int i = 0; i < screenWidth; i++) screen[i] = ' ';
			wsprintf(&screen[1], L"GAME OVER! Press SPACE to restart.");
			alive = false;
		}

		// draw head
		screen[calculateScreenPosition(headX, headY)] = 's';

		// render screen to console 
		screen[screenWidth * screenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(console, screen, screenWidth * screenHeight, { 0,0 }, &bytesWritten);
    }
}

int calculateScreenPosition(int x, int y) {
    return y * screenWidth + x;
}

void spawnApple() {
    int randX;
    int randY;

    randX = rand() % screenWidth;
    randY = (rand() % (screenHeight - 1)) + 1; // not 0
    screen[calculateScreenPosition(randX, randY)] = 'a';
}

void setupGame() {
    headX = 12;
    headY = 12;
	for (int i = 0; i < screenWidth * screenHeight; i++)
		screen[i] = ' ';
    spawnApple();
    snake = new queue<point>();
    snake->push({ headX, headY });
    alive = true;
    lvl = 0;
    playerDirection = UP;
}
