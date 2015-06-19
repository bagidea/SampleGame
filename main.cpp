#include "GameWindow.h"
#include "GameObject.h"

GameWindow* bis;
GameObject* mc;

SDL_Rect Clip[8];

void Start()
{
	Clip[0].x = 0;
	Clip[0].y = 0;
	Clip[0].w = 80;
	Clip[0].h = 120;

	Clip[1].x = 80;
	Clip[1].y = 0;
	Clip[1].w = 80;
	Clip[1].h = 120;

	Clip[2].x = 160;
	Clip[2].y = 0;
	Clip[2].w = 80;
	Clip[2].h = 120;

	Clip[3].x = 240;
	Clip[3].y = 0;
	Clip[3].w = 80;
	Clip[3].h = 120;

	Clip[4].x = 320;
	Clip[4].y = 0;
	Clip[4].w = 80;
	Clip[4].h = 120;

	Clip[5].x = 400;
	Clip[5].y = 0;
	Clip[5].w = 80;
	Clip[5].h = 120;

	Clip[6].x = 480;
	Clip[6].y = 0;
	Clip[6].w = 80;
	Clip[6].h = 120;

	Clip[7].x = 560;
	Clip[7].y = 0;
	Clip[7].w = 80;
	Clip[7].h = 120;

	mc = new GameObject(bis->GetRenderer());
	mc->Load("source/SPplayer.png");

	mc->AddClip(Clip);
	mc->width = 80;
	mc->height = 120;
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