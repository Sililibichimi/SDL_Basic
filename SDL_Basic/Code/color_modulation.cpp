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

SDL_Window* gWindow = NULL;

SDL_Renderer* gRernder = NULL;

bool init();

bool loadMedia();

void close();

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

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

