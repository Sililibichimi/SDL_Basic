#include "SDL.h"
#include "SDL_image.h"
#include "stdio.h"
#include "string"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int  TOTAL_BUTTONS = 4;

enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};

class LTexture
{
public:
	LTexture();
	~LTexture();

	bool loadFromFile(std::string);
	#if defined(SDL_TTF_MAJOR)
	bool loadFromRenderedText(std::string textTexture, SDL_Color textColor);
	#endif

	void free();
	void setColor(Uint8 r, Uint8 g, Uint8 b);
	void setBlend(SDL_BlendMode blending);
	void setAlpha(Uint8 alpha);

	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0,SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	int getWidth();
	int getHeight();
	
private:
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
};

class LButton
{
public:
	LButton();
	
	void setPosition(int x, int y);
	void handleEvent(SDL_Event* e);

	void render();

private:
	SDL_Point mPosition;

	LButtonSprite mCurrentSprite;
};

bool init();
bool loadMedia();
void close();

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

SDL_Rect gSpriteClip[BUTTON_SPRITE_TOTAL];
LTexture gButtonSpriteSheetTexture;
LButton gButtons[TOTAL_BUTTONS];

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

bool LTexture::loadFromFile(std::string path)
{
	free();

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xff, 0xff));

		mTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);

		if (mTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			mHeight = loadedSurface->h;
			mWidth = loadedSurface->w;
		}

		SDL_FreeSurface(loadedSurface);
	}

	return mTexture != NULL;
}


#if defined(SDL_TTF_MAJOR_VERSION)
bool loadFromRenderedText(std::string textTexture, SDL_Color textColor)
{
	free();

	SDL_Surface* loadedSurface = TTF_RenderTextSolid(gFont, textTexture.c_str(), textColor);

	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
}
#endif

void LTexture::free()
{
	SDL_DestroyTexture(mTexture);
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

void LTexture::setColor(Uint8 r, Uint8 g, Uint8 b)
{
	SDL_SetTextureColorMod(mTexture, r, g, b);
}

void LTexture::setAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::setBlend(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;

	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}

void LButton::handleEvent(SDL_Event* e)
{
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);

		bool inside = true;
		if (x < mPosition.x)
		{
			inside = false;
		}
		else if (x > mPosition.x + BUTTON_WIDTH)
		{
			inside = false;
		}
		else if (y < mPosition.y)
		{
			inside = false;
		}
		else if (y > mPosition.y + BUTTON_HEIGHT)
		{
			inside = false;
		}

		if (!inside)
		{
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		else
		{

			switch (e->type)
			{
			case SDL_MOUSEMOTION:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;

			case SDL_MOUSEBUTTONDOWN:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				break;

			case SDL_MOUSEBUTTONUP:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
				break;
			}
		}
	}
}

void LButton::render()
{
	gButtonSpriteSheetTexture.render(mPosition.x, mPosition.y, &gSpriteClip[mCurrentSprite]);
}


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
		if (!(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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
	bool success = true;

	if (!(gButtonSpriteSheetTexture.loadFromFile("Image\\Button\\button.png")))
	{
		printf("Failed to load button sprite texture!\n");
		success = false;
	}
	else
	{
		for (int i = 0; i < TOTAL_BUTTONS; ++i)
		{
			gSpriteClip[i].x = 0;
			gSpriteClip[i].y = i * 200;
			gSpriteClip[i].w = BUTTON_WIDTH;
			gSpriteClip[i].h = BUTTON_HEIGHT;
		}

		gButtons[0].setPosition(0, 0);
		gButtons[1].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, 0);
		gButtons[2].setPosition(0, SCREEN_HEIGHT - BUTTON_HEIGHT);
		gButtons[3].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT);
	}

	return success;
}

void close()
{
	gButtonSpriteSheetTexture.free();

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	SDL_Quit();

}

int main(int argc, char* arg[])
{

	if (!init())
	{
		printf("Failed to load media!\n");
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
					
					for (int i = 0; i < TOTAL_BUTTONS; ++i)
					{
						gButtons[i].handleEvent(&e);
					}
				}

				SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
				SDL_RenderClear(gRenderer);

				for (int i = 0; i < TOTAL_BUTTONS; ++i)
				{
					gButtons[i].render();
				}

				SDL_RenderPresent(gRenderer);
			}
		}
	}
	close();
	return 0;
}