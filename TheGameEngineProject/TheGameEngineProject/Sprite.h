#pragma once
#include "stdafx.h"
#include "Hitbox.h"

class CSprite
{
public:
	CSprite();
	CSprite(SDL_Renderer* p_renderer, SDL_Texture *p_image, int w, int h, int x, int y,
			SDL_Rect portionDimension, int sheetParamX, int sheetParamY);
	virtual ~CSprite(void);
	void initializeSprite(SDL_Renderer* p_renderer, SDL_Texture *p_image, int w, int h, int x, int y, 
						  SDL_Rect portionDimension, int sheetParamX, int sheetParamY);

	void drawSprite(float camX, float camY);

	void drawSteady();

	int getWidth() { return rect.w; }
	int getHeight() { return rect.h; }

	SDL_Texture *getImage() { return image; }

	void setWidth(int W);
	void setHeight(int H);

	void transformIntoCenter(int x, int y);
	void setPosition(int x, int y);

	void setX(int x);
	void setY(int y);

	customPoint getCenter() { return center; }
	void setSheetParams(int p_paramX, int p_paramY);

	void setAnimate(int startFrame, int endFrame, int row, float speed);
	void setAnimateManual(int row);

	void setWaveAnimate(int startFrame, int endFrame, int row, float speed);
	void cropImage(int frame, int row);

	//void test() {printf("This is a sprite\n");};

private:
	float* camX;
	float* camY;

	SDL_Rect camera;

	//float origin_X;
	//float origin_Y;

	customPoint center;

	//float X_pos;
	//float Y_pos;

	SDL_Texture* image;

	SDL_Rect rect;
	SDL_Rect unitPortion;
	SDL_Rect crop;

	bool loopBack;

	int img_width;
	int img_height;
	int currentFrame;
	unsigned int animationSpeed;

	int imagecountX;
	int imagecountY;

	SDL_Renderer* renderer;
};
