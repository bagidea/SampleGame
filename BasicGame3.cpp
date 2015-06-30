#include <cstdlib>
#include <ctime>

#include "GameWindow.h"
#include "GameObject.h"
#include "GameAudio.h"

class FireBall
{
public:
	int x, y, speed;
	bool isDead;

	FireBall(SDL_Renderer* renderer, GameSurface* surface);
	~FireBall();

	void Render();
	int GetWidth();

	bool HitTest(GameObject* ob);
private:
	GameObject* gameObject;
};

FireBall::FireBall(SDL_Renderer* renderer, GameSurface* surface)
{
	x = 0;
	y = 0;
	speed = 0;
	isDead = false;

	gameObject = new GameObject(renderer);
	gameObject->CreateGameTextureFromGameSurface(surface);
	gameObject->width = 50;
	gameObject->height = 50;
}

FireBall::~FireBall()
{
	delete gameObject;
	gameObject = NULL;
}

void FireBall::Render()
{
	gameObject->x = x;
	y += speed;
	gameObject->y = y;

	gameObject->Render();

	if(y < -50 || y > 650)
	{
		isDead = true;
	}
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
	bool left, isDead, isShoot;

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
	if(left)
		x -= speed;
	else
		x += speed;

	y++;

	gameObject->x = x;
	gameObject->y = y;

	if(time(0)-tmr >= 2)
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

GameObject* mc;

GameSurface* bgSurface;
GameObject* bg1;
GameObject* bg2;

GameSurface* fireSurface;
vector<FireBall*> fireBall;

int tmr;
GameSurface* enemySurface;
vector<Enemy*> enemy;

void Start()
{
	srand(time(NULL));
	tmr = time(0);

	mL = false;
	mR = false;
	velocity = 0;
	maxVelocity = 10;
	speed = 2;

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

	enemySurface = new GameSurface();
	enemySurface->Load("source/ShipEnemy.png");

	fireSurface = new GameSurface();
	fireSurface->Load("source/FireBall.png");
}

void Update()
{
	int i, a;

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

	bg1->Render();
	bg2->Render();

	mc->Render();

	if(time(0)-tmr >= 3)
	{
		Enemy* en = new Enemy(bis->GetRenderer(), enemySurface);
		en->x = rand() % (screenWidth-en->GetWidth());
		en->y = -en->GetHeight();
		enemy.push_back(en);

		tmr = time(0);
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

		if(enemy[i]->isShoot)
		{
			FireBall* fire = new FireBall(bis->GetRenderer(), fireSurface);
			fire->x = (enemy[i]->x+(enemy[i]->GetWidth()/2))-(fire->GetWidth()/2);
			fire->y = enemy[i]->y;
			fire->speed = 10;
			fireBall.push_back(fire);

			enemy[i]->isShoot = false;
		}

		if(enemy[i]->y > screenHeight)
		{
			delete enemy[i];
			enemy[i] = NULL;
			enemy.erase(enemy.begin()+i);
		}
	}

	for(i = 0; i < fireBall.size(); i++)
	{
		fireBall[i]->Render();

		for(a = 0; a < enemy.size(); a++)
		{
			if(fireBall[i]->HitTest(enemy[a]->GetGameObject()) && fireBall[i]->speed < 0)
			{
				delete enemy[a];
				enemy[a] = NULL;
				delete fireBall[i];
				fireBall[i] = NULL;

				enemy.erase(enemy.begin()+a);
				fireBall.erase(fireBall.begin()+i);
				break;
			}
		}

		if(fireBall[i]->HitTest(mc) && fireBall[i]->speed > 0)
		{
			delete fireBall[i];
			fireBall[i] = NULL;
			fireBall.erase(fireBall.begin()+i);
			break;
		}
		else if(fireBall[i]->isDead)
		{
			delete fireBall[i];
			fireBall[i] = NULL;
			fireBall.erase(fireBall.begin()+i);
			break;
		}
	}
}

void Event()
{
	switch(bis->GetEvent().type)
	{
	case SDL_KEYDOWN:
		if(bis->GetEvent().key.keysym.sym == SDLK_SPACE)
		{
			FireBall* fire = new FireBall(bis->GetRenderer(), fireSurface);
			fire->x = (mc->x+(mc->width/2))-(fire->GetWidth()/2);
			fire->y = mc->y;
			fire->speed = -10;
			fireBall.push_back(fire);
		}
		else if(bis->GetEvent().key.keysym.sym == SDLK_LEFT)
		{
			mL = true;
		}
		else if(bis->GetEvent().key.keysym.sym == SDLK_RIGHT)
		{
			mR = true;
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
	int i;

	delete mc;
	mc = NULL;

	delete bg1;
	bg1 = NULL;

	delete bg2;
	bg2 = NULL;

	delete bgSurface;
	bgSurface = NULL;

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