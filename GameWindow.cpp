#include "GameWindow.h"

GameWindow::GameWindow()
{
	quit = false;

	window = NULL;
	renderer = NULL;

	Start = NULL;
	Update = NULL;
}

GameWindow::~GameWindow()
{
	Close();
}

bool GameWindow::CreateWindow(string title, int windowWidth, int windowHeight)
{
	bool success = true;

	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		success = false;
		cerr << "SDL Init Error : " << SDL_GetError() << endl;
	}else{
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
		if(window == NULL)
		{
			success = false;
			cerr << "SDL Create Window Error : " << SDL_GetError() << endl;
		}else{
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if(renderer == NULL)
			{
				success = false;
				cerr << "SDL Create Renderer Error : " << SDL_GetError() << endl;
			}else{
				SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
			}
		}
	}

	return success;	
}

void GameWindow::Run(void* Start, void* Update)
{
	this->Start = (void(*)(void))Start;
	this->Update = (void(*)(void))Update;

	if(this->Start != NULL)
	{
		this->Start();
	}

	while(!quit)
	{
		while(SDL_PollEvent(&e) != 0)
		{
			switch(e.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				if(e.key.keysym.sym == SDLK_ESCAPE)
				{
					quit = true;
					break;
				}
			}
		}

		SDL_RenderClear(renderer);

		if(this->Update != NULL)
		{
			this->Update();
		}

		SDL_RenderPresent(renderer);
	}

	Close();
}

void GameWindow::Close()
{
	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	SDL_DestroyWindow(window);
	window = NULL;

	SDL_Quit();
}