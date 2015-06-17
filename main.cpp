#include "GameWindow.h"

GameWindow bis;

void Start()
{

}

void Update()
{

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