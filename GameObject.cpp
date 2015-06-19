#include "GameObject.h"

GameObject::GameObject(SDL_Renderer* renderer)
{
	this->tex = new GameTexture();
	this->renderer = NULL;
	
	if(renderer != NULL)
	{
		this->renderer = renderer;
		this->tex->SetRenderer(renderer);
	}
}

GameObject::~GameObject()
{
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
}

void GameObject::Render()
{
	tex->Render();
}