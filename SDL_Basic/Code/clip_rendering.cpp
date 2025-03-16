#include "SDL.h"
#include "SDL_image.h"
#include "stdio.h"
#include "string"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class LTexture
{
public :

	LTexture();
	~LTexture();

	void free();

	bool loadFromPath(std::string path);

	void render(int x, int y, SDL_Rect* clip = NULL);

	int getWidth();
	int getHeight();

private:
	SDL_Texture* mTexture;
	int mHeight;
	int mWidth;
	
};

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

bool init();

bool loadMedia();

LTexture::LTexture()
{
	mTexture = NULL;
	mHeight = 0;
	mWidth = 0;
}

LTexture::~LTexture()
{
	free();
}

int LTexture::getHeight()
{
	return mHeight;
}

int LTexture::getWidth()
{
	return mWidth;
}

bool LTexture::loadFromPath(std::string path)
{
	free();

	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	
	if (loadedSurface == NULL)
	{
		printf("Fail to load image!");
		return false;
	}
	else
	{
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xff, 0xff));
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);

		mTexture = newTexture;

		mWidth = loadedSurface->w;
		mHeight = loadedSurface->h;

		SDL_FreeSurface(loadedSurface);
	}
	return mTexture != NULL;
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

bool init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		return false;
	}
	else
	{
		gWindow = SDL_CreateWindow("Clip Rendering", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			return false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				return false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);

				int imglags = IMG_INIT_PNG;
				if (!(IMG_Init(imglags) & imglags))
				{
					return false;
				}
			}
		}
	}
	return true;
}

SDL_Rect gSprite[4];
LTexture gSpriteSheetTexture;

bool loadMedia()
{
	bool success = true;

	if (!gSpriteSheetTexture.loadFromPath("Image\\ClipRendering\\dots.png"))
	{
		success = false;
	}
	else
	{
		gSprite[0].x = 0;
		gSprite[0].y = 0;
		gSprite[0].w = 100;
		gSprite[0].h = 100;

		gSprite[1].x = 0;
		gSprite[1].y = 100;
		gSprite[1].w = 100;
		gSprite[1].h = 100;

		gSprite[2].x = 100;
		gSprite[2].y = 0;
		gSprite[2].w = 100;
		gSprite[2].h = 100;

		gSprite[3].x = 100;
		gSprite[3].y = 100;
		gSprite[3].w = 100;
		gSprite[3].h = 100;
	}

	return success;
}

void close()
{
	gSpriteSheetTexture.free();

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);

	gRenderer = NULL;
	gWindow = NULL;
}

int main(int argc, char* arg[])
{
	if (!init())
	{
		printf("Fail to initialize window, %s",SDL_GetError());
	}
	else
	{
		if (!loadMedia())
		{
			printf("Fail to load image, %s", SDL_GetError());
		}
		else
		{
			SDL_Event e;
			bool quit = false;

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

				gSpriteSheetTexture.render(0, 0, &gSprite[0]);

				gSpriteSheetTexture.render(SCREEN_WIDTH - gSprite[1].w, 0, &gSprite[1]);

				gSpriteSheetTexture.render(0, SCREEN_HEIGHT - gSprite[2].h, &gSprite[2]);

				gSpriteSheetTexture.render(SCREEN_WIDTH - gSprite[3].w, SCREEN_HEIGHT - gSprite[3].h, &gSprite[3]);

				SDL_RenderPresent(gRenderer);
			}
		}
	}

	close();
	return 0;
}