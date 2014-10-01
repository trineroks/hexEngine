#include "StdAfx.h"
#include "Unit.h"

int roundUp(int health);

CUnit::CUnit(int p_owner, string p_name, int p_unitType, int p_moveRange, int p_optics, int p_maxHP, int defense, int armor, 
	CWeapon *primary, CWeapon *support, customPoint p_pos, CSDL_Setup *p_csdl_setup, SDL_Texture *p_texture, CEnvironment *p_environment, bool transport)
{
	unitType_ = p_unitType;
	gameEnvironment_ = p_environment;
	unitSheet_ = p_texture;
	owner_ = p_owner;
	unitName_ = p_name;
	setup_ = p_csdl_setup;
	inDefaultState_ = true;

	primary_ = primary;
	support_ = support;

	primaryAmmo_ = primary->getMaxAmmo();
	supportAmmo_ = support->getMaxAmmo();

	animationSpeed_ = 0;

	optics_ = p_optics;
	armor_ = armor;
	defense_ = defense;

	position_.x = p_pos.x;
	position_.y = p_pos.y;

	previousPos_.x = p_pos.x;
	previousPos_.y = p_pos.y;

	moveRange_ = p_moveRange;
	maxHP_ = p_maxHP;
	currentHP_ = maxHP_;
	movedThisTurn_ = false;
	attackedThisTurn_ = false;
	selectedMove_ = false;
	selectedAttack_ = false;
	inFloaterState_ = false;

	numberFloater_ = NULL;
	label_ = NULL;

	if (moveRange_/MINIMUM_MOVE_COST > optics_)
		optics_ = moveRange_/MINIMUM_MOVE_COST;
}

CUnit::~CUnit(void)
{
	delete unitSprite_;
	if (label_)
		delete label_;
	if (numberFloater_)
		delete numberFloater_;
	numberFloater_ = NULL;
	unitSprite_ = NULL;
	setup_ = NULL;
	label_ = NULL;
	support_ = NULL;
	primary_ = NULL;
	fogTiles_.clear();
	openSteps_.clear();
	attackTiles_.clear();
	validAttackTiles_.clear();
}

void CUnit::setupSprite(SDL_Rect portion, CSDL_Setup *p_csdl_setup, int w, int h, int sheetParamX, int sheetParamY)
{
	if (w == NULL || h == NULL) //Use native size if either of these fields are undefined
	{
		imageCamera_.w = portion.w / sheetParamX;
		imageCamera_.h = portion.h / sheetParamY;
	}
	else
	{
		imageCamera_.w = w;
		imageCamera_.h = h;
	}
	CenterCoordinates(position_.x, position_.y);

	label_ = new CLabel(setup_->getUnitLabel(), setup_->getUnitLabelOutline(), setup_->getRenderer());
	label_->setPosition(imageCamera_.x-5, imageCamera_.y + HP_LABEL_POS);
	setHP(currentHP_);

	unitSprite_ = new CSprite(setup_->getRenderer(), unitSheet_, imageCamera_.w, imageCamera_.h, imageCamera_.x, imageCamera_.y, portion, sheetParamX, sheetParamY);
	//Each unit will have 4 frames of 2 types of animation: Idle and Attack.
	//The Idle frames are on top, Attack frames on bottom. Refer to unit spritesheet.
	unitSprite_->cropImage(0, 0);
}

customPoint CUnit::getPixelPosition()
{
	customPoint output;
	output.x = imageCamera_.x;
	output.y = imageCamera_.y;
	return output;
}

void CUnit::CenterCoordinates(int x, int y)
{
	imageCamera_.y = ((HEX_SIZE *3.0/4.0) * y) + (HEX_SIZE - imageCamera_.h)/2;
	if (position_.y % 2 == 0)
	{
		imageCamera_.x = (x * HEX_SIZE) + (HEX_SIZE - imageCamera_.w)/2;
	}
	else
	{
		imageCamera_.x = (x * HEX_SIZE) + (HEX_SIZE/2.0) + (HEX_SIZE - imageCamera_.w)/2;
	}
}

void CUnit::decrementAmmo(int weapon)
{
	switch(weapon)
	{
	case WEAPON_PRIMARY:
		if (primaryAmmo_ > 0)
			primaryAmmo_--;
		break;
	case WEAPON_SUPPORT:
		if (supportAmmo_ > 0)
			supportAmmo_--;
		break;
	default:
		printf("Invalid weapon\n");
	}
}

void CUnit::replenishAmmo()
{
	primaryAmmo_ = primary_->getMaxAmmo();
	supportAmmo_ = support_->getMaxAmmo();
}

void CUnit::setPos(int x, int y)
{
	position_.x = x;
	position_.y = y;
}

void CUnit::doCancel()
{
	moveTo(previousPos_.x, previousPos_.y);
	movedThisTurn_ = false;
}

void CUnit::moveTo(int x, int y)
{
	movedThisTurn_ = true;
	selectedMove_ = false;
	gameEnvironment_->getHex(position_.x, position_.y)->removeUnit();
	setPos(x, y);
	gameEnvironment_->getHex(x, y)->setUnit(this);
	CenterCoordinates(x, y);
	label_->setPosition(imageCamera_.x-5, imageCamera_.y + HP_LABEL_POS);
	unitSprite_->setPosition(imageCamera_.x, imageCamera_.y);
	hideUnitActionTiles();
	markPossibleAction(ACTION_ATTACK);
}

void CUnit::updateHPLabel()
{
	setHP(currentHP_);
}

void CUnit::setHP(int hp)
{
	if (hp >= maxHP_)
	{
		currentHP_ = maxHP_;
		label_->setColor(0, 50, 255, 255);
		label_->setLabelFromInt(currentHP_);
	}
	else
	{
		currentHP_ = hp;
		if (currentHP_ < maxHP_ && currentHP_ > (maxHP_/2.0))
		{
			label_->setColor(0, 50, 255, 50);
			label_->setLabelFromInt(currentHP_);
		}
		else if (currentHP_ <= (maxHP_/2.0) && currentHP_ > maxHP_/4.0)
		{
			label_->setColor(0, 200, 255, 0);
			label_->setLabelFromInt(currentHP_);
		}
		else
		{
			label_->setColor(0, 255, 30, 30);
			label_->setLabelFromInt(currentHP_);
		}
	}	
}

void CUnit::decrementHP(int dec)
{
	currentHP_ -= dec;
	//setHP(currentHP_);
}

void CUnit::incrementHP(int inc)
{
	currentHP_ += inc;
	//setHP(currentHP_);
}

void CUnit::activate()
{
	movedThisTurn_ = false;
	attackedThisTurn_ = false;
	hideUnitActionTiles();
	//setDefog();
}

void CUnit::startAttack()
{
	markPossibleAction(ACTION_ATTACK);
}

void CUnit::selectUnit()
{
	if (!movedThisTurn_)
	{
		selectedMove_ = true;
		printf("Selected %s at %d, %d\n", unitName_.c_str(), position_.x, position_.y);
		markPossibleAction(ACTION_MOVE);
		markPossibleAction(ACTION_ATTACK);
	}
	//printf("%s with %d ammo\n", primary_->getName(), primaryAmmo_);
	//printf("%s with %d ammo\n", support_->getName(), supportAmmo_);
}

void CUnit::deselectUnit()
{
	selectedMove_ = false;
	selectedAttack_ = false;
	printf("Deselected %s\n", unitName_.c_str());
	hideUnitActionTiles();
}

void CUnit::unmarkPossibleMovement()
{
	openSteps_.clear();
}

void CUnit::unmarkPossibleAttack()
{
	attackTiles_.clear();
	validAttackTiles_.clear();
	validSupportTiles_.clear();
}

void CUnit::markPossibleAction(int action)
{
	switch(action)
	{
	case ACTION_MOVE:
		getMovableTiles();
		break;
	case ACTION_ATTACK:
		if (movedThisTurn_) //We have two different conditions: 1 is after the unit's moved, and 2 is when the unit hasn't moved yet
		{
			if (primary_->getCanFireAfterMove())
				getTargetableTiles(primary_);
			if (support_->getCanFireAfterMove())
				getTargetableTiles(support_);
		}
		else
		{
			getTargetableTiles(primary_);
			getTargetableTiles(support_);
		}
		break;
	case ACTION_DEFEND:
		if (primary_->getCanFireAfterMove())
			getTargetableTiles(primary_);
		if (support_->getCanFireAfterMove())
			getTargetableTiles(support_);
		break;
	}
}

void CUnit::paintMovement(float camX, float camY)
{
	for (vector<tilePoint>::const_iterator i = openSteps_.begin()+1; i != openSteps_.end(); i++)
		gameEnvironment_->paintMovementTile((i)->x, (i)->y, camX, camY);
}

void CUnit::paintAttack(float camX, float camY)
{
	for (vector<customPoint>::const_iterator i = validAttackTiles_.begin(); i != validAttackTiles_.end(); i++)
		gameEnvironment_->paintTargetableTile((i)->x, (i)->y, camX, camY);
	for (vector<customPoint>::const_iterator i = validSupportTiles_.begin(); i != validSupportTiles_.end(); i++)
		gameEnvironment_->paintSupportableTile((i)->x, (i)->y, camX, camY);
}

bool CUnit::validMoveTile(int x, int y)
{
	for (vector<tilePoint>::const_iterator i = openSteps_.begin()+1; i != openSteps_.end(); i++)
	{
		if ((i)->x == x && (i)->y == y)
			return true;
	}
	return false;
}

void CUnit::hideUnitActionTiles()
{
	unmarkPossibleMovement();
	unmarkPossibleAttack();
}

void CUnit::endUnitActions()
{

	movedThisTurn_ = true;
	attackedThisTurn_ = true;
	previousPos_.x = position_.x;
	previousPos_.y = position_.y;
	hideUnitActionTiles();
	setDefog();
}

void CUnit::setDefog()
{
	fogTiles_.clear();
	customPoint adjacent[6];

	int layer = 0;

	gameEnvironment_->getHex(position_.x, position_.y)->setFog(false);

	bool skip = false;
	tilePoint insert;
	insert.x = position_.x;
	insert.y = position_.y;
	insert.cost = optics_;
	fogTiles_.push_back(insert);
	for (int i=0; i<fogTiles_.size(); i++)
	{
		gameEnvironment_->getAdjacentHexes(fogTiles_[i].x, fogTiles_[i].y, adjacent);
		for (int k=0; k<6; k++)
		{
			skip = false;
			if (adjacent[k].x == position_.x && adjacent[k].y == position_.y)
				skip = true;
			//if (layer > 0 && gameEnvironment_->blockedLineOfSight(position_.x, position_.y, adjacent[k].x, adjacent[k].y))
			//{
			//	printf("%d, %d TO %d, %d\n", position_.x, position_.y, adjacent[k].x, adjacent[k].y);
			//	skip = true;
			//}
			else
			{
				for (vector<tilePoint>::iterator j = fogTiles_.begin(); j != fogTiles_.end(); j++)
				{
					if ((j)->x == adjacent[k].x && (j)->y == adjacent[k].y)
					{
						skip = true;
						break;
					}
				}
				if (gameEnvironment_->isValidTile(adjacent[k].x, adjacent[k].y) && fogTiles_[i].cost > 0 && !skip) 
				{
					gameEnvironment_->getHex(adjacent[k].x, adjacent[k].y)->setFog(false);
					insert.x = adjacent[k].x;
					insert.y = adjacent[k].y;
					insert.cost = fogTiles_[i].cost - 1;
					fogTiles_.push_back(insert);
				}
			}
		}
		layer++;
	}
}

void CUnit::getMovableTiles()
{
	customPoint adjacent[6];
	bool skip;
	bool ZOC;
	tilePoint insert;
	insert.x = position_.x;
	insert.y = position_.y;
	insert.cost = moveRange_;
	openSteps_.push_back(insert);
	for (int k=0; k<openSteps_.size(); k++)
	{
		gameEnvironment_->getAdjacentHexes(openSteps_[k].x, openSteps_[k].y, adjacent);
		for (int i=0; i<6; i++)
		{
			skip = false;
			if (adjacent[i].x == position_.x && adjacent[i].y == position_.y)
				skip = true;
			if (gameEnvironment_->getHex(adjacent[i].x, adjacent[i].y)->isFogged()) //if the tile is fogged, don't add it into the movelist
				skip = true;
			else
			{
				for (vector<tilePoint>::iterator j = openSteps_.begin(); j != openSteps_.end(); j++)
				{
					if ((j)->x == adjacent[i].x && (j)->y == adjacent[i].y)
					{
						if (openSteps_[k].cost - gameEnvironment_->getMoveCost(adjacent[i].x, adjacent[i].y, unitType_) > (j)->cost)
						{
							insert.x = adjacent[i].x;
							insert.y = adjacent[i].y;
							customPoint testEnemyAdjacent[6];
							gameEnvironment_->getAdjacentHexes(adjacent[i].x, adjacent[i].y, testEnemyAdjacent);
							for (int y = 0; y<6; y++)
							{
								if (gameEnvironment_->anyEnemyOnTile(testEnemyAdjacent[y].x, testEnemyAdjacent[y].y, owner_) != NULL)
								{
									skip = true;
									break;
								}
							}
							if (!skip)
								(j)->cost = openSteps_[k].cost - gameEnvironment_->getMoveCost(adjacent[i].x, adjacent[i].y, unitType_);
						}
						skip = true;
						break;
					}
				}
				if (gameEnvironment_->traversable(adjacent[i].x, adjacent[i].y, openSteps_[k].cost, unitType_, owner_) && !skip)
				{
					ZOC = false;
					insert.x = adjacent[i].x;
					insert.y = adjacent[i].y;
					customPoint testEnemyAdjacent[6];
					gameEnvironment_->getAdjacentHexes(adjacent[i].x, adjacent[i].y, testEnemyAdjacent);
					for (int y=0; y<6; y++)
					{
						if (gameEnvironment_->anyEnemyOnTile(testEnemyAdjacent[y].x, testEnemyAdjacent[y].y, owner_) != NULL)
						{
							ZOC = true;
							break;
						}
					}
					if (ZOC == true)
						insert.cost = 0;
					else
						insert.cost = openSteps_[k].cost - gameEnvironment_->getMoveCost(adjacent[i].x, adjacent[i].y, unitType_);
					openSteps_.push_back(insert);
				}
			}
		}
	}
	return;
}

bool CUnit::getTargetableTiles(CWeapon *weapon)
{
	customPoint adjacent[6];
	bool skip = false;
	bool indirect = weapon->getIndirect();
	bool isSupport = weapon->getIsSupport();
	tilePoint insert;
	insert.x = position_.x;
	insert.y = position_.y;
	insert.cost = weapon->getRange();
	attackTiles_.push_back(insert);
	if (!isSupport && primaryAmmo_ <= 0) //checking primary to see if its ammo is at 0
	{
		//printf("No targetable enemies with Primary\n");
		attackTiles_.clear();
		return false;
	}
	if (isSupport && supportAmmo_ <= 0) //checking support to see if its ammo is at 0
	{
		//printf("No targetable enemies with Support\n");
		attackTiles_.clear();
		return false;
	}
	else
	{
		for (int i=0; i<attackTiles_.size(); i++)
		{
			gameEnvironment_->getAdjacentHexes(attackTiles_[i].x, attackTiles_[i].y, adjacent);
			for (int k=0; k<6; k++)
			{
				skip = false;
				if (adjacent[k].x == position_.x && adjacent[k].y == position_.y)
					skip = true;
				else
				{
					for (vector<tilePoint>::iterator j = attackTiles_.begin(); j != attackTiles_.end(); j++)
					{
						if ((j)->x == adjacent[k].x && (j)->y == adjacent[k].y)
						{
							skip = true;
							break;
						}
					}
					if (gameEnvironment_->isValidTile(adjacent[k].x, adjacent[k].y) && attackTiles_[i].cost > 0 && !skip) 
					{
						//Obstacles are only added into the range (max of 2) when the unit is adjacent to it.
						if (!indirect)
						{
							if (i == 0 && gameEnvironment_->getHex(adjacent[k].x, adjacent[k].y)->getIsObstacle())
							{
								insert.x = adjacent[k].x;
								insert.y = adjacent[k].y;
								insert.cost = 0;
								attackTiles_.push_back(insert);
							}
							else if (!gameEnvironment_->getHex(adjacent[k].x, adjacent[k].y)->getIsObstacle())
							{
								insert.x = adjacent[k].x;
								insert.y = adjacent[k].y;
								insert.cost = attackTiles_[i].cost - 1;
								attackTiles_.push_back(insert);
							}
						}
						else
						{
							insert.x = adjacent[k].x;
							insert.y = adjacent[k].y;
							insert.cost = attackTiles_[i].cost - 1;
							attackTiles_.push_back(insert);
						}
					}
				}
			}
		}
	}
	getValidAttackTiles(weapon, isSupport);
	return true;
}

void CUnit::getValidAttackTiles(CWeapon *weapon, bool isSupport)
{
	customPoint insert;
	int iterator = 0;
	for (vector<tilePoint>::iterator i = attackTiles_.begin()+1; i != attackTiles_.end(); i++)
	{
		insert.x = (i)->x;
		insert.y = (i)->y;
		if (gameEnvironment_->getHex(insert.x, insert.y)->isFogged() == false)
		{
			CUnit *enemy = gameEnvironment_->anyEnemyOnTile(insert.x, insert.y, owner_);
			if (enemy != NULL)
			{
				if (enemy->getUnitType() == UNIT_INFANTRY && weapon->getCanTarget().infantry)
				{
					if (isSupport)
						validSupportTiles_.push_back(insert);
					else	
						validAttackTiles_.push_back(insert);
				}
				else if (enemy->getUnitType() == UNIT_VEHICLE && weapon->getCanTarget().vehicle)
				{
					if (isSupport)
						validSupportTiles_.push_back(insert);
					else	
						validAttackTiles_.push_back(insert);
				}
				else if (enemy->getUnitType() == UNIT_AIRCRAFT && weapon->getCanTarget().air)
				{
					if (isSupport)
						validSupportTiles_.push_back(insert);
					else	
						validAttackTiles_.push_back(insert);
				}
			}
		}
	}
	attackTiles_.clear();
}

int CUnit::getWeaponToUse(int x, int y)
{
	for (vector<customPoint>::const_iterator i = validAttackTiles_.begin(); i != validAttackTiles_.end(); i++)
	{
		if ((i)->x == x && (i)->y == y)
			return WEAPON_PRIMARY;
	}
	for (vector<customPoint>::const_iterator i = validSupportTiles_.begin(); i != validSupportTiles_.end(); i++)
	{
		if ((i)->x == x && (i)->y == y)
			return WEAPON_SUPPORT;
	}
	return NONE;
}

int CUnit::calculateRawDamage(int weapon)
{
	switch(weapon)
	{
	//if the weapon used to attack is the unit's primary weapon..
	case WEAPON_PRIMARY:
		decrementAmmo(WEAPON_PRIMARY);
		if (primary_->getIsStrengthBased())
			return primary_->getDamage()*roundUp(currentHP_); //Each member in the squad (20 HP) outputs the weapon damage
		return primary_->getDamage();
	//if the weapon used to attack is NOT the unit's primary weapon (the support weapon)
	case WEAPON_SUPPORT:
		decrementAmmo(WEAPON_SUPPORT);
		if (support_->getIsStrengthBased())
			return support_->getDamage()*roundUp(currentHP_);
		return support_->getDamage();
	default:
		return 0;
	}
}

void CUnit::startCombatAnimation()
{
	animationSpeed_ = SDL_GetTicks();
	inDefaultState_ = false;
}

void CUnit::prepareDamageFloater(int damage)
{
	numberFloater_ = new CLabel(setup_->getUnitLabel(), setup_->getUnitLabelOutline(), setup_->getRenderer());
	numberFloater_->setPosition(imageCamera_.x + 60, imageCamera_.y);
	numberFloater_->setColor(0, 255, 30, 30);
	numberFloater_->setLabelFromInt(damage);
}

void CUnit::attackHandler(float camX, float camY)
{
	unitSprite_->setWaveAnimate(0, 3, 1, 150);
	unitSprite_->drawSprite(camX, camY);
}

void CUnit::numberFloaterHandler(float camX, float camY)
{
	numberFloater_->setFloatAnimate(120);
	numberFloater_->drawWithOutline(camX, camY);
}

void CUnit::deleteFloater()
{
	delete numberFloater_;
	numberFloater_ = NULL;
	inFloaterState_ = false;
}

void CUnit::drawSprite(float camX, float camY)
{
	if (inDefaultState_)
	{
		//unitSprite_->setWaveAnimate(0, 3, 0, 150);
		unitSprite_->setAnimateManual(0);
		unitSprite_->drawSprite(camX, camY);
		if (inFloaterState_)
			numberFloaterHandler(camX, camY);
	}
	else
		attackHandler(camX, camY);
	//label_->setFloatAnimate(150);
	label_->drawWithOutline(camX, camY);
	if (movedThisTurn_ && attackedThisTurn_)
		gameEnvironment_->paintEndTile(position_.x, position_.y, camX, camY);
}
//test
int CUnit::attackerInRangeForRetaliation(CUnit* attacker)
{
	int primaryDamage = 0;
	int supportDamage = 0;
	markPossibleAction(ACTION_DEFEND);
	for (vector<customPoint>::const_iterator i = validAttackTiles_.begin(); i != validAttackTiles_.end(); i++)
	{
		if (gameEnvironment_->getHex((i)->x, (i)->y)->getUnit() == attacker)
			primaryDamage = calculateRawDamage(WEAPON_PRIMARY);
	}
	for (vector<customPoint>::const_iterator i = validSupportTiles_.begin(); i != validSupportTiles_.end(); i++)
	{
		if (gameEnvironment_->getHex((i)->x, (i)->y)->getUnit() == attacker)
			supportDamage = calculateRawDamage(WEAPON_SUPPORT);
	}
	hideUnitActionTiles();
	if (primaryDamage != 0 || supportDamage != 0)
	{
		if (primaryDamage >= supportDamage)
			return WEAPON_PRIMARY;
		else
			return WEAPON_SUPPORT;
	}
	return NONE;
}

/*
int CUnit::attackerInRangeForRetaliation(CUnit* attacker)
{
	markPossibleAction(ACTION_DEFEND);
	for (vector<customPoint>::const_iterator i = validAttackTiles_.begin(); i != validAttackTiles_.end(); i++)
	{
		if (gameEnvironment_->getHex((i)->x, (i)->y)->getUnit() == attacker)
		{
			hideUnitActionTiles();
			return WEAPON_PRIMARY;
		}
	}
	for (vector<customPoint>::const_iterator i = validSupportTiles_.begin(); i != validSupportTiles_.end(); i++)
	{
		if (gameEnvironment_->getHex((i)->x, (i)->y)->getUnit() == attacker)
		{
			hideUnitActionTiles();
			return WEAPON_SUPPORT;
		}
	}
	hideUnitActionTiles();
	return NONE;
}
*/

int roundUp(int health)
{
	if (health%20 == 0)
		return health/20;
	else
		return (health + (20-(health%20)))/20;
}