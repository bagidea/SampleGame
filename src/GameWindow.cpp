#include "GameWindow.h"

//GameWindow Class
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

				if(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0)
				{
					success = false;
					cerr << "IMG Init Error : " << IMG_GetError() << endl;
				}

				if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2028) < 0)
				{
					success = false;
					cerr << "MIX Init Error : " << Mix_GetError() << endl;
				}
			}
		}
	}

	return success;	
}

void GameWindow::Run(void* Start, void* Update, void* Event)
{
	this->Start = (void(*)(void))Start;
	this->Update = (void(*)(void))Update;
	this->Event = (void(*)(void))Event;

	if(this->Start != NULL)
	{
		this->Start();
	}

	while(!quit)
	{
		while(SDL_PollEvent(&e) != 0)
		{
			if(this->Event != NULL)
			{
				this->Event();
			}

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

void GameWindow::Quit()
{
	quit = true;
}

SDL_Renderer* GameWindow::GetRenderer()
{
	return renderer;
}

SDL_Event GameWindow::GetEvent()
{
	return e;
}

void GameWindow::Close()
{
	if(renderer != NULL)
		SDL_DestroyRenderer(renderer);
	renderer = NULL;

	if(window != NULL)
		SDL_DestroyWindow(window);
	window = NULL;

	SDL_Quit();
}