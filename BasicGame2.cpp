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

bool Effect::IsPlay()
{
	return gameObject->IsPlay();
}

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
	eff->x = (ob->x+25)-150;
	eff->y = (ob->y+25)-150;
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
	player1->Load("source/pongPlayer.png");
	player1->x = 325;
	player1->y = 20;
	player1->width = 150;
	player1->height = 30;

	player2 = new GameObject(bis->GetRenderer());
	player2->Load("source/pongPlayer.png");
	player2->x = 325;
	player2->y = 550;
	player2->width = 150;
	player2->height = 30;

	ball = new GameObject(bis->GetRenderer());
	ball->Load("source/Ball.png");
	ball->x = 375;
	ball->y = 275;
	ball->width = 50;
	ball->height = 50;
	ball->SetCenter(25, 25);
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
	else if(player1->x >= 650)
	{
		player1->x = 650;
	}

	if(player2->x+75 > ball->x+25)
	{
		player2->x -= speed;
	}
	else if(player2->x+75 < ball->x+25)
	{
		player2->x += speed;
	}

	if(player2->x <= 0)
	{
		player2->x = 0;
	}
	else if(player2->x >= 650)
	{
		player2->x = 650;
	}

	ball->rotation += mX;
	ball->x += mX;
	ball->y += mY;

	if(ball->x <= 0 || ball->x >= 750)
	{
		mX = -mX;
	}

	if((chk && ball->HitTest(player1)) || (!chk && ball->HitTest(player2)))
	{
		if(!chk)
			chk = true;
		else
			chk = false;

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
			int ranX = rand() % 2;
			int ranY = rand() % 2;

			if(ranY == 0)
			{
				chk = false;
				mY = speed/2;
			}else{
				chk = true;
				mY = -speed/2;
			}

			if(ranX == 0)
			{
				mX = speed;
			}else{
				mX = -speed;
			}

			start = true;
		}
	}else{
		if(ball->y < -50 || ball->y > 850)
		{
			start = false;
			tmr = time(0);
			mX = 0;
			mY = 0;
			ball->x = 375;
			ball->y = 275;

			player1->x = 325;
			player1->y = 50;
			player2->x = 325;
			player2->y = 520;
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