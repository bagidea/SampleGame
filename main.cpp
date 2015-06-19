#include "GameWindow.h"
#include "GameObject.h"

GameWindow* bis;
GameObject* mc;

SDL_Rect Clip[8];

void Start()
{
	mc = new GameObject(bis->GetRenderer());
	mc->Load("source/SPplayer.png");

	mc->LoadClip("source/SPplayer.animate");
	mc->width = 80;
	mc->height = 120;

	mc->Play();
}

void Update()
{
	mc->Render();
}

void Close()
{
	delete mc;
	mc = NULL;

	delete bis;
	bis = NULL;
}

int main(int argc, char* argv[])
{
	bis = new GameWindow();

	if(!bis->CreateWindow("Sample Game", 800, 600))
	{
		cerr << "Can't Create GameWindow" << endl;
	}else{
		bis->Run((void*)&Start, (void*)Update);
	}

	Close();

	return 0;
}