#include "SDL.h"
#include "SDL_image.h"
#include "stdio.h"
#include "string"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class LTexture
{
public:
	LTexture();

	~LTexture();

	bool loadFromFile(std::string path);

	void free();

	void render(int x, int y);

	int getWidth();
	int getHeight();

private :
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
};	

bool init();
bool loadMedia();
bool loadTexture(std::string path);

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

LTexture gFooTexture;
LTexture gBackGroundTexture;

LTexture::LTexture()
{
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	free();
}

bool LTexture::loadFromFile(std::string path)
{
	free();

	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadSurface = IMG_Load(path.c_str());

	if (loadSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		SDL_SetColorKey(loadSurface, SDL_TRUE, SDL_MapRGB(loadSurface->format, 0, 0xFF, 0xFF));
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadSurface);

		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			mHeight = loadSurface->h;
			mWidth = loadSurface->w;
		}
		SDL_FreeSurface(loadSurface);
	}
	mTexture = newTexture;
	return mTexture != NULL;
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

void LTexture::free()
{
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::render(int x, int y)
{
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	SDL_RenderCopy(gRenderer, mTexture, NULL, &renderQuad);
}

bool init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Could not initialize window! %s", SDL_GetError());
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow("Color keying", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Could not create window here! %s", SDL_GetError());
		}
		else
		{
			if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
			{
				printf("Warning: Linear texture filtering not enabled!");
			}
			else
			{
				gRenderer = SDL_CreateRenderer(gWindow,-1, SDL_RENDERER_ACCELERATED);
				if (gRenderer == NULL)
				{
					printf("Renderer could not be created!SDL Error : % s\n", SDL_GetError());
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
	}

	return success;
}


bool loadMedia()
{
	bool success = true;

	if (!gFooTexture.loadFromFile("Image\\ColorKeying\\foo.png"))
	{
		printf("Failed to load Foo' texture image!\n");
		success = false;
	}

	if (!gBackGroundTexture.loadFromFile("Image\\ColorKeying\\background.png"))
	{
		printf("Failed to load Backgournd' texture image!\n");
		success = false;
	}

	return success;
}

void close()
{
	gFooTexture.free();
	gBackGroundTexture.free();

	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
	IMG_Quit();
}

int main(int argc, char* agr[])
{
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			bool quit = false;
			SDL_Event e;

			while (!quit)
			{

				while (SDL_PollEvent(&e))
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
				}

				SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
				SDL_RenderClear(gRenderer);

				gBackGroundTexture.render(0, 0);
				
				gFooTexture.render(240, 190);

				SDL_RenderPresent(gRenderer);
			}
		}
	}

	close();
	return 0;
}