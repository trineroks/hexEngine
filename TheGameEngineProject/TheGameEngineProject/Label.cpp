#include "StdAfx.h"
#include "Label.h"

CLabel::CLabel(TTF_Font *p_font, TTF_Font *p_outline, SDL_Renderer *p_renderer)
{
	font_ = p_font;
	outline_ = p_outline;

	renderer_ = p_renderer;
	rect_.x = 0;
	rect_.y = 0;
	rect_.h = 0;
	rect_.w = 0;

	animationSpeed_ = 0;

	context_.x = rect_.x;
	context_.y = rect_.y;
	context_.h = rect_.h;
	context_.w = rect_.w;

	color_.r = 255;
	color_.g = 255;
	color_.b = 255;

	if (outline_ != NULL)
		TTF_SetFontOutline(outline_, 2);

	rectOutline_.x = 0;
	rectOutline_.y = 0;
	rectOutline_.h = 0;
	rectOutline_.w = 0;

	contextOutline_.x = rectOutline_.x;
	contextOutline_.y = rectOutline_.y;
	contextOutline_.h = rectOutline_.h;
	contextOutline_.w = rectOutline_.w;

	colorOutline_.r = 0;
	colorOutline_.g = 0;
	colorOutline_.b = 0;
}


CLabel::~CLabel(void)
{
	SDL_FreeSurface(surface_);
	SDL_FreeSurface(surfaceOutline_);
	label_ = NULL;
	font_ = NULL;
	outline_ = NULL;
	renderer_ = NULL;
	surface_ = NULL;
	surfaceOutline_ = NULL;
	fontTexture_ = NULL;
	outlineTexture_ = NULL;
}

void CLabel::setStringLabel(char p_string[])
{
	label_ = p_string;
	surface_ = TTF_RenderText_Solid(font_, label_, color_);

	fontTexture_ = SDL_CreateTextureFromSurface(renderer_, surface_);

	SDL_QueryTexture(fontTexture_, NULL, NULL, &context_.w, &context_.h);

	if (outline_ != NULL)
	{
		surfaceOutline_ = TTF_RenderText_Solid(outline_, label_, colorOutline_);
		outlineTexture_ = SDL_CreateTextureFromSurface(renderer_, surfaceOutline_);
		SDL_QueryTexture(outlineTexture_, NULL, NULL, &contextOutline_.w, &contextOutline_.h);
	}
}

void CLabel::setColor(int a, int r, int g, int b)
{
	color_.a = a;
	color_.r = r;
	color_.g = g;
	color_.b = b;
}

void CLabel::setLabelFromInt(int p_int)
{
	char buffer[10];
	sprintf(buffer, "%d", p_int);
	label_ = buffer;
	surface_ = TTF_RenderText_Solid(font_, label_, color_);

	fontTexture_ = SDL_CreateTextureFromSurface(renderer_, surface_);

	SDL_QueryTexture(fontTexture_, NULL, NULL, &context_.w, &context_.h);
	if (outline_ != NULL)
	{
		surfaceOutline_ = TTF_RenderText_Solid(outline_, label_, colorOutline_);
		outlineTexture_ = SDL_CreateTextureFromSurface(renderer_, surfaceOutline_);
		SDL_QueryTexture(outlineTexture_, NULL, NULL, &contextOutline_.w, &contextOutline_.h);
	}
}

void CLabel::setPosition(int p_x, int p_y)
{
	rect_.x = p_x;
	rect_.y = p_y;
	rectOutline_.x = p_x - 2;
	rectOutline_.y = p_y - 2;
}

void CLabel::setX(int p_x)
{
	rect_.x = p_x;
	rectOutline_.x = p_x - 2;
}

void CLabel::setY(int p_y)
{
	rect_.y = p_y;
	rectOutline_.y = p_y - 2;
}

void CLabel::draw(float camX, float camY)
{
	context_.x = rect_.x - camX;
	context_.y = rect_.y - camY;
	SDL_RenderCopy(renderer_, fontTexture_, NULL, &context_);
}

void CLabel::drawWithOutline(float camX, float camY)
{
	if (outline_ == NULL)
	{
		printf("An outline does not exist!\n");
		return;
	}
	contextOutline_.x = rectOutline_.x - camX;
	contextOutline_.y = rectOutline_.y - camY;
	context_.x = rect_.x - camX;
	context_.y = rect_.y - camY;
	SDL_RenderCopy(renderer_, outlineTexture_, NULL, &contextOutline_);
	SDL_RenderCopy(renderer_, fontTexture_, NULL, &context_);
}

void CLabel::drawSteadyLabel()
{
	context_.x = rect_.x;
	context_.y = rect_.y;
	SDL_RenderCopy(renderer_, fontTexture_, NULL, &context_);
}

void CLabel::drawSteadyWithOutline()
{
	SDL_RenderCopy(renderer_, outlineTexture_, NULL, &contextOutline_);
	SDL_RenderCopy(renderer_, fontTexture_, NULL, &context_);
}

void CLabel::setFloatAnimate(int speed)
{
	if (animationSpeed_+speed < SDL_GetTicks())
	{
		rect_.y--;
		rectOutline_.y--;
		animationSpeed_ = SDL_GetTicks();
	}
}