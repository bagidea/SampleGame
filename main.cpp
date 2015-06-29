#include "GameWindow.h"

int screenWidth = 800;
int screenHeight = 600;

GameWindow* bis;

void Start()
{
}

void Update()
{
}

void Event()
{
}

void Close()
{
	delete bis;
	bis = NULL;
}

int main(int argc, char* argv[])
{
	bis = new GameWindow();

	if(!bis->CreateWindow("Sample Game", screenWidth, screenHeight))
	{
		cerr << "Can't Create GameWindow" << endl;
	}else{
		bis->Run((void*)&Start, (void*)&Update, (void*)&Event);
	}

	Close();

	return 0;
}