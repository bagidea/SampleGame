#include "GameWindow.h"
#include "GameObject.h"

int screenWidth = 800;
int screenHeight = 600;

GameWindow* bis;
GameObject* bg;
GameObject* mc;
GameObject* wall;

SDL_Rect Clip[8];

int speed;
bool Kup, Kdown, Kleft, Kright;

void Start()
{
	speed = 5;
	Kup = false;
	Kdown = false;
	Kleft = false;
	Kright = false;

	bg = new GameObject(bis->GetRenderer());
	bg->Load("source/Floor.jpg");
	bg->width = 800;
	bg->height = 600;

	mc = new GameObject(bis->GetRenderer());
	mc->Load("source/SPplayer.png");

	mc->LoadClip("source/SPplayer.animate");

	mc->x = 10;
	mc->y = 10;
	mc->width = 80;
	mc->height = 120;

	mc->SetAnimation(0, 0);
	mc->Play();

	wall = new GameObject(bis->GetRenderer());
	wall->Load("source/Box.png");
	wall->x = 300;
	wall->y = 200;
	wall->width = 200;
	wall->height = 200;
}

void Event();

void Update()
{
	if(Kup)
	{
		mc->y -= speed;
	}
	else if(Kdown)
	{
		mc->y += speed;
	}

	if(Kleft)
	{
		mc->x -= speed;
		mc->SetFlip(SDL_FLIP_HORIZONTAL);
	}
	else if(Kright)
	{
		mc->x += speed;
		mc->SetFlip(SDL_FLIP_NONE);
	}

	if(!Kup && !Kdown && !Kleft && !Kright)
	{
		mc->SetAnimation(0, 0);
	}else{
		mc->SetAnimation(1, 7);
	}

	if(mc->y <= 0)
	{
		mc->y = 0;	
	}
	else if(mc->y >= screenHeight-mc->height)
	{
		mc->y = screenHeight-mc->height;
	}

	if(mc->x <= 0)
	{
		mc->x = 0;	
	}
	else if(mc->x >= screenWidth-mc->width)
	{
		mc->x = screenWidth-mc->width;
	}

	bg->Render();
	wall->Render();
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