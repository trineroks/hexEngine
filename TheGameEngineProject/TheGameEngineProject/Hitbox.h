#pragma once
class CHitbox
{
public:
	CHitbox();
	CHitbox(int p_offsetx, int p_offsety, int p_w, int p_h);
	~CHitbox(void);

	void setBox(int x, int y, int w, int h);
	SDL_Rect getBox() { return hitbox; }

	void setX(int x) { hitbox.x = x + offsetX; }
	void setY(int y) { hitbox.y = y + offsetY; }

private:
	int offsetX;
	int offsetY;
	SDL_Rect hitbox;
};

