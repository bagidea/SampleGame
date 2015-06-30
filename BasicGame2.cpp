#include <cstdlib>
#include <ctime>

#include "GameWindow.h"
#include "GameObject.h"
#include "GameAudio.h"

class Effect
{
public:
	bool end;
	int x;
	int y;

	Effect(SDL_Renderer* renderer, GameSurface* gameSurface);
	~Effect();

	int GetWidth();
	int GetHeight();

	void Render();
	bool IsPlay();
private:
	GameObject* gameObject;
};

Effect::Effect(SDL_Renderer* renderer, GameSurface* gameSurface)
{	
	end = false;
	x = 0;
	y = 0;

	gameObject = new GameObject(renderer);
	gameObject->CreateGameTextureFromGameSurface(gameSurface);
	gameObject->GenerateClip(5, 4);
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

GameSurface* playerSurface;
GameSurface* effectSurface;

GameObject* player1;
GameObject* player2;
GameObject* ball;

GameObject* heart;
int player1HP, player2HP;

bool chk;
bool start;

int speed;
int mX, mY;
bool Kleft, Kright;

time_t tmr;

vector<Effect*> effList;

bool gameOver;
GameObject* gameOverScreen;

AudioBackground* music;
AudioClip* soundDamage;
AudioClip* soundKill;

void AddEffect(GameObject* ob)
{
	Effect* eff = new Effect(bis->GetRenderer(), effectSurface);
	eff->x = (ob->x+(ob->width/2))-(eff->GetWidth()/2);
	eff->y = (ob->y+(ob->width/2))-(eff->GetHeight()/2);
	effList.push_back(eff);

	soundDamage->Play();
}

void GameSetup()
{
	gameOver = false;

	player1HP = 3;
	player2HP = 3;

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

	playerSurface = new GameSurface();
	playerSurface->Load("source/pongPlayer.jpg");

	player1 = new GameObject(bis->GetRenderer());
	player1->CreateGameTextureFromGameSurface(playerSurface);
	player1->SetFlip(FLIP_VERTICAL);
	player1->width = 150;
	player1->height = 30;
	player1->x = (screenWidth/2)-(player1->width/2);
	player1->y = screenHeight-20-player1->height;

	player2 = new GameObject(bis->GetRenderer());
	player2->CreateGameTextureFromGameSurface(playerSurface);
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

	effectSurface = new GameSurface();
	effectSurface->Load("source/Effect.png");

	heart = new GameObject(bis->GetRenderer());
	heart->Load("source/Heart.png");
	heart->width = 45;
	heart->height = 40;

	gameOverScreen = new GameObject(bis->GetRenderer());
	gameOverScreen->Load("source/GameOver2.png");
	gameOverScreen->x = 150;
	gameOverScreen->y = 100;
	gameOverScreen->width = 500;
	gameOverScreen->height = 400;

	soundDamage = new AudioClip();
	soundDamage->Load("source/sounds/Damage.wav");

	soundKill = new AudioClip();
	soundKill->Load("source/sounds/StartGame.wav");

	music = new AudioBackground();
	music->Load("source/sounds/Music.wav");
	music->Play();
}

void Start()
{
	srand(time(NULL));
	GameSetup();
}

void Update()
{
	int i;

	if(!gameOver)
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
		if(!gameOver)
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

		}
	}else{
		if(ball->y < -ball->height || ball->y > screenWidth+ball->height)
		{
			if(ball->y > screenWidth+ball->height)
			{
				if(player1HP > 1)
				{
					player1HP--;
				}else{
					player1HP--;
					gameOver = true;
				}
			}
			else if(ball->y < -ball->height)
			{
				if(player2HP > 1)
				{
					player2HP--;
				}else{
					player2HP--;
					gameOver = true;
				}
			}

			soundKill->Play();

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

	for(i = 0; i < player2HP; i++)
	{
		heart->y = 0;
		heart->x = 10+(i*heart->width);
		heart->Render();
	}

	for(i = 0; i < player1HP; i++)
	{
		heart->y = screenHeight-heart->height-2;
		heart->x = ((screenWidth-heart->width)-10)-(i*heart->width);
		heart->Render();
	}

	for(i = 0; i < effList.size(); i++)
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

	if(gameOver)
		gameOverScreen->Render();
}

void GameClear()
{
	for(int i = 0; i < effList.size(); i++)
	{
		delete effList[i];
		effList[i] = NULL;
	}

	effList.clear();

	delete player1;
	player1 = NULL;

	delete player2;
	player2 = NULL;

	delete playerSurface;
	playerSurface = NULL;

	delete bg;
	bg = NULL;

	delete ball;
	ball = NULL;

	delete effectSurface;
	effectSurface = NULL;

	delete heart;
	heart = NULL;

	music->Stop();
	delete music;
	music = NULL;

	delete soundDamage;
	soundDamage = NULL;

	delete soundKill;
	soundKill = NULL;

	delete gameOverScreen;
	gameOverScreen = NULL;
}

void Event()
{
	switch(bis->GetEvent().type)
	{
	case SDL_KEYDOWN:
		if(bis->GetEvent().key.keysym.sym == SDLK_SPACE)
		{
			if(gameOver)
			{
				GameClear();
				GameSetup();
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
		else if(bis->GetEvent().key.keysym.sym == SDLK_RETURN)
		{
			if(gameOver)
				bis->Quit();
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
	GameClear();

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