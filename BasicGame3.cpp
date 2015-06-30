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

class FireBall
{
public:
	int x, y, speed;
	bool isDead, gameOver;

	FireBall(SDL_Renderer* renderer, GameSurface* surface);
	~FireBall();

	int Render();
	int GetWidth();

	bool HitTest(GameObject* ob);
private:
	GameObject* gameObject;
};

FireBall::FireBall(SDL_Renderer* renderer, GameSurface* surface)
{
	gameOver = false;

	x = 0;
	y = 0;
	speed = 0;
	isDead = false;

	gameObject = new GameObject(renderer);
	gameObject->CreateGameTextureFromGameSurface(surface);
	gameObject->width = 20;
	gameObject->height = 20;
}

FireBall::~FireBall()
{
	delete gameObject;
	gameObject = NULL;
}

int FireBall::Render()
{
	if(!gameOver)
	{
		gameObject->x = x;
		y += speed;
		gameObject->y = y;
	}

	gameObject->Render();

	if(y < -GetWidth() || y > 600+GetWidth())
	{
		isDead = true;
		if(y < -GetWidth())
			return 1;
	}

	return 0;
}

int FireBall::GetWidth(){return gameObject->width;}

bool FireBall::HitTest(GameObject* ob)
{
	gameObject->HitTest(ob);
}

class Enemy
{
public:
	int x, y, speed;
	bool left, isDead, isShoot, gameOver;

	Enemy(SDL_Renderer* renderer, GameSurface* surface);
	~Enemy();

	void Render();
	int GetWidth();
	int GetHeight();

	GameObject* GetGameObject();
private:
	int tmr;
	GameObject* gameObject;
};

Enemy::Enemy(SDL_Renderer* renderer, GameSurface* surface)
{
	gameOver = false;

	x = 0;
	y = 0;

	speed = rand() % 10 + 1;
	left = rand() % 2;
	isDead = false;
	isShoot = false;

	tmr = time(0);

	gameObject = new GameObject(renderer);
	gameObject->CreateGameTextureFromGameSurface(surface);
	gameObject->width = 100;
	gameObject->height = 75;
}

Enemy::~Enemy()
{
	delete gameObject;
	gameObject = NULL;
}

void Enemy::Render()
{
	if(!gameOver)
	{
		if(left)
			x -= speed;
		else
			x += speed;

		y++;
	}

	gameObject->x = x;
	gameObject->y = y;

	if(time(0)-tmr >= 1)
	{
		isShoot = true;
		tmr = time(0);
	}

	gameObject->Render();
}

int Enemy::GetWidth(){return gameObject->width;}
int Enemy::GetHeight(){return gameObject->height;}
GameObject* Enemy::GetGameObject(){return gameObject;}

int screenWidth = 800;
int screenHeight = 600;

GameWindow* bis;

bool mL, mR;
int velocity;
int maxVelocity;
int speed;

GameObject* heart;
int HP;

GameObject* mc;

GameSurface* bgSurface;
GameObject* bg1;
GameObject* bg2;

GameObject* ammunition;
int playerShoot;

GameSurface* fireSurface;
vector<FireBall*> fireBall;

int tmr;
GameSurface* enemySurface;
vector<Enemy*> enemy;

GameSurface* effectSurface;
vector<Effect*> effList;

bool gameOver;
GameObject* gameOverScreen;

AudioBackground* music;
AudioClip* soundDamage;
AudioClip* soundShoot;
AudioClip* soundShoot2;

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
	tmr = time(0);
	gameOver = false;
	HP = 3;

	mL = false;
	mR = false;
	velocity = 0;
	maxVelocity = 10;
	speed = 2;

	playerShoot = 3;

	mc = new GameObject(bis->GetRenderer());
	mc->Load("source/Jet.png");
	mc->width = 100;
	mc->height = 75;
	mc->x = (screenWidth/2)-(mc->width/2);
	mc->y = (screenHeight-mc->height)-50;

	bgSurface = new GameSurface();
	bgSurface->Load("source/BGspace.png");

	bg1 = new GameObject(bis->GetRenderer());
	bg1->CreateGameTextureFromGameSurface(bgSurface);
	bg1->width = screenWidth;
	bg1->height = screenHeight;

	bg2 = new GameObject(bis->GetRenderer());
	bg2->CreateGameTextureFromGameSurface(bgSurface);
	bg2->width = screenWidth;
	bg2->height = screenHeight;
	bg2->y = -screenHeight;

	heart = new GameObject(bis->GetRenderer());
	heart->Load("source/Heart.png");
	heart->width = 45;
	heart->height = 40;

	ammunition = new GameObject(bis->GetRenderer());
	ammunition->Load("source/Ammunition.png");
	ammunition->width = 45;
	ammunition->height = 40;

	enemySurface = new GameSurface();
	enemySurface->Load("source/ShipEnemy.png");

	fireSurface = new GameSurface();
	fireSurface->Load("source/FireBall.png");

	effectSurface = new GameSurface();
	effectSurface->Load("source/Effect.png");

	gameOverScreen = new GameObject(bis->GetRenderer());
	gameOverScreen->Load("source/GameOver2.png");
	gameOverScreen->x = 150;
	gameOverScreen->y = 100;
	gameOverScreen->width = 500;
	gameOverScreen->height = 400;

	soundDamage = new AudioClip();
	soundDamage->Load("source/sounds/Damage.wav");

	soundShoot = new AudioClip();
	soundShoot->Load("source/sounds/sf.wav");

	soundShoot2 = new AudioClip();
	soundShoot2->Load("source/sounds/StartGame.wav");

	music = new AudioBackground();
	music->Load("source/sounds/Music2.wav");
	music->Play();
}

void Start()
{
	srand(time(NULL));
	
	GameSetup();
}

void Update()
{
	int i, a;

	if(!gameOver)
	{
		if(mL)
		{
			velocity -= speed;
			if(velocity <= -maxVelocity)
				velocity = -maxVelocity;
		}
		else if(mR)
		{
			velocity += speed;
			if(velocity >= maxVelocity)
				velocity = maxVelocity;
		}else{
			if(velocity > 0)
				velocity -= speed/2;
			else if(velocity < 0)
				velocity += speed/2;
		}

		mc->x += velocity;

		if(mc->x <= 0)
		{
			mc->x = 0;
			velocity = 0;
		}
		else if(mc->x >= screenWidth-mc->width)
		{
			mc->x = screenWidth-mc->width;
			velocity = 0;
		}

		bg1->y += 5;
		bg2->y += 5;

		if(bg1->y > screenHeight)
			bg1->y = -screenHeight;

		if(bg2->y > screenHeight)
			bg2->y = -screenHeight;
	}

	bg1->Render();
	bg2->Render();

	mc->Render();

	if(!gameOver)
	{
		if(time(0)-tmr >= 3)
		{
			Enemy* en = new Enemy(bis->GetRenderer(), enemySurface);
			en->x = rand() % (screenWidth-en->GetWidth());
			en->y = -en->GetHeight();
			enemy.push_back(en);

			tmr = time(0);
		}
	}

	for(i = 0; i < enemy.size(); i++)
	{
		if(!enemy[i]->left && enemy[i]->x >= screenWidth-enemy[i]->GetWidth())
		{
			enemy[i]->left = true;
		}
		else if(enemy[i]->left && enemy[i]->x <= 0)
		{
			enemy[i]->left = false;
		}

		enemy[i]->Render();

		if(!gameOver)
		{
			if(enemy[i]->isShoot)
			{
				FireBall* fire = new FireBall(bis->GetRenderer(), fireSurface);
				fire->x = (enemy[i]->x+(enemy[i]->GetWidth()/2))-(fire->GetWidth()/2);
				fire->y = enemy[i]->y;
				fire->speed = 10;
				fireBall.push_back(fire);

				enemy[i]->isShoot = false;

				soundShoot2->Play();
			}

			if(enemy[i]->y > screenHeight)
			{
				delete enemy[i];
				enemy[i] = NULL;
				enemy.erase(enemy.begin()+i);
			}
		}
	}

	for(i = 0; i < fireBall.size(); i++)
	{
		playerShoot += fireBall[i]->Render();

		if(!gameOver)
		{
			if(fireBall[i]->HitTest(mc) && fireBall[i]->speed > 0)
			{
				AddEffect(mc);

				HP--;
				if(HP <= 0)
				{
					for(a = 0; a < fireBall.size(); a++)
						fireBall[a]->gameOver = true;
					for(a = 0; a < enemy.size(); a++)
						enemy[a]->gameOver = true;
					gameOver = true;
				}

				delete fireBall[i];
				fireBall[i] = NULL;
				fireBall.erase(fireBall.begin()+i);
			}
			else if(fireBall[i]->isDead)
			{
				delete fireBall[i];
				fireBall[i] = NULL;
				fireBall.erase(fireBall.begin()+i);
			}else{
				for(a = 0; a < enemy.size(); a++)
				{
					if(fireBall[i]->HitTest(enemy[a]->GetGameObject()) && fireBall[i]->speed < 0)
					{
						playerShoot++;
						AddEffect(enemy[a]->GetGameObject());

						delete enemy[a];
						enemy[a] = NULL;
						delete fireBall[i];
						fireBall[i] = NULL;

						enemy.erase(enemy.begin()+a);
						fireBall.erase(fireBall.begin()+i);
						break;
					}
				}
			}
		}
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

	for(i = 0; i < HP; i++)
	{
		heart->y = screenHeight-heart->height-2;
		heart->x = 10+(i*heart->width);
		heart->Render();
	}

	for(i = 0; i < playerShoot; i++)
	{
		ammunition->y = screenHeight-ammunition->height-2;
		ammunition->x = ((screenWidth-ammunition->width)-10)-(i*ammunition->width);
		ammunition->Render();
	}

	if(gameOver)
		gameOverScreen->Render();
}

void GameClear()
{
	int i;

	delete mc;
	mc = NULL;

	delete bg1;
	bg1 = NULL;

	delete bg2;
	bg2 = NULL;

	delete bgSurface;
	bgSurface = NULL;

	delete heart;
	heart = NULL;

	delete ammunition;
	ammunition = NULL;

	for(i = 0; i < enemy.size(); i++)
	{
		delete enemy[i];
		enemy[i] = NULL;
	}

	enemy.clear();

	for(i = 0; i < fireBall.size(); i++)
	{
		delete fireBall[i];
		fireBall[i] = NULL;
	}

	fireBall.clear();

	delete enemySurface;
	enemySurface = NULL;

	delete fireSurface;
	fireSurface = NULL;

	for(i = 0; i < effList.size(); i++)
	{
		delete effList[i];
		effList[i] = NULL;
	}

	effList.clear();

	delete effectSurface;
	effectSurface = NULL;

	delete gameOverScreen;
	gameOverScreen = NULL;

	music->Stop();
	delete music;
	music = NULL;

	delete soundDamage;
	soundDamage = NULL;

	delete soundShoot;
	soundShoot = NULL;

	delete soundShoot2;
	soundShoot2 = NULL;
}

void Event()
{
	switch(bis->GetEvent().type)
	{
	case SDL_KEYDOWN:
		if(bis->GetEvent().key.keysym.sym == SDLK_SPACE)
		{
			if(!gameOver)
			{
				if(playerShoot > 0)
				{
					FireBall* fire = new FireBall(bis->GetRenderer(), fireSurface);
					fire->x = (mc->x+(mc->width/2))-(fire->GetWidth()/2);
					fire->y = mc->y;
					fire->speed = -10;
					fireBall.push_back(fire);
					playerShoot--;

					soundShoot->Play();
				}
			}else{
				GameClear();
				GameSetup();
			}
		}
		else if(bis->GetEvent().key.keysym.sym == SDLK_LEFT)
		{
			mL = true;
		}
		else if(bis->GetEvent().key.keysym.sym == SDLK_RIGHT)
		{
			mR = true;
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
			mL = false;
		}
		else if(bis->GetEvent().key.keysym.sym == SDLK_RIGHT)
		{
			mR = false;
		}
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

	if(!bis->CreateWindow("Sample Game Shooting", screenWidth, screenHeight))
	{
		cerr << "Can't Create GameWindow" << endl;
	}else{
		bis->Run((void*)&Start, (void*)&Update, (void*)&Event);
	}

	Close();

	return 0;
}