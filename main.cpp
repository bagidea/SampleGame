#include "GameWindow.h"
#include "GameTexture.h"

GameWindow bis;
GameTexture image;

void Start()
{
	image.SetRenderer(bis.GetRenderer());
	if(!image.Load("source/BG.png"))
	{
		cerr << "Load source/Bomb.png Fail" << endl;
	}
}

void Update()
{
	image.Render();
}

int main(int argc, char* argv[])
{
	if(!bis.CreateWindow("Sample Game", 800, 600))
	{
		cerr << "Can't Create GameWindow" << endl;
	}else{
		bis.Run((void*)&Start, (void*)Update);
	}

	return 0;
}