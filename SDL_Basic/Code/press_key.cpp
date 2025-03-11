#include "SDL.h"
#include "stdio.h"
#include "string"
// Screen dimensions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

enum key {
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_S,
	KEY_PRESS_SURFACE_TOTAL
};

// window
SDL_Window* gWindow = NULL;

// The surface contained in window
SDL_Surface* gScreenSurface = NULL;

// The image that coresponds to keypress
SDL_Surface* gKeyPressSurface[KEY_PRESS_SURFACE_TOTAL];

// Current display image
SDL_Surface* gCurrentSurface;

bool init()
{
	bool flag = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize the window! SDL error : %s\n", SDL_GetError());
		flag = false;
	}
	else
	{
		gWindow = SDL_CreateWindow("Key press", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL error : %s\n", SDL_GetError());
			flag = false;
		}
		else
		{
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}
	return flag;
}

SDL_Surface* loadSurface(std::string path)
{

	SDL_Surface* optimizedSurface = NULL;

	SDL_Surface* loadSurface = SDL_LoadBMP(path.c_str());

	if (loadSurface == NULL)
	{
		printf("SDL could not load the surface! SDL error: %s\n", SDL_GetError());
	}
	else 
	{
		optimizedSurface = SDL_ConvertSurface(loadSurface, gScreenSurface->format, 0);
		if (optimizedSurface == NULL)
		{
			printf("Fail to convert image %s! SDL error %s. /n", path.c_str(), SDL_GetError());
		}

		SDL_FreeSurface(loadSurface);
	}
	return optimizedSurface;
}

bool loadMedia()
{
	//bool success flag
	bool success = true;

	// load default surface
	gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT] = SDL_LoadBMP("Image\\PressImage\\press.bmp");
	if (gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT] == NULL)
	{
		printf("Fail to load defalut image! SDL error: %s\n", SDL_GetError());
		success = false;
	}

	// Load down image
	gKeyPressSurface[KEY_PRESS_SURFACE_DOWN] = SDL_LoadBMP("Image\\PressImage\\down.bmp");
	if (gKeyPressSurface[KEY_PRESS_SURFACE_DOWN] == NULL)
	{
		printf("Fail to load down image! SDL error: %s\n", SDL_GetError());
		success = false;
	}

	// Load up image
	gKeyPressSurface[KEY_PRESS_SURFACE_LEFT] = SDL_LoadBMP("Image\\PressImage\\left.bmp");
	if (gKeyPressSurface[KEY_PRESS_SURFACE_LEFT] == NULL)
	{
		printf("Fail to load left image! SDL_Error! %s\n", SDL_GetError());
		success = false;
	}

	gKeyPressSurface[KEY_PRESS_SURFACE_RIGHT] = SDL_LoadBMP("Image\\PressImage\\right.bmp");
	if (gKeyPressSurface[KEY_PRESS_SURFACE_RIGHT] == NULL)
	{
		printf("Fail to load right image! SDL error: %s \n", SDL_GetError());
		success = false;
	}

	gKeyPressSurface[KEY_PRESS_SURFACE_UP] = SDL_LoadBMP("Image\\PressImage\\up.bmp");
	if (gKeyPressSurface[KEY_PRESS_SURFACE_UP] == NULL)
	{
		printf("Fail to laod up image! SDL error: %s\n", SDL_GetError());
		success = false;
	}

	return success;
}

void close()
{
	for (int i = 0; i < KEY_PRESS_SURFACE_TOTAL; ++i)
	{
		SDL_FreeSurface(gKeyPressSurface[i]);
		gKeyPressSurface[i] = NULL;
	}

	SDL_DestroyWindow(gWindow);

	SDL_Quit();
}



int main(int argc, char* args[])
{
	if (!init())
	{
		printf("Fail to load window");
	}
	else
	{
		gKeyPressSurface[KEY_PRESS_SURFACE_S] = loadSurface("Image\\PressImage\\stretch.bmp");

		if (!loadMedia())
		{
			printf("Fail to load image");
		}
		else
		{
			bool quit = false;

			SDL_Event e;

			SDL_Rect strectchRect;
			strectchRect.x = 0;
			strectchRect.y = 0;
			strectchRect.w = SCREEN_WIDTH;
			strectchRect.h = SCREEN_HEIGHT;

			gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT];
			SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);

			SDL_UpdateWindowSurface(gWindow);

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
						case SDLK_UP:
							gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_UP];
							break;
						case SDLK_DOWN:
							gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DOWN];
							break;
						case SDLK_LEFT:
							gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_LEFT];
							break;
						case SDLK_RIGHT:
							gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_RIGHT];
							break;
						case SDLK_s :
							gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_S];
							break;
						default:
							gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT];
						}
					}
				}

				SDL_BlitScaled(gCurrentSurface, NULL, gScreenSurface, &strectchRect);
				//SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);

				SDL_UpdateWindowSurface(gWindow);
			}
		}
	}

	close();
	return 0;
}