#ifndef GAMEWINDOW_INCLUDE_H
#define GAMEWINDOW_INCLUDE_H

#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

using namespace std;

//GameWindow Class
class GameWindow
{
public:
	GameWindow();
	~GameWindow();

	bool CreateWindow(string title, int windowWidth, int windowHeight);
	void Run(void* Start = NULL, void* Update = NULL, void* Event = NULL);

	void Quit();

	SDL_Renderer* GetRenderer();
	SDL_Event GetEvent();
private:
	bool quit;
	SDL_Event e;

	SDL_Window* window;
	SDL_Renderer* renderer;

	void (*Start)();
	void (*Update)();
	void (*Event)();

	void Close();
};

#endif