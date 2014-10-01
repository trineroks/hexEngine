#pragma once
#include "stdafx.h"
#include "Sprite.h"
#include "Environment.h"
#include "TileData.h"
#include "Label.h"
#include "Weapon.h"

class CUnit
{
public:
	CUnit(int p_owner, string p_name, int p_unitType, int p_moveRange, int p_optics, int p_maxHP, int defense, int armor, 
		CWeapon *primary, CWeapon *support, customPoint p_pos, CSDL_Setup *p_csdl_setup, SDL_Texture *p_texture, CEnvironment *p_environment,
		bool transport);

	~CUnit(void);

	void drawSprite(float camX, float camY);

	void moveTo(int x, int y);
	void doCancel();

	void setSheetPortion(SDL_Rect portion);

	void setupSprite(SDL_Rect portion, CSDL_Setup *p_csdl_setup, int w, int h, int sheetParamX, int sheetParamY);

	void setHP(int hp);
	void decrementHP(int dec);
	void incrementHP(int inc);
	void updateHPLabel();
	int getHP() { return currentHP_; }

	void activate();
	void startAttack();
	void selectUnit();
	void deselectUnit();

	void endUnitActions();

	bool validMoveTile(int x, int y);
	int getWeaponToUse(int x, int y);

	bool selectedForMove() { return selectedMove_; }
	bool didMoveThisTurn() { return movedThisTurn_; }
	bool didAttackThisTurn() { return attackedThisTurn_; }

	int getMoveRange() { return moveRange_; }
	int getUnitType() { return unitType_; }
	int getOwner() { return owner_; }

	customPoint getPosition() { return position_; }
	customPoint getPixelPosition();

	void paintMovement(float camX, float camY);
	void paintAttack(float camX, float camY);
	void setDefog();

	void decrementAmmo(int weapon);
	void replenishAmmo();

	void startCombatAnimation();
	void prepareDamageFloater(int damage);

	void numberFloaterEnder();
	void numberFloaterHandler(float camX, float camY);
	void deleteFloater();

	int attackerInRangeForRetaliation(CUnit* attacker);

	int calculateRawDamage(int weapon);

	CWeapon *getPrimary() { return primary_; }
	CWeapon *getSupport() { return support_; }

	void hideUnitActionTiles();

	void setFloaterState(bool state) { inFloaterState_ = state; }
	void setDefaultState(bool state) { inDefaultState_ = state; }
	bool getFloaterState() { return inFloaterState_; }

	string getUnitName() { return unitName_; }

private:
	void markPossibleAction(int action);
	void attackHandler(float camX, float camY);

	void setPos(int x, int y);
	void getMovableTiles();

	//getTargetableTiles can easily be converted to fog of war
	bool getTargetableTiles(CWeapon *weapon);
	void getValidAttackTiles(CWeapon *weapon, bool isSupport);

	void unmarkPossibleMovement();
	void unmarkPossibleAttack();
	void CenterCoordinates(int x, int y);

	CWeapon *primary_;
	CWeapon *support_;

	int primaryAmmo_;
	int supportAmmo_;

	vector<tilePoint> openSteps_;
	vector<tilePoint> attackTiles_;
	vector<tilePoint> fogTiles_;
	vector<customPoint> validAttackTiles_;
	vector<customPoint> validSupportTiles_;

	CSDL_Setup *setup_;
	CEnvironment *gameEnvironment_;
	CLabel *numberFloater_; //This is the little number that floats over a unit when it heals/gets hit

	bool movedThisTurn_;
	bool attackedThisTurn_;
	bool selectedMove_;
	bool selectedAttack_;
	bool inDefaultState_;
	bool inFloaterState_;

	customPoint position_;
	customPoint previousPos_;

	string unitName_;

	SDL_Texture *unitSheet_;
	CSprite *unitSprite_;
	CLabel *label_;

	SDL_Rect imageCamera_;

	int owner_;
	int unitType_;
	int moveRange_;
	int attackRange_;
	int maxHP_;
	int currentHP_;
	int animationSpeed_;

	int optics_;
	int defense_;
	int armor_;
};
