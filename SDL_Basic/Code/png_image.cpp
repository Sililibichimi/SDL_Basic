#include "SDL.h"
#include <stdio.h>
#include "string"
#include "SDL_image.h"

const int SCREEN_HEIGHT = 480;
const int SCREEN_WIDTH = 640;

SDL_Window* gWindow = NULL;

SDL_Surface* gScreenSurface = NULL;

SDL_Surface* gCurrentSurface = NULL;

bool init();

bool loadMedia();

void close();

bool init()
{
	bool flag = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not intialize window! SDl error %s\n", SDL_GetError());
		flag = false;
	}
	else
	{
		gWindow = SDL_CreateWindow("PNG Image", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("SDL could not create Window! SDL error %s\n", SDL_GetError());
			flag = false;
		}
		else 
		{
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) & imgFlags))
			{
				printf("SDL image could not initialize here! SDL_Image  error: %s\n", SDL_GetError());
				flag = false;
			}
			else
			{
				// Get window surface
				gScreenSurface = SDL_GetWindowSurface(gWindow);
			}
		}
	}
	return flag;
}

SDL_Surface* loadImage(std::string path)
{
	SDL_Surface* loadSurface = IMG_Load(path.c_str());
	SDL_Surface* optimizedSurface = NULL;

	if (loadSurface == NULL)
	{
		printf("Fail to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		return NULL;
	}
	else
	{
		optimizedSurface = SDL_ConvertSurface(loadSurface, gScreenSurface->format, 0);

		if (optimizedSurface == NULL)
		{
			printf("Unable to optimize image %s! SDL Error: %s", path.c_str(), SDL_GetError());
		}

		SDL_FreeSurface(loadSurface);
	}

	return optimizedSurface;
}

void close()
{
	SDL_FreeSurface(gScreenSurface);
	gCurrentSurface = NULL;
	
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	IMG_Quit();
	SDL_Quit();
}

int main(int agrc, char* args[])
{
	if (!init())
	{
		printf("Fail to create window\n");
	}
	else
	{
		gCurrentSurface = loadImage("Image\\Png\\loaded.png");
		if (gCurrentSurface == NULL)
		{
			printf("Fail to load image!!!");
		}
		else
		{

			SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);

			SDL_UpdateWindowSurface(gWindow);

			SDL_Event e;
			bool quit = false;

			while (!quit)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
				}
			}
		}
	}

	close();
	return 0;
}