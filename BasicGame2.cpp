#include <cstdlib>
#include <ctime>

#include "GameWindow.h"
#include "GameObject.h"

class Effect
{
public:
	bool end;
	int x;
	int y;

	Effect(SDL_Renderer* renderer);
	~Effect();

	int GetWidth();
	int GetHeight();

	void Render();
	bool IsPlay();
private:
	GameObject* gameObject;
};

Effect::Effect(SDL_Renderer* renderer)
{	
	end = false;
	x = 0;
	y = 0;

	gameObject = new GameObject(renderer);
	gameObject->Load("source/Effect.png");
	gameObject->LoadClip("source/Effect.animate");
	gameObject->SetLoop(false);
	gameObject->width = 300;
	gameObject->height = 300;
	gameObject->Play();
}

Effect::~Effect()
{
	delete gameObject;
	gameObject = NULL;
}

void Effect::Render()
{
	gameObject->x = x;
	gameObject->y = y;
	gameObject->Render();
	if(!gameObject->IsPlay())
	{
		end = true;
	}
}

bool Effect::IsPlay(){return gameObject->IsPlay();}
int Effect::GetWidth(){return gameObject->width;}
int Effect::GetHeight(){return gameObject->height;}

int screenWidth = 800;
int screenHeight = 600;

GameWindow* bis;
GameObject* bg;

GameObject* player1;
GameObject* player2;
GameObject* ball;

bool chk;
bool start;

int speed;
int mX, mY;
bool Kleft, Kright;

time_t tmr;

vector<Effect*> effList;

void AddEffect(GameObject* ob)
{
	Effect* eff = new Effect(bis->GetRenderer());
	eff->x = (ob->x+(ob->width/2))-(eff->GetWidth()/2);
	eff->y = (ob->y+(ob->width/2))-(eff->GetHeight()/2);
	effList.push_back(eff);
}

void Start()
{
	srand(time(NULL));

	speed = 10;
	mX = 0;
	mY = 0;
	Kleft = false;
	Kright = false;

	tmr = time(0);
	start = false;

	chk = false;

	bg = new GameObject(bis->GetRenderer());
	bg->Load("source/pongBG.jpg");
	bg->width = 800;
	bg->height = 600;

	player1 = new GameObject(bis->GetRenderer());
	player1->Load("source/pongPlayer.jpg");
	player1->SetFlip(SDL_FLIP_VERTICAL);
	player1->width = 150;
	player1->height = 30;
	player1->x = (screenWidth/2)-(player1->width/2);
	player1->y = screenHeight-20-player1->height;

	player2 = new GameObject(bis->GetRenderer());
	player2->Load("source/pongPlayer.jpg");
	player2->width = 150;
	player2->height = 30;
	player2->x = (screenWidth/2)-(player2->width/2);
	player2->y = 20;

	ball = new GameObject(bis->GetRenderer());
	ball->Load("source/Ball.png");
	ball->width = 50;
	ball->height = 50;
	ball->x = (screenWidth/2)-(ball->width/2);
	ball->y = (screenHeight/2)-(ball->height/2);
	ball->SetCenter(ball->width/2, ball->width/2);
}

void Update()
{
	if(Kleft)
	{
		player1->x -= speed;
	}
	else if(Kright)
	{
		player1->x += speed;
	}

	if(player1->x <= 0)
	{
		player1->x = 0;
	}
	else if(player1->x >= screenWidth-player1->width)
	{
		player1->x = screenWidth-player1->width;
	}

	if(player2->x > ball->x-(ball->width/2) || player2->x+player2->width < ball->x+(ball->width/2))
	{
		if(player2->x+(player2->width/2) > ball->x+(ball->width/2))
		{
			player2->x -= speed;
		}
		else if(player2->x+(player2->width/2) < ball->x+(ball->width/2))
		{
			player2->x += speed;
		}
	}

	if(player2->x <= 0)
	{
		player2->x = 0;
	}
	else if(player2->x >= screenWidth-player2->width)
	{
		player2->x = screenWidth-player2->width;
	}

	ball->rotation += mX*2;
	ball->x += mX;
	ball->y += mY;

	if(ball->x <= 0 || ball->x >= screenWidth-ball->width)
	{
		mX = -mX;
		if(ball->x <= 0)
			ball->x = 1;
		else if(ball->x >= screenWidth-ball->width)
			ball->x = screenWidth-ball->width-1;

		AddEffect(ball);
	}

	if((chk && ball->HitTest(player1)) || (!chk && ball->HitTest(player2)))
	{
		if(chk)
		{
			mX = ((ball->x+(ball->width/2))-(player1->x+(player1->width/2)))/8;
			chk = false;
		}else{
			mX = ((ball->x+(ball->width/2))-(player2->x+(player2->width/2)))/8;
			chk = true;
		}

		if(mY > 0)
			mY++;
		else if(mY < 0)
			mY--;

		mY = -mY;	
		AddEffect(ball);
	}

	if(!start)
	{
		if(time(0)-tmr >= 2)
		{
			int ranY = rand() % 2;
			int ranX = (rand() % (speed*2)+1)-speed;

			if(ranY == 0)
			{
				chk = true;
				mY = speed/2;
			}else{
				chk = false;
				mY = -speed/2;
			}

			mX = ranX;
			start = true;
		}
	}else{
		if(ball->y < -ball->width || ball->y > screenWidth+ball->width)
		{
			start = false;
			tmr = time(0);
			mX = 0;
			mY = 0;
			ball->x = (screenWidth/2)-(ball->width/2);
			ball->y = (screenHeight/2)-(ball->height/2);

			player1->x = (screenWidth/2)-(player1->width/2);
			player1->y = screenHeight-20-player1->height;
			player2->x = (screenWidth/2)-(player2->width/2);
			player2->y = 20;
		}
	}

	bg->Render();
	player1->Render();
	player2->Render();
	ball->Render();

	for(int i = 0; i < effList.size(); i++)
	{
		if(effList[i]->IsPlay())
		{
			effList[i]->Render();
		}else{
			delete effList[i];
			effList[i] = NULL;
			effList.erase(effList.begin()+i);
		}
	}
}

void Event()
{
	switch(bis->GetEvent().type)
	{
	case SDL_KEYDOWN:
		if(bis->GetEvent().key.keysym.sym == SDLK_LEFT)
		{
			Kleft = true;
		}
		else if(bis->GetEvent().key.keysym.sym == SDLK_RIGHT)
		{
			Kright = true;
		}
		break;
	case SDL_KEYUP:
		if(bis->GetEvent().key.keysym.sym == SDLK_LEFT)
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
	for(int i = 0; i < effList.size(); i++)
	{
		delete effList[i];
		effList[i] = NULL;
	}

	effList.clear();

	delete bis;
	bis = NULL;
}

int main(int argc, char* argv[])
{
	bis = new GameWindow();

	if(!bis->CreateWindow("Sample Game Pong 2D", screenWidth, screenHeight))
	{
		cerr << "Can't Create GameWindow" << endl;
	}else{
		bis->Run((void*)&Start, (void*)&Update, (void*)&Event);
	}

	Close();

	return 0;
}