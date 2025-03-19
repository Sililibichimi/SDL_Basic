#include "stdio.h"
#include "string"
#include "SDL.h"
#include "SDL_image.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class LTexture
{
public :

	LTexture();
	~LTexture();

	bool loadFromPath(std::string path);

	void free();

	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	void render(int x, int y, SDL_Rect* clip = NULL);

	int getWidth();
	int getHeight();

private :
	int mHeight;
	int mWidth;
	SDL_Texture* mTexture;
};

// SDL start up and create window
bool init();

bool loadMedia();

void close();

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

LTexture gModulatedTexture;


// Class LTexture function
LTexture::LTexture()
{
	mHeight = 0;
	mWidth = 0;
	mTexture = NULL;
}

LTexture::~LTexture()
{
	free();
}

bool LTexture::loadFromPath(std::string path)
{
	free();

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (loadedSurface == NULL) {
		printf("Could not load image! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}
	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
	
	if (newTexture == NULL) {
		printf("Fail to load image from path : %s. SDL Error : %s", path.c_str(), SDL_GetError());
		return false;
	}

	mHeight = loadedSurface->h;
	mWidth = loadedSurface->w;

	mTexture = newTexture;

	SDL_FreeSurface(loadedSurface);

	return mTexture != NULL;
}

void LTexture::free()
{
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mHeight = 0;
		mWidth = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::render(int x, int y, SDL_Rect* clip)
{

	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}


bool init()
{
	bool flag = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		flag = false;
		printf("Could not initialize a window, SDL Error : %s\n", SDL_GetError());
	}
	else
	{
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enable!\n");
		}

		gWindow = SDL_CreateWindow("Color Modulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		
		if (gWindow == NULL)
		{
			flag = false;
			printf("Could not create window! SDL Error %s\n", SDL_GetError());
		}
		else {

			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

			if (gRenderer == NULL)
			{
				flag = false;
				printf("Could not initialize renderer! SDL Error: %s\n", SDL_GetError());
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);

				int imgFlag = IMG_INIT_PNG;

				if (!(IMG_Init(imgFlag) & imgFlag))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					flag = false;
				}
			}
		}
	}

	return flag;
}

bool loadMedia()
{
	bool flag = true;

	if (!gModulatedTexture.loadFromPath("Image\\color_modulation\\colors.png"))
	{
		flag = false;
	}

	return flag;
}

void close()
{
	gModulatedTexture.free();

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gRenderer = NULL;
	gWindow = NULL;

	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* arg[])
{

	if (!init())
	{
		printf("Fail to init\n");
	}
	else
	{
		if (!loadMedia())
		{
			printf("Fail to load image\n");
		}
		else 
		{
			bool quit = false;
			SDL_Event e;

			// Modulation component
			Uint8 r = 255;
			Uint8 g = 255;
			Uint8 b = 255;

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
							//Increase red
						case SDLK_q:
							r += 32;
							break;

							//Increase green
						case SDLK_w:
							g += 32;
							break;

							//Increase blue
						case SDLK_e:
							b += 32;
							break;

							//Decrease red
						case SDLK_a:
							r -= 32;
							break;

							//Decrease green
						case SDLK_s:
							g -= 32;
							break;

							//Decrease blue
						case SDLK_d:
							b -= 32;
							break;
						}
					}
				}

				SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
				SDL_RenderClear(gRenderer);

				gModulatedTexture.setColor(r, g, b);
				gModulatedTexture.render(0, 0);

				SDL_RenderPresent(gRenderer);
			}

		}
	}

	close();
	return 0;	
}

