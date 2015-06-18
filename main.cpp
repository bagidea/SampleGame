#include "GameWindow.h"
#include "GameTexture.h"

GameWindow* bis;
GameTexture* image1;
GameTexture* image2;

void Start()
{
	image1 = new GameTexture();
	image1->SetRenderer(bis->GetRenderer());

	image2 = new GameTexture();
	image2->SetRenderer(bis->GetRenderer());

	if(!image1->Load("source/SPtest.png") || !image2->Load("source/SPtest.png"))
	{
		cerr << "Load source/SPtest.png Fail" << endl;
	}

	image1->SetClip(0, 0, 90, 140);
	image2->SetClip(90, 0, 90, 140);
	image2->SetX(150);
}

void Update()
{
	image1->Render();
	image2->Render();
}

void Close()
{
	delete image1;
	delete image2;
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