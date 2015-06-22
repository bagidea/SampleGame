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
int mX, mY;
bool Kup, Kdown, Kleft, Kright;

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
bool Collision(GameObject* a, GameObject* b);

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
	if(mc->y <= 0 || mc->y >= screenHeight-mc->height || Collision(mc, wall))
	{
		mc->y -= mY;
	}

	mc->x += mX;
	if(mc->x <= 0 || mc->x >= screenWidth-mc->width || Collision(mc, wall))
	{
		mc->x -= mX;
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

bool Collision(GameObject* a, GameObject* b)
{
	int upA, upB, downA, downB, leftA, leftB, rightA, rightB;

	upA = a->y;
	upB = b->y;
	downA = a->y+a->height;
	downB = b->y+b->height;

	leftA = a->x;
	leftB = b->x;
	rightA = a->x+a->width;
	rightB = b->x+b->width;

	if(upA >= downB)
	{
		return false;
	}

	if(downA <= upB)
	{
		return false;
	}

	if(leftA >= rightB)
	{
		return false;
	}

	if(rightA <= leftB)
	{
		return false;
	}

	return true;
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