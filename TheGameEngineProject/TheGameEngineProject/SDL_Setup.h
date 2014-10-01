#pragma once
class CSDL_Setup
{
public:
	CSDL_Setup(bool exit, int p_screenW, int p_screenH);
	~CSDL_Setup(void);

	SDL_Renderer* getRenderer() { return renderer; }
	SDL_Event* getMainEvent() { return mainevent; }
	Mix_Music* getMusic() { return music; }

	TTF_Font* getUnitLabel() { return unitLabel; }
	TTF_Font* getMenuLabel() { return menuLabel; }
	TTF_Font* getUnitLabelOutline() { return unitLabelOutline; }
	TTF_Font* getMenuLabelOutline() { return menuOutline; }

	SDL_Texture *getButtons() { return buttons_; }
	SDL_Texture *getTerrain() { return terrain_; }
	SDL_Texture *getUnits() { return units_; }
	SDL_Texture *getBackground() { return background_; }

	void start();
	void end();

	void useGameWorld();
	void useOnscreenMenu();
	void useFullscreen();

	int getWidth() { return screenW; }
	int getHeight() { return screenH; }

private:
	int screenW;
	int screenH;
	
	SDL_Rect gameWorld;
	SDL_Rect onScreenMenu;
	SDL_Rect fullscreen;

	TTF_Font *unitLabel;
	TTF_Font *unitLabelOutline;
	TTF_Font *menuLabel;
	TTF_Font *menuOutline;

	SDL_Texture *buttons_;
	SDL_Texture *terrain_;
	SDL_Texture *units_;
	SDL_Texture *background_;
	//TRYING OUT SDL_TTF
	/*
	SDL_Color color;
	SDL_Surface *surface;
	SDL_Texture *fontTexture;
	*/

	Mix_Music *music;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event* mainevent;
};

