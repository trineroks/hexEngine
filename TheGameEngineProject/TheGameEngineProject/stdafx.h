#pragma once

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <math.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <json/json.h>

#define HEX_SIZE 100
#define HP_LABEL_POS 50
#define SPRITE_SIZE 80
#define UNITSHEET_CROP_DIMENSIONS 82
#define MINIMUM_MOVE_COST 3
#define NON_PASSABLE 99
#define NONE 99
#define GAME_PIXEL_BUFFER 50
#define SPRITESHEET_DIMENSIONSX 500
#define SPRITESHEET_DIMENSIONSY 500
#define IMAGE_COUNTX 5
#define IMAGE_COUNTY 5
#define ONSCREEN_MENU_HEIGHT 75

#define GAME_DEFAULT_STATE 0 //When the game is just in hex selection/action box selection
#define GAME_COMBAT_STATE 1 

#define ACTION_BUTTON_HEIGHT 60
#define ACTION_MENU_WIDTH 250
#define ACTION_HEADER_HEIGHT 50
#define ACTION_FOOTER_HEIGHT 6

#define ACTION_MOVE 0
#define ACTION_STAY 1
#define ACTION_ATTACK 2
#define ACTION_CANCEL 3
#define ACTION_DISBAND 4
#define ACTION_ENDTURN 5
#define ACTION_DEFEND 6
#define ACTION_PRIMARY 7
#define ACTION_SUPPORT 8
#define ACTION_INFO 9

#define WEAPON_PRIMARY 0
#define WEAPON_SUPPORT 1

#define UNIT_INFANTRY 0
#define UNIT_VEHICLE 1
#define UNIT_AIRCRAFT 2

typedef enum GameState
{
	STATE_NULL = 0,
	STATE_MENU,
	STATE_GAME,
	STATE_EXIT
}GameState;

typedef enum UnitState
{
	UNIT_SELECTED = 0,
	UNIT_UNSELECTED
}UnitState;

typedef struct customPoint {
	int x;
	int y;
}customPoint;

typedef struct tilePoint {
	int x;
	int y;
	int cost;
}tilePoint;

typedef struct canTarget {
	bool infantry;
	bool vehicle;
	bool air;
}canTarget;

typedef struct moveCost
{
	int infantry;
	int vehicle;
	int air;
}moveCost;

using namespace std;