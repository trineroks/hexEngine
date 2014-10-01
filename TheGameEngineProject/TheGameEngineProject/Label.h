#pragma once
#include "stdafx.h"
class CLabel
{
public:
	CLabel(TTF_Font *p_font, TTF_Font *p_outline, SDL_Renderer *p_renderer);
	~CLabel(void);

	void setStringLabel(char p_string[]);
	void setLabelFromInt(int p_int);

	void setPosition(int p_x, int p_y);
	void setX(int p_x);
	void setY(int p_y);
	void setColor(int a, int r, int g, int b);

	void draw(float camX, float camY);
	void drawWithOutline(float camX, float camY);
	void drawSteadyLabel();
	void drawSteadyWithOutline();

	void setFloatAnimate(int speed);

	int getWidth() { return context_.w; };
	int getHeight() { return context_.h; };
	int getX() { return rect_.x; }
	int getY() { return rect_.y; }

private:
	char *label_;
	unsigned int animationSpeed_;

	TTF_Font *font_;
	TTF_Font *outline_;

	SDL_Renderer *renderer_;

	SDL_Surface *surface_;
	SDL_Surface *surfaceOutline_;

	SDL_Texture *fontTexture_;
	SDL_Texture *outlineTexture_;

	SDL_Color color_;
	SDL_Color colorOutline_;

	SDL_Rect contextOutline_;
	SDL_Rect context_;

	SDL_Rect rectOutline_;
	SDL_Rect rect_;
};

