#include "GameWindow.h"
#include "GameObject.h"

#include <vector>

int screenWidth = 800;
int screenHeight = 600;

GameWindow* bis;
GameObject* bg;
GameObject* wall;
GameObject* mc;

SDL_Rect Clip[8];

int speed;
int mX, mY;
bool Kup, Kdown, Kleft, Kright;

int Map[6][8] = {
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 0, 0, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 1, 1, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1
};

struct position
{
	int x, y;
	position(int _x, int _y):x(_x), y(_y){}
};
vector<position*> tile;

void GenMap()
{
	int i, a;

	for(i = 0; i < 6; i++)
	{
		for(a = 0; a < 8; a++)
		{
			if(Map[i][a] == 1)
			{
				tile.push_back(new position(a*100, i*100));
			}
		}
	}
}

void Start()
{
	mX = 0;
	mY = 0;

	speed = 5;
	Kup = false;
	Kdown = false;
	Kleft = false;
	Kright = false;

	bg = new GameObject(bis->GetRenderer());
	bg->Load("source/BG.jpg");
	bg->width = 800;
	bg->height = 600;

	wall = new GameObject(bis->GetRenderer());
	wall->Load("source/Wall.jpg");
	wall->width = 100;
	wall->height = 100;

	GenMap();

	mc = new GameObject(bis->GetRenderer());
	mc->Load("source/SPplayer.png");

	mc->LoadClip("source/SPplayer.animate");

	mc->x = 150;
	mc->y = 420;
	mc->width = 50;
	mc->height = 80;

	mc->SetTimeScale(0.8);
	mc->SetAnimation(0, 0);
	mc->Play();
}

void Event();

void Update()
{
	if(Kup)
	{
		mY = -speed;
	}
	else if(Kdown)
	{
		mY = speed;
	}
	else if(!Kup && !Kdown)
	{
		mY = 0;
	}

	if(Kleft)
	{
		mX = -speed;
		mc->SetFlip(SDL_FLIP_HORIZONTAL);
	}
	else if(Kright)
	{
		mX = speed;
		mc->SetFlip(SDL_FLIP_NONE);
	}
	else if(!Kleft && !Kright)
	{
		mX = 0;
	}

	if(!Kup && !Kdown && !Kleft && !Kright)
	{
		mc->SetAnimation(0, 0);
	}else{
		mc->SetAnimation(1, 7);
	}

	mc->y += mY;
	if(mc->y <= 0 || mc->y >= screenHeight-mc->height)
	{
		mc->y -= mY;
	}

	mc->x += mX;
	if(mc->x <= 0 || mc->x >= screenWidth-mc->width)
	{
		mc->x -= mX;
	}

	bg->Render();

	for(int i = 0; i < tile.size(); i++)
	{
		wall->x = tile[i]->x;
		wall->y = tile[i]->y;
		wall->Render();
	}

	mc->Render();
}

void Event()
{
	switch(bis->GetEvent().type)
	{
	case SDL_KEYDOWN:
		if(bis->GetEvent().key.keysym.sym == SDLK_UP)
		{
			Kup = true;
		}
		else if(bis->GetEvent().key.keysym.sym == SDLK_DOWN)
		{
			Kdown = true;
		}
		else if(bis->GetEvent().key.keysym.sym == SDLK_LEFT)
		{
			Kleft = true;
		}
		else if(bis->GetEvent().key.keysym.sym == SDLK_RIGHT)
		{
			Kright = true;
		}
		break;
	case SDL_KEYUP:
		if(bis->GetEvent().key.keysym.sym == SDLK_UP)
		{
			Kup = false;
		}
		else if(bis->GetEvent().key.keysym.sym == SDLK_DOWN)
		{
			Kdown = false;
		}
		else if(bis->GetEvent().key.keysym.sym == SDLK_LEFT)
		{
			Kleft = false;
		}
		else if(bis->GetEvent().key.keysym.sym == SDLK_RIGHT)
		{
			Kright = false;
		}
		break;
	}
}

void Close()
{
	delete bg;
	bg = NULL;

	for(int i = 0; i < tile.size(); i++)
	{
		delete tile[i];
		tile[i] = NULL;
	}
	tile.clear();

	delete mc;
	mc = NULL;

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