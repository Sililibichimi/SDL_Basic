#include "SDL.h"
#include "SDL_image.h"
#include "stdio.h"
#include "string"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;

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
	int mWeight;
};	

bool init();
bool loadMedia();
bool loadTexture(std::string path);

SDL_Window* gWidow = NULL;

SDL_Texture* gTexture = NULL;

SDL_Renderer* gRenderer = NULL;

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

	}

	return success;
}
