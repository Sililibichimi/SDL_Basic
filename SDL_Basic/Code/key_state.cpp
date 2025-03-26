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

	bool loadFromFile(std::string);
#if defined(SDL_TTF_MAJOR)
	bool loadFromRenderedText(std::string textTexture, SDL_Color textColor);
#endif

	void free();
	void setColor(Uint8 r, Uint8 g, Uint8 b);
	void setBlend(SDL_BlendMode blending);
	void setAlpha(Uint8 alpha);

	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	int getWidth();
	int getHeight();

private:
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
};

