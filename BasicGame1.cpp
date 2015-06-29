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

class Monster
{
public:
	bool gameStart;
	bool dead;
	int x, y, width, height;
	int mX, mY;

	Monster(SDL_Renderer* renderer, GameSurface* surface);
	~Monster();

	void Render();
	void Flip(int flip);
	bool HitTest(GameObject* hit);
private:
	GameObject* gameObject;
};

Monster::Monster(SDL_Renderer* renderer, GameSurface* surface)
{
	gameStart = true;

	x = 0;
	y = 0;
	mX = 3;
	mY = -10;
	dead = false;

	gameObject = new GameObject(renderer);
	gameObject->CreateGameTextureFromGameSurface(surface);

	gameObject->GenerateClip(2, 4);
	gameObject->SetTimeScale(0.7);
	gameObject->Play();

	width = 100;
	height = 70;

	gameObject->width = width;
	gameObject->height = height;
}

Monster::~Monster()
{
	delete gameObject;
	gameObject = NULL;
}

void Monster::Render()
{
	if(!gameStart)
		gameObject->Stop();

	gameObject->x = x;
	gameObject->y = y;

	gameObject->Render();

	if(dead)
	{
		mY += 1;
		gameObject->Stop();
		gameObject->SetFlip(FLIP_VERTICAL);
	}
}

void Monster::Flip(int flip)
{
	if(flip == 0)
		gameObject->SetFlip(FLIP_HORIZONTAL);
	else if(flip == 1)
		gameObject->SetFlip(FLIP_NONE);
}

bool Monster::HitTest(GameObject* hit)
{
	return gameObject->HitTest(hit);
}

GameSurface* catSurface;

int screenWidth = 800;
int screenHeight = 600;

GameWindow* bis;
GameObject* bg;
GameObject* wall;
GameObject* gameOver;

GameSurface* effectSurface;
vector<Effect*> effList;

GameObject* mc;

GameSurface* coinSurface;

vector<Monster*> monList;

int gravity;
int speed;
bool jump;
int mX, mY;
bool kspace, Kleft, Kright;

bool startGame;
bool end;

int Map[6][8] = {
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 1, 1, 1, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 1, 1,
	1, 1, 1, 0, 0, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1
};

struct position
{
	int x, y;
	position(int _x, int _y):x(_x), y(_y){}
};

vector<position*> tile;
vector<GameObject*> coin;

AudioBackground* music;
AudioClip* soundDamage;
AudioClip* soundCoin;
AudioClip* soundKill;

void GenMap()
{
	int i, a;

	for(i = 0; i < 6; i++)
	{
		for(a = 0; a < 8; a++)
		{
			if(Map[i][a] == 1)
			{
				tile.push_back(new position(a*wall->width, i*wall->height));
			}
		}
	}
}

void AddCoin(int x, int y)
{
	GameObject* ob = new GameObject(bis->GetRenderer());
	ob->CreateGameTextureFromGameSurface(coinSurface);
	
	//ob->LoadClip("source/Coin.animate");
	ob->GenerateClip(10, 1);

	ob->width = 44;
	ob->height = 40;
	ob->x = (x*wall->width)+(ob->width/2);
	ob->y = (y*wall->height)+(wall->height-ob->height);
	ob->SetTimeScale(0.8);
	ob->SetAnimation(0, 9);
	ob->Play();

	coin.push_back(ob);
}

void AddMonster(int x, int y)
{
	Monster* mon = new Monster(bis->GetRenderer(), catSurface);
	mon->x = x*wall->width;
	mon->y = (y*wall->height)+(wall->height-mon->height);
	monList.push_back(mon);
}

void AddEffect(GameObject* ob)
{
	Effect* eff = new Effect(bis->GetRenderer(), effectSurface);
	eff->x = (ob->x+(ob->width/2))-(eff->GetWidth()/2);
	eff->y = (ob->y+ob->width)-(eff->GetHeight()/2);
	effList.push_back(eff);
}

void GameSetup()
{
	end = false;

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

	coinSurface = new GameSurface();
	coinSurface->Load("source/Coin.png");

	AddCoin(2, 1);
	AddCoin(3, 1);
	AddCoin(4, 1);
	AddCoin(1, 3);
	AddCoin(2, 3);
	AddCoin(3, 4);
	AddCoin(4, 4);
	AddCoin(5, 3);

	mc = new GameObject(bis->GetRenderer());
	mc->Load("source/SPplayer.png");

	//mc->LoadClip("source/SPplayer.animate");
	mc->GenerateClip(8, 1);

	mc->width = 50;
	mc->height = 80;
	mc->x = wall->width*6;
	mc->y = wall->height*2;

	mc->SetFlip(FLIP_HORIZONTAL);
	mc->SetTimeScale(0.8);
	mc->SetAnimation(0, 0);
	mc->Play();

	catSurface = new GameSurface();
	catSurface->Load("source/Cat.png");

	AddMonster(2, 1);
	AddMonster(3, 4);

	gameOver = new GameObject(bis->GetRenderer());
	gameOver->Load("source/GameOver2.png");
	gameOver->x = 150;
	gameOver->y = 100;
	gameOver->width = 500;
	gameOver->height = 400;

	effectSurface = new GameSurface();
	effectSurface->Load("source/Effect.png");

	soundDamage = new AudioClip();
	soundDamage->Load("source/sounds/Damage.wav");

	soundCoin = new AudioClip();
	soundCoin->Load("source/sounds/sf.wav");

	soundKill = new AudioClip();
	soundKill->Load("source/sounds/StartGame.wav");

	music = new AudioBackground();
	music->Load("source/sounds/Music2.wav");
	music->Play();
}

void Start()
{
	GameSetup();
	startGame = true;
}

bool CheckCollision(GameObject* ob, bool chk = false)
{
	int topLeft = Map[ob->y/wall->height][ob->x/wall->width];
	int topRight = Map[ob->y/wall->height][(ob->x+ob->width-1)/wall->width];
	int bottomLeft = Map[(ob->y+ob->height-1)/wall->height][ob->x/wall->width];
	int bottomRight = Map[(ob->y+ob->height-1)/wall->height][(ob->x+ob->width-1)/wall->width];

	if(topLeft == 1 || topRight == 1 || bottomLeft == 1 || bottomRight == 1)
	{
		if(chk)
		{
			if(topLeft == 1 || topRight == 1)
			{
				ob->y = ((ob->y/wall->height)*wall->height)+wall->height;
				mY = 0;
			}
			
			if(bottomLeft == 1 || bottomRight == 1)
			{
				mY = 0;
				ob->y = (((ob->y+ob->height-1)/wall->height)*wall->height)-ob->height;
				jump = false;
			}
		}
		return true;
	}

	return false;
}

void Update()
{
	if(startGame)
	{
		int i;

		mY += gravity;

		if(!end)
		{
			if(Kleft)
			{
				mX = -speed;
				mc->SetFlip(FLIP_HORIZONTAL);
			}
			else if(Kright)
			{
				mX = speed;
				mc->SetFlip(FLIP_NONE);
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
					if(mc->GetStart() != 1 && mc->GetEnd() != 7)
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
		}

		bg->Render();

		for(i = 0; i < tile.size(); i++)
		{
			wall->x = tile[i]->x;
			wall->y = tile[i]->y;
			wall->Render();
		}

		for(i = 0; i < coin.size(); i++)
		{
			if(coin[i]->HitTest(mc))
			{
				delete coin[i];
				coin[i] = NULL;
				coin.erase(coin.begin()+i);

				soundCoin->Play();
			}else{
				if(end)
					coin[i]->Stop();

				coin[i]->Render();
			}
		}

		for(i = 0; i < monList.size(); i++)
		{
			monList[i]->gameStart = !end;

			if(!end && !monList[i]->dead)
			{
				monList[i]->x += monList[i]->mX;
			}
			else if(!end && monList[i]->dead)
			{
				monList[i]->y += monList[i]->mY;
			}

			if(!monList[i]->dead)
			{
				if(monList[i]->mX > 0)
				{	
					if((Map[(monList[i]->y+monList[i]->height-1)/wall->height][(monList[i]->x+monList[i]->width-1)/wall->width] == 0 && Map[((monList[i]->y+monList[i]->height-1)/wall->height)+1][(monList[i]->x+monList[i]->width-1)/wall->width] == 0) || Map[(monList[i]->y+monList[i]->height-1)/wall->height][(monList[i]->x+monList[i]->width-1)/wall->width] == 1)
					{
						monList[i]->mX = -monList[i]->mX;
						monList[i]->Flip(0);
					}
				}
				else if(monList[i]->mX < 0)
				{	
					if((Map[(monList[i]->y+monList[i]->height-1)/wall->height][monList[i]->x/wall->width] == 0 && Map[((monList[i]->y+monList[i]->height-1)/wall->height)+1][monList[i]->x/wall->width] == 0) || Map[(monList[i]->y+monList[i]->height-1)/wall->height][monList[i]->x/wall->width] == 1)
					{
						monList[i]->mX = -monList[i]->mX;
						monList[i]->Flip(1);
					}
				}
			}

			monList[i]->Render();

			if(!monList[i]->dead && !end)
			{
				if(monList[i]->HitTest(mc))
				{
					if(monList[i]->y+(monList[i]->height/2) < mc->y+mc->height)
					{
						end = true;
						mc->Stop();

						soundKill->Play();
					}else{
						monList[i]->dead = true;
						mY = -20;
						AddEffect(mc);

						soundDamage->Play();
					}
				}
			}else{
				if(monList[i]->y > screenHeight)
				{
					delete monList[i];
					monList[i] = NULL;
					monList.erase(monList.begin()+i);
				}
			}
		}

		mc->Render();

		if(coin.size() == 0)
		{
			end = true;
			mc->Stop();
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

		if(end)
			gameOver->Render();
	}
}

void GameClear()
{
	int i;

	delete bg;
	bg = NULL;

	for(i = 0; i < tile.size(); i++)
	{
		delete tile[i];
		tile[i] = NULL;
	}

	tile.clear();

	for(i = 0; i < coin.size(); i++)
	{
		delete coin[i];
		coin[i] = NULL;
	}

	coin.clear();

	delete coinSurface;
	coinSurface = NULL;

	delete mc;
	mc = NULL;

	for(i = 0; i < monList.size(); i++)
	{
		delete monList[i];
		monList[i] = NULL;
	}

	monList.clear();

	delete catSurface;
	catSurface = NULL;

	for(i = 0; i < effList.size(); i++)
	{
		delete effList[i];
		effList[i] = NULL;
	}

	effList.clear();

	delete effectSurface;
	effectSurface = NULL;

	music->Stop();
	delete music;
	music = NULL;

	delete soundDamage;
	soundDamage = NULL;

	delete soundCoin;
	soundCoin = NULL;

	delete soundKill;
	soundKill = NULL;

	delete gameOver;
	gameOver = NULL;
}

void Event()
{
	switch(bis->GetEvent().type)
	{
	case SDL_KEYDOWN:
		if(bis->GetEvent().key.keysym.sym == SDLK_SPACE)
		{
			if(!end)
			{
				if(!kspace && !jump)
				{
					mY = -50;
					jump = true;
					kspace = true;
				}
			}else{
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
			if(end)
				bis->Quit();
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
	GameClear();

	delete bis;
	bis = NULL;
}

int main(int argc, char* argv[])
{
	bis = new GameWindow();

	if(!bis->CreateWindow("Sample Game Mario", screenWidth, screenHeight))
	{
		cerr << "Can't Create GameWindow" << endl;
	}else{
		bis->Run((void*)&Start, (void*)&Update, (void*)&Event);
	}

	Close();

	return 0;
}