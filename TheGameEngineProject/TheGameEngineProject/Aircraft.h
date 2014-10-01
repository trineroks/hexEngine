#pragma once
#include "Unit.h"
class CAircraft : public CUnit
{
public:
	CAircraft(int p_owner, string p_name, int p_moveRange, int p_optics, int p_hp, int defense, int armor, 
		CWeapon *primary, CWeapon *support, customPoint p_pos, CSDL_Setup *p_csdl_setup, SDL_Texture *p_texture, CEnvironment *p_environment);
	~CAircraft(void);
};

