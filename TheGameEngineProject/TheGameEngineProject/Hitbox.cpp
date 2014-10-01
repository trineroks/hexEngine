#include "StdAfx.h"
#include "Hitbox.h"


CHitbox::CHitbox(void)
{
	offsetX = 0;
	offsetY = 0;
	setBox(0, 0, 0, 0);
}

CHitbox::CHitbox(int p_offsetx, int p_offsety, int p_w, int p_h)
{
	offsetX = p_offsetx;
	offsetY = p_offsety;
	setBox(offsetX, offsetY, p_w, p_h);
}

CHitbox::~CHitbox(void)
{

}

void CHitbox::setBox(int x, int y, int w, int h)
{
	hitbox.x = x;
	hitbox.y = y;
	hitbox.w = w;
	hitbox.h = h;
}