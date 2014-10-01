#pragma once
#include "stdafx.h"
class CWeapon
{
public:
	CWeapon(int p_ammo, int p_damage, int p_range, bool p_indirect, int p_armorPiercing, 
		bool canTargetInfantry, bool canTargetVehicle, bool canTargetAir, char p_name[], 
		bool p_strengthBased, int p_rearmCost, int p_minimumRange, bool p_canFireAfterMove, bool p_isSupport);
	~CWeapon(void);

	int getMaxAmmo() { return maxAmmo_; }
	int getDamage() { return damage_; }
	int getRearmCost() { return rearmCost_; }
	int getArmorPiercing() { return armorPiercing_; }
	char *getName() { return name_; }

	canTarget getCanTarget() { return canTarget_; }
	int getRange() { return range_; }

	bool getIndirect() { return indirect_; }
	bool getIsSupport() { return isSupport_; }
	bool getIsStrengthBased() { return isStrengthBased_; }
	bool getCanFireAfterMove() { return canFireAfterMove_; }
	int getMinimumRange() { return minimumRange_; }

private:
	int minimumRange_;
	int range_;
	int maxAmmo_;
	int damage_;
	int rearmCost_;
	int armorPiercing_;
	char* name_;
	bool indirect_;
	bool isSupport_;
	bool canFireAfterMove_;
	bool isStrengthBased_;
	canTarget canTarget_;
};

