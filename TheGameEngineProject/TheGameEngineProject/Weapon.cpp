#include "StdAfx.h"
#include "Weapon.h"


CWeapon::CWeapon(int p_ammo, int p_damage, int p_range, bool p_indirect, int p_armorPiercing, 
		bool canTargetInfantry, bool canTargetVehicle, bool canTargetAir, char p_name[], 
		bool p_strengthBased, int p_rearmCost, int p_minimumRange, bool p_canFireAfterMove, bool p_isSupport)
{
	maxAmmo_ = p_ammo;
	damage_ = p_damage;
	canFireAfterMove_ = p_canFireAfterMove;
	minimumRange_ = p_minimumRange;
	rearmCost_ = p_rearmCost;
	range_ = p_range;
	indirect_ = p_indirect;
	isSupport_ = p_isSupport;
	isStrengthBased_ = p_strengthBased;
	armorPiercing_ = p_armorPiercing;
	canTarget_.air = canTargetAir;
	canTarget_.infantry = canTargetInfantry;
	canTarget_.vehicle = canTargetVehicle;
	name_ = p_name;
}


CWeapon::~CWeapon(void)
{
	name_ = NULL;
}