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

int gravity;
int speed;
bool jump;
int mX, mY;
bool kspace, Kleft, Kright;

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

	gravity = 5;
	speed = 5;
	jump = false;
	kspace = false;
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

bool CheckCollision(GameObject* ob, bool chk = false)
{
	int topLeft = Map[ob->y/100][ob->x/100];
	int topRight = Map[ob->y/100][(ob->x+ob->width-1)/100];
	int bottomLeft = Map[(ob->y+ob->height-1)/100][ob->x/100];
	int bottomRight = Map[(ob->y+ob->height-1)/100][(ob->x+ob->width-1)/100];

	if(topLeft == 1 || topRight == 1 || bottomLeft == 1 || bottomRight == 1)
	{
		if(chk)
		{
			if(topLeft == 1 || topRight == 1)
			{
				ob->y = ((ob->y/100)*100)+100;
				mY = 0;
			}
			
			if(bottomLeft == 1 || bottomRight == 1)
			{
				mY = 0;
				ob->y = (((ob->y+ob->height-1)/100)*100)-ob->height;
				jump = false;
			}
		}
		return true;
	}

	return false;
}

void Update()
{
	mY += gravity;

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

	if(!jump)
	{
		if(!Kleft && !Kright)
		{
			mc->SetAnimation(0, 0);
		}else{
			mc->SetAnimation(1, 7);
		}
	}else{
		mc->SetAnimation(2, 2);
	}

	mc->y += mY;
	if(mc->y <= 0 || mc->y >= screenHeight-mc->height || CheckCollision(mc, true))
	{
		mc->y -= mY;
	}

	mc->x += mX;
	if(mc->x <= 0 || mc->x >= screenWidth-mc->width || CheckCollision(mc))
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
		if(bis->GetEvent().key.keysym.sym == SDLK_SPACE)
		{
			if(!kspace && !jump)
			{
				mY = -50;
				jump = true;
				kspace = true;
			}
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
		if(bis->GetEvent().key.keysym.sym == SDLK_SPACE)
		{
			kspace = false;
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