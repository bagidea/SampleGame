#include "GameObject.h"

//GameObject Class
GameObject::GameObject(SDL_Renderer* renderer)
{
	timeScale = 1.0;
	speedAnimation = 1;
	index = 0;

	x = 0;
	y = 0;
	width = 0;
	height = 0;
	rotation = 0;

	this->tex = new GameTexture();
	this->renderer = NULL;
	
	if(renderer != NULL)
	{
		this->renderer = renderer;
		this->tex->SetRenderer(renderer);
	}

	isPlay = false;
	loop = true;
}

GameObject::~GameObject()
{
	clipList.clear();

	delete tex;
	tex = NULL;
}

void GameObject::SetRenderer(SDL_Renderer* renderer)
{
	this->renderer = renderer;
	tex->SetRenderer(renderer);
}

void GameObject::Load(string path)
{
	tex->Load(path);

	x = tex->GetX();
	y = tex->GetY();
	width = tex->GetWidth();
	height = tex->GetHeight();
}

void GameObject::SetTexture(GameTexture* tex)
{
	this->tex = tex;
}

bool GameObject::CreateGameTextureFromGameSurface(GameSurface* gameSurface)
{
	bool success = true;

	if(gameSurface != NULL || gameSurface->GetSurface() != NULL)
	{
		SDL_Texture* gTex = SDL_CreateTextureFromSurface(renderer, gameSurface->GetSurface());
		if(gTex == NULL)
		{
			success = false;
			cerr << "SDL Create Texture Error : " << SDL_GetError() << endl;
		}

		width = gameSurface->GetSurface()->w;
		height = gameSurface->GetSurface()->h;

		tex->SetWidth(width);
		tex->SetHeight(height);
		tex->SetClip(0, 0, width, height);
		tex->SetTexture(gTex);
	}else{
		success = false;
		cerr << "Create Error - GameSurface" << endl;
	}

	return success;
}

void GameObject::AddClip(SDL_Rect* clip)
{
	for(int i = 0; i < sizeof(clip); i++)
	{
		clipList.push_back(clip[i]);
	}

	SetAnimation(0, clipList.size());
}

void GameObject::LoadClip(string path)
{
	string line;

	ifstream in(path.c_str());
	if(!in.is_open())
	{
		cerr << "Can't Load File : " << path << endl;
	}else{
		while(in.good())
		{
			getline(in, line);
			int _x, _y, _w, _h;
			sscanf(line.c_str(), "%d/%d/%d/%d", &_x, &_y, &_w, &_h);

			SDL_Rect rect = {_x, _y, _w, _h};
			clipList.push_back(rect);
		}
	}

	SetAnimation(0, clipList.size());
}

void GameObject::GenerateClip(int cols, int rows)
{
	int cW = width/cols;
	int cH = height/rows;

	for(int i = 0; i < rows; i++)
	{
		for(int a = 0; a < cols; a++)
		{
			SDL_Rect rect = {a*cW, i*cH, cW, cH};
			clipList.push_back(rect);
		}
	}

	SetAnimation(0, clipList.size());
}

void GameObject::SetCenter(int x, int y)
{
	tex->SetCenter(x, y);
}

void GameObject::SetFrame(int index)
{
	this->index = index;
}

void GameObject::SetAnimation(int start, int end)
{
	this->start = start;
	this->end = end;
	index = this->start*speedAnimation;
}

void GameObject::SetFlip(SDL_RendererFlip flip)
{
	tex->SetFlip(flip);
}

bool GameObject::HitTest(GameObject* hit)
{
	int upA, upB, downA, downB, leftA, leftB, rightA, rightB;

	upA = y;
	upB = hit->y;
	downA = y+height;
	downB = hit->y + hit->height;

	leftA = x;
	leftB = hit->x;
	rightA = x+width;
	rightB = hit->x + hit->width;

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

void GameObject::SetTimeScale(float tmr)
{
	if(tmr >= 1.0)
		tmr = 1.0;
	else if(tmr <= 0.0)
		tmr = 0.0;

	timeScale = tmr;
	speedAnimation = 10 - (9 * timeScale);
}

void GameObject::SetClip(vector<SDL_Rect> clip)
{
	clipList.clear();
	clipList = clip;
}

vector<SDL_Rect> GameObject::GetClip()
{
	return clipList;
}

void GameObject::Render()
{
	tex->SetX(x);
	tex->SetY(y);
	tex->SetWidth(width);
	tex->SetHeight(height);
	tex->SetRotate(rotation);

	if(clipList.size() > 0)
	{
		if(speedAnimation == 0)
		{
			tex->SetClip(clipList[index].x, clipList[index].y, clipList[index].w, clipList[index].h);
		}else{
			tex->SetClip(clipList[index/speedAnimation].x, clipList[index/speedAnimation].y, clipList[index/speedAnimation].w, clipList[index/speedAnimation].h);
		}

		if(isPlay)
		{
			if(timeScale > 0)
			{
				index++;

				if(index/speedAnimation >= end)
				{
					if(loop)
						index = start*speedAnimation;
					else
						Stop();
				}
			}
		}
	}

	tex->Render();
}

void GameObject::ClearClip()
{
	clipList.clear();
}

GameTexture* GameObject::GetTexture(){return tex;}
float GameObject::GetTimeScale(){return timeScale;}
bool GameObject::IsPlay(){return isPlay;}
void GameObject::Play(){isPlay = true;};
void GameObject::Stop(){isPlay = false;};
int GameObject::GetStart(){return start;}
int GameObject::GetEnd(){return end;}

void GameObject::SetLoop(bool loop)
{
	this->loop = loop;
}