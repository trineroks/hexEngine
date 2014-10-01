#include "StdAfx.h"
#include "Vehicle.h"


CVehicle::CVehicle(int p_owner, string p_name, int p_moveRange, int p_optics, int p_hp, int defense, int armor, int chassisType,
	   CWeapon *primary, CWeapon *support, customPoint p_pos, CSDL_Setup *p_csdl_setup, SDL_Texture *p_texture, CEnvironment *p_environment)
			 	   : CUnit(p_owner, p_name, UNIT_VEHICLE, p_moveRange, p_optics, p_hp, defense, armor, primary, support, p_pos, p_csdl_setup, p_texture, p_environment, false)
{
	SDL_Rect portion;
	if (p_owner == 0)
	{
		if (chassisType == 0)
		{
			portion.x = 0;
			portion.y = 320;
			portion.w = 320;
			portion.h = 160;
		}
		else
		{
			portion.x = 0;
			portion.y = 480;
			portion.w = 320;
			portion.h = 160;
		}
	}
	else
	{
		if (chassisType == 0)
		{
			portion.x = 320;
			portion.y = 320;
			portion.w = 320;
			portion.h = 160;
		}
		else
		{
			portion.x = 320;
			portion.y = 480;
			portion.w = 320;
			portion.h = 160;
		}
	}
	setupSprite(portion, p_csdl_setup, NULL, NULL, 4, 2);
}


CVehicle::~CVehicle(void)
{
}
