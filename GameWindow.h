#ifndef GAMEWINDOW_INCLUDE_H
#define GAMEWINDOW_INCLUDE_H

#include <iostream>
#include <string>
#include <SDL.h>

using namespace std;

class GameWindow
{
public:
	GameWindow();
	~GameWindow();

	bool CreateWindow(string title, int windowWidth, int windowHeight);
	void Run(void* Start = NULL, void* Update = NULL);
private:
	bool quit;
	SDL_Event e;

	SDL_Window* window;
	SDL_Renderer* renderer;

	void (*Start)();
	void (*Update)();

	void Close();
};

#endif