#include "SDL.h"
#include "stdio.h"
#include "SDL_image.h"
#include "cmath"
#include "string"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();
bool loadMedia();
void close();
SDL_Texture* loadTexture(std::string path);
SDL_Window* gWindow = NULL;

SDL_Texture* loadedTexture = NULL;

SDL_Renderer* gRenderer = NULL;

bool init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filltering not enable");
		}

		gWindow = SDL_CreateWindow("Geometry Rendering", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}
	return success;
}

bool loadMedia()
{
	bool flag = true;
	return flag; // nothing to load
}

void close()
{
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
	IMG_Quit();
}

SDL_Texture* loadTexture(std::string path)
{
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	SDL_Texture* newTexture = NULL;

	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

int main(int argc, char* arg[])
{
	if (init())
	{
		if (loadMedia())
		{
			bool quit = false;
			SDL_Event e;

			while (!quit)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					else if (e.type == SDL_KEYDOWN)
					{
						switch (e.key.keysym.sym)
						{
						case SDLK_1:
						{
							SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
							SDL_SetRenderDrawColor(gRenderer, 0xff, 0x00, 0x00, 0xff);
							SDL_RenderFillRect(gRenderer, &fillRect);
							break;
						}
							
							
						case SDLK_2:
						{
							SDL_Rect oulineRect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3 };
							SDL_SetRenderDrawColor(gRenderer, 0x00, 0xff, 0x00, 0xff);
							SDL_RenderDrawRect(gRenderer, &oulineRect);
							break;
						}
							
						case SDLK_3:
						{
							SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xff, 0xff);
							SDL_RenderDrawLine(gRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);
							break;
						}
						case SDLK_4:
						{
							SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
							for (int i = 0; i < SCREEN_HEIGHT; i += 4)
							{
								SDL_RenderDrawPoint(gRenderer, SCREEN_WIDTH / 2, i);
							}
							break;
						}
						default:
						{
							SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
							SDL_RenderClear(gRenderer);
						}
						
						}
						SDL_RenderPresent(gRenderer);
					}
				}

				
			}
		}

	}
	close();
	return 0;
}