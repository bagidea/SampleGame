#include "GameWindow.h"
#include "GameTexture.h"

GameWindow* bis;
GameTexture* image1;

int frame;
int m;
SDL_Rect clip[8];

void Start()
{
	frame = 0;
	m = 3;

	clip[0].x = 0;
	clip[0].y = 0;
	clip[0].w = 90;
	clip[0].h = 140;

	clip[1].x = 90;
	clip[1].y = 0;
	clip[1].w = 90;
	clip[1].h = 140;

	clip[2].x = 180;
	clip[2].y = 0;
	clip[2].w = 90;
	clip[2].h = 140;

	clip[3].x = 270;
	clip[3].y = 0;
	clip[3].w = 90;
	clip[3].h = 140;

	clip[4].x = 360;
	clip[4].y = 0;
	clip[4].w = 90;
	clip[4].h = 140;

	clip[5].x = 450;
	clip[5].y = 0;
	clip[5].w = 90;
	clip[5].h = 140;

	clip[6].x = 540;
	clip[6].y = 0;
	clip[6].w = 90;
	clip[6].h = 140;

	clip[7].x = 630;
	clip[7].y = 0;
	clip[7].w = 90;
	clip[7].h = 140;

	image1 = new GameTexture();
	image1->SetRenderer(bis->GetRenderer());

	if(!image1->Load("source/SPtest.png"))
	{
		cerr << "Load source/SPtest.png Fail" << endl;
	}

	image1->SetClip(clip[0].x, clip[0].y, clip[0].w, clip[0].h);
	image1->SetX(355);
	image1->SetY(230);
	image1->SetWidth(90);
	image1->SetHeight(140);
}

void Update()
{
	image1->SetClip(clip[frame/m].x, clip[frame/m].y, clip[frame/m].w, clip[frame/m].h);
	image1->Render();

	frame++;
	if(frame / m >= 8)
		frame = 0;
}

void Close()
{
	delete image1;
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