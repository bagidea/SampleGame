#include "GameWindow.h"
#include "GameTexture.h"

GameWindow* bis;
GameTexture* image;

int speed;

void Start()
{
	speed = 1;

	image = new GameTexture();
	image->SetRenderer(bis->GetRenderer());
	if(!image->Load("source/Bomb.png"))
	{
		cerr << "Load source/Bomb.png Fail" << endl;
	}

	image->SetWidth(50);
	image->SetHeight(50);
}

void Update()
{
	image->SetX(image->GetX()+speed);
	image->SetY(image->GetY()+speed);
	image->SetWidth(image->GetWidth()+speed);
	image->SetHeight(image->GetHeight()+speed);
	image->Render();
}

void Close()
{
	delete image;
	delete bis;
}

int main(int argc, char* argv[])
{
	bis = new GameWindow();

	if(!bis->CreateWindow("Sample Game", 800, 600))
	{
		cerr << "Can't Create GameWindow" << endl;
	}else{
		bis->Run((void*)&Start, (void*)Update);
	}

	Close();

	return 0;
}