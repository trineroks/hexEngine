#include "StdAfx.h"
#include "Sprite.h"

CSprite::CSprite()
{
	renderer = NULL;
	image = NULL;

	rect.x = 0;
	rect.y = 0;
	rect.w = 0;
	rect.h = 0;

	unitPortion.x = 0;
	unitPortion.y = 0;
	unitPortion.w = 0;
	unitPortion.h = 0;

	crop.x = 0;
	crop.y = 0;
	crop.w = 0;
	crop.h = 0;

	camera.x = 0;
	camera.y = 0;
	camera.w = 0;
	camera.h = 0;

	imagecountX = 0; 
	imagecountY = 0;
	currentFrame = 1;
	animationSpeed = 0;
	loopBack = false;
}

//YOU MUST NOW PASS IN AN IMAGE
CSprite::CSprite(SDL_Renderer* p_renderer, SDL_Texture* p_image, int w, int h, int x, int y, 
				SDL_Rect portionDimension, int sheetParamX, int sheetParamY)
{
	renderer = p_renderer;
	image = p_image;

	imagecountX = sheetParamX; 
	imagecountY = sheetParamY;

	//SDL_QueryTexture(image, NULL, NULL, &img_width, &img_height);

	unitPortion.x = portionDimension.x;
	unitPortion.y = portionDimension.y;
	unitPortion.w = portionDimension.w;
	unitPortion.h = portionDimension.h;

	crop.x = unitPortion.x;
	crop.y = unitPortion.y;
	crop.w = unitPortion.w;
	crop.h = unitPortion.h;

	currentFrame = 0;

	animationSpeed = 0;

	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;

	camera.x = rect.x;
	camera.y = rect.y;
	camera.w = rect.w;
	camera.h = rect.h;

	loopBack = false;
}

void CSprite::initializeSprite(SDL_Renderer* p_renderer, SDL_Texture *p_image, int w, int h, int x, int y, 
							 SDL_Rect portionDimension, int sheetParamX, int sheetParamY)
{
	renderer = p_renderer;
	image = p_image;

	imagecountX = sheetParamX; 
	imagecountY = sheetParamY;

	unitPortion.x = portionDimension.x;
	unitPortion.y = portionDimension.y;
	unitPortion.w = portionDimension.w;
	unitPortion.h = portionDimension.h;

	crop.x = unitPortion.x;
	crop.y = unitPortion.y;
	crop.w = unitPortion.w;
	crop.h = unitPortion.h;

	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;

	camera.x = rect.x;
	camera.y = rect.y;
	camera.w = rect.w;
	camera.h = rect.h;
}

void CSprite::setSheetParams(int p_imagecountX, int p_imagecountY)
{
	imagecountX = p_imagecountX;
	imagecountY = p_imagecountY;
}
//Cropping is now done locally given a certain space within a spritesheet. This allows for
//more flexible spritesheets, the only caveat being that all sprites within a given space
//must be equally spaced and equally sized
void CSprite::cropImage(int frame, int row)
{
	crop.x = unitPortion.x + frame * (unitPortion.w/imagecountX);
	crop.y = unitPortion.y + row * (unitPortion.h/imagecountY);
	crop.w = unitPortion.w/imagecountX;
	crop.h = unitPortion.h/imagecountY;
}

void CSprite::setWaveAnimate(int startFrame, int endFrame, int row, float speed) //Animations always go horizontal
{
	if (SDL_GetTicks() - animationSpeed > speed)
	{
		if (endFrame <= currentFrame)
		{
			currentFrame = endFrame;
			loopBack = true;
		}
		else if (currentFrame <= startFrame)
		{
			currentFrame = startFrame;
			loopBack = false;
		}
		if (loopBack)
			currentFrame--;
		else
			currentFrame++;
		crop.x = unitPortion.x + currentFrame * (unitPortion.w/imagecountX);
		crop.y = unitPortion.y + row * (unitPortion.h/imagecountY);
		crop.w = unitPortion.w/imagecountX;
		crop.h = unitPortion.h/imagecountY;

		animationSpeed = SDL_GetTicks();
	}
}

void CSprite::setAnimate(int startFrame, int endFrame, int row, float speed) //Animations always go horizontal
{
	if (SDL_GetTicks() - animationSpeed > speed)
	{
		if (endFrame <= currentFrame)
			currentFrame = startFrame;
		else
			currentFrame++;
		crop.x = unitPortion.x + currentFrame * (unitPortion.w/imagecountX);
		crop.y = unitPortion.y + row * (unitPortion.h/imagecountY);
		crop.w = unitPortion.w/imagecountX;
		crop.h = unitPortion.h/imagecountY;

		animationSpeed = SDL_GetTicks();
	}
}

void CSprite::setAnimateManual(int row)
{
	if (imagecountX - 1 <= currentFrame)
		currentFrame = 0;
	else
		currentFrame++;
	crop.x = unitPortion.x + currentFrame * (unitPortion.w / imagecountX);
	crop.y = unitPortion.y + row * (unitPortion.h / imagecountY);
	crop.w = unitPortion.w / imagecountX;
	crop.h = unitPortion.h / imagecountY;
}

CSprite::~CSprite(void)
{
	image = NULL;
}

void CSprite::drawSprite(float camX, float camY)
{
	camera.x = rect.x - camX;
	camera.y = rect.y - camY;

	SDL_RenderCopy(renderer, image, &crop, &camera);
}

void CSprite::drawSteady()
{
	SDL_RenderCopy(renderer, image, &crop, &rect);
}

void CSprite::transformIntoCenter(int x, int y)
{
	rect.y = ((HEX_SIZE *3.0/4.0) * y) + (HEX_SIZE - rect.h)/2;
	if (y % 2 == 0)
	{
		rect.x = (x * HEX_SIZE) + (HEX_SIZE - rect.w)/2;
	}
	else
	{
		rect.x = (x * HEX_SIZE) + (HEX_SIZE/2.0) + (HEX_SIZE - rect.w)/2;
	}
}

void CSprite::setWidth(int W)
{
	rect.w = W;
}

void CSprite::setHeight(int H)
{
	rect.h = H;
}

void CSprite::setPosition(int x, int y)
{
	rect.x = x;
	rect.y = y;
}

void CSprite::setX(int x)
{
	rect.x = x;
}

void CSprite::setY(int y)
{
	rect.y = y;
}