#include "GameWindow.h"
#include "GameTexture.h"

GameWindow* bis;
GameTexture* image;

int angle;
int speed;

void Start()
{
	angle = 0;
	speed = 1;

	image = new GameTexture();
	image->SetRenderer(bis->GetRenderer());
	if(!image->Load("source/Bomb.png"))
	{
		cerr << "Load source/Bomb.png Fail" << endl;
	}

	image->SetWidth(300);
	image->SetHeight(300);
	image->SetCenter(150, 150);
	image->SetX(250);
	image->SetY(150);
}

void Update()
{
	angle++;
	image->SetRotate(angle);
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