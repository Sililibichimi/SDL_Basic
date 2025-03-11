#include "SDL.h"
#include "SDL_image.h"
#include "stdio.h"
#include "string"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGTH = 480;

bool init();

SDL_Texture* loadTexture(std::string path);

bool loadMedia();

void close();


SDL_Window* gWindow = NULL;

SDL_Texture* gTexture = NULL;

SDL_Renderer* gRenderer = NULL;

bool init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize the window! SDL Error %s", SDL_GetError());
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow("Texture loading and Rendering", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGTH,SDL_WINDOW_SHOWN);

		if (gWindow == NULL)
		{
			printf("SDL could not create the window! SDL Error %s", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("SDL could not create rederer here! SDL Error %s", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);

				int imageFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imageFlags) & imageFlags))
				{
					printf("SDL image could not initial here ! SDL Error %s", SDL_GetError());
					success = false;
				}
			}
		}
		
	}
	return success;
}

SDL_Texture* loadTexture(std::string path)
{
	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if(loadedSurface == NULL)
	{
		printf("Unalbe to load image %s ! SDL Error %s", path.c_str(), SDL_GetError());
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Fail to create new texture ! SDL Error %s", SDL_GetError());
		}

		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

bool loadMedia()
{
	bool success = true;

	gTexture = loadTexture("Image\\Texture\\texture.png");

	if (gTexture == NULL)
	{
		printf("Fail to load texture!!!");
		success = false;
	}

	return success;
}

void close()
{
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;

	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
	IMG_Quit();
}

int main(int argc, char* arg[])
{
	if (!init())
	{
		printf("Fail to init!!!");
	}
	else
	{
		if (!loadMedia())
		{
			printf("Fail to load media!!!");
		}
		else
		{
			SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
			SDL_RenderClear(gRenderer);

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
						case SDLK_c :
							SDL_RenderClear(gRenderer);
							break;
						case SDLK_p :
							SDL_RenderClear(gRenderer);
							SDL_Rect strectchRect;
							strectchRect.x = 0;
							strectchRect.y = 0;
							strectchRect.w = SCREEN_WIDTH;
							strectchRect.h = SCREEN_HEIGTH;
							SDL_RenderSetViewport(gRenderer, &strectchRect);
							SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
							break;
						case SDLK_s :
						{
							SDL_RenderClear(gRenderer);
							SDL_Rect topLeftView = {
								0, 
								0, 
								SCREEN_WIDTH / 2, 
								SCREEN_HEIGTH / 2
							};
							SDL_RenderSetViewport(gRenderer, &topLeftView);
							SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

							SDL_Rect topRightView = { 
								SCREEN_WIDTH / 2,
								0, 
								SCREEN_WIDTH / 2, 
								SCREEN_HEIGTH / 2 
							};
							SDL_RenderSetViewport(gRenderer, &topRightView);
							SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

							SDL_Rect bottomView = {
								0,
								SCREEN_HEIGTH / 2,
								SCREEN_WIDTH,
								SCREEN_HEIGTH / 2,
								
							};
							SDL_RenderSetViewport(gRenderer, &bottomView);
							SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
						}
						}
					}
				}
				SDL_RenderPresent(gRenderer);
			}
		}
	}

	close();
	return 0;
}
