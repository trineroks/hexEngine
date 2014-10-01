#include "StdAfx.h"
#include "Environment.h"
#include "Unit.h"
#include "Vehicle.h"
#include "Infantry.h"
#include "Aircraft.h"
#include "Weapon.h"

int getSign(int arg);

CEnvironment::CEnvironment(int *board, int playerNum, CPlayer *p_players, int p_boardW, int p_boardH, CSDL_Setup* p_SDL_setup, bool *p_exit)
{
	selectionMade_ = false;
	actionMenuOpened_ = false;

	exit_ = p_exit;

	animationLimit_ = 0;

	attackerHex_ = NULL;
	defenderHex_ = NULL;

	isInCombat_ = false;
	isInNumberFloater_ = false;

	numberOfPlayers = playerNum;

	csdl_setup = p_SDL_setup;
	renderer = p_SDL_setup->getRenderer();

	actionMenu_ = NULL;

	currentlySelectedUnit_ = NULL;

	currentPlayer = 0;

	terrain = NULL;
	terrain = csdl_setup->getTerrain();

	GUI = NULL;
	GUI = csdl_setup->getButtons();

	units = NULL;
	units = csdl_setup->getUnits();

	backgroundTexture = NULL;
	backgroundTexture = csdl_setup->getBackground();

	sheetParams.x = 5;
	sheetParams.y = 5;

	moveHighlight_.x = 3 * HEX_SIZE; //The movehighlight hex is on (3, 4) of the spritesheet
	moveHighlight_.y = 4 * HEX_SIZE;
	moveHighlight_.w = HEX_SIZE;
	moveHighlight_.h = HEX_SIZE;

	attackHighlight_.x = 1 * HEX_SIZE; //The attackhighlight hex is on (3, 4) of the spritesheet
	attackHighlight_.y = 4 * HEX_SIZE;
	attackHighlight_.w = HEX_SIZE;
	attackHighlight_.h = HEX_SIZE;

	supportHighlight_.x = 0 * HEX_SIZE; //The supporthighlight hex is on (3, 4) of the spritesheet
	supportHighlight_.y = 4 * HEX_SIZE;
	supportHighlight_.w = HEX_SIZE;
	supportHighlight_.h = HEX_SIZE;

	endHighlight_.x = 2 * HEX_SIZE; //endHighlight is also going to be used for fog of war, because they're both the same sprite.
	endHighlight_.y = 4 * HEX_SIZE;
	endHighlight_.w = HEX_SIZE;
	endHighlight_.h = HEX_SIZE;

	onScreenButtonAssets_.x = 200;
	onScreenButtonAssets_.y = 0;
	onScreenButtonAssets_.w = 150;
	onScreenButtonAssets_.h = 150;

	actionMenuAssets_.x = 0; //This is where the actionMenuAssets are located
	actionMenuAssets_.y = 50;
	actionMenuAssets_.w = 200;
	actionMenuAssets_.h = 120;

	tileHeight_ = HEX_SIZE *3.0/4.0;
	upperThird_ = HEX_SIZE/4.0;
	halfHex_ = HEX_SIZE/2.0;

	height = p_boardH;
	width = p_boardW;

	for (int i=0; i<height*width; i++)
		localBoard[i] = board[i];

	/*
	for (int i = 0; i < numberOfPlayers; i++)
		players_[i] = p_players[i];
	*/

	mapSize = height*width;

	hexes_ = new CTileData[mapSize];

	calculatePixelDimensions(HEX_SIZE * 3.0/4.0);

	crop_.w = HEX_SIZE;
	crop_.h = HEX_SIZE;

	int x = 0;
	int y = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			y = tileHeight_*i;
			if (i % 2 == 0)
				x = j*HEX_SIZE;
			else
				x = j*HEX_SIZE + halfHex_;
			switch (localBoard[j + width*i])
			{
			case 1:
				//SAVE THESE VALUES TO FILE
				crop_.x = 0 * HEX_SIZE;
				crop_.y = 0 * HEX_SIZE;
				hexes_[j + width*i].initializeValues("DIRT", MINIMUM_MOVE_COST, MINIMUM_MOVE_COST, MINIMUM_MOVE_COST, 0, false, terrain, &crop_, x, y, renderer);
				break;
			case 2:
				crop_.x = 1 * HEX_SIZE;
				crop_.y = 0 * HEX_SIZE;
				hexes_[j + width*i].initializeValues("MOUNTAIN", MINIMUM_MOVE_COST, 5, NON_PASSABLE, 4, true, terrain, &crop_, x, y, renderer);
				break;
			case 3:
				crop_.x = 2 * HEX_SIZE;
				crop_.y = 0 * HEX_SIZE;
				hexes_[j + width*i].initializeValues("WATER", MINIMUM_MOVE_COST, NON_PASSABLE, NON_PASSABLE, 0, false, terrain, &crop_, x, y, renderer);
				break;
			default:
				crop_.x = 4 * HEX_SIZE;
				crop_.y = 4 * HEX_SIZE;
				hexes_[j + width*i].initializeValues("NULL", NON_PASSABLE, NON_PASSABLE, NON_PASSABLE, 0, false, terrain, &crop_, x, y, renderer);
				break;
			}
		}
	}

	SDL_Rect cursorSpritesOnSheetLocation;
	cursorSpritesOnSheetLocation.x = 0;
	cursorSpritesOnSheetLocation.y = 300;
	cursorSpritesOnSheetLocation.w = 500;
	cursorSpritesOnSheetLocation.h = HEX_SIZE;
	cursor = new CSprite(renderer, terrain, HEX_SIZE, HEX_SIZE, pixelWidth, pixelHeight, cursorSpritesOnSheetLocation, 5, 1);
	//cursor->setSheetParams(5, 1);
	cursor->cropImage(0, 0);
	
	CWeapon *nothing = new CWeapon(0, 0, 0, false, 0, false, false, false, "No Weapon", false, 0, 0, false, true);

	CWeapon *primary = new CWeapon(10, 8, 1, false, 0, true, false, false, "M16A2", true, 30, 0, true, false);
	CWeapon *support = new CWeapon(10, 50, 2, true, 2, false, false, true, "LAAD Stinger-C", false, 50, 0, false, true);

	//CWeapon *Sovietprimary = new CWeapon(10, 11, 1, false, 0, true, false, false, "AK-74", true, 30, 0, true, false);
	//CWeapon *Sovietsupport = new CWeapon(10, 45, 2, true, 2, false, false, true, "LAAD Igla-1M", false, 50, 0, true, true);

	CWeapon *gunshipprimary = new CWeapon(10, 40, 1, false, 0, true, false, true, "Yak-B", false, 30, 0, true, false);
	CWeapon *gunshipsupport = new CWeapon(10, 60, 2, true, 5, false, true, false, "9M114 Kokon", false, 50, 0, true, true);

	CWeapon *tankprimary = new CWeapon(10, 50, 1, false, 5, true, true, false, "2A46M", false, 50, 0, true, false);
	CWeapon *artilleryprimary = new CWeapon(10, 40, 5, true, 0, true, true, false, "115mm Howitzer", false, 50, 2, false, false);

	CWeapon *artillerysupport = new CWeapon(10, 20, 1, false, 0, true, false, false, ".50 HMG", false, 20, 0, true, true);

	customPoint Upos;
	Upos.x = 7;
	Upos.y = 10;
	hexes_[Upos.x + width*Upos.y].setUnit(new CInfantry(0, "US Riflemen", 6, 5, 100, 4, 0, primary, support, Upos, csdl_setup, units, this));
	/*
	Upos.x = 6;
	Upos.y = 10;
	hexes_[Upos.x + width*Upos.y].setUnit(new CInfantry(0, "US Riflemen", 6, 2, 100, 4, 0, primary, support, Upos, csdl_setup, units, this));
	Upos.x = 6;
	Upos.y = 9;
	hexes_[Upos.x + width*Upos.y].setUnit(new CInfantry(0, "US Riflemen", 6, 2, 100, 4, 0, primary, support, Upos, csdl_setup, units, this));
	Upos.x = 7;
	Upos.y = 10;
	hexes_[Upos.x + width*Upos.y].setUnit(new CVehicle(0, "M1A2 Abrams", 9, 2, 100, 4, 0, 0, tankprimary, nothing, Upos, csdl_setup, units, this));
	Upos.x = 8;
	Upos.y = 10;
	hexes_[Upos.x + width*Upos.y].setUnit(new CVehicle(0, "M1A2 Abrams", 9, 2, 100, 4, 0, 0, tankprimary, nothing, Upos, csdl_setup, units, this));
	Upos.x = 7;
	Upos.y = 11;
	hexes_[Upos.x + width*Upos.y].setUnit(new CVehicle(0, "M1A2 Abrams", 9, 2, 100, 4, 0, 0, tankprimary, nothing, Upos, csdl_setup, units, this));
	Upos.x = 8;
	Upos.y = 11;
	hexes_[Upos.x + width*Upos.y].setUnit(new CVehicle(0, "Paladin", 6, 2, 100, 4, 0, 1, artilleryprimary, artillerysupport, Upos, csdl_setup, units, this));
	Upos.x = 6;
	Upos.y = 11;
	hexes_[Upos.x + width*Upos.y].setUnit(new CAircraft(0, "AH-64 Apache", 12, 4, 100, 4, 0, gunshipprimary, gunshipsupport, Upos, csdl_setup, units, this));
	Upos.x = 8;
	Upos.y = 8;
	hexes_[Upos.x + width*Upos.y].setUnit(new CAircraft(0, "AH-64 Apache", 12, 4, 100, 4, 0, gunshipprimary, gunshipsupport, Upos, csdl_setup, units, this));
	*/

	Upos.x = 6;
	Upos.y = 4;
	hexes_[Upos.x + width*Upos.y].setUnit(new CInfantry(1, "Soviet Riflemen", 6, 5, 100, 4, 0, primary, support, Upos, csdl_setup, units, this));
	/*
	Upos.x = 7;
	Upos.y = 4;
	hexes_[Upos.x + width*Upos.y].setUnit(new CInfantry(1, "Soviet Riflemen", 6, 2, 100, 4, 0, primary, support, Upos, csdl_setup, units, this));
	Upos.x = 7;
	Upos.y = 3;
	hexes_[Upos.x + width*Upos.y].setUnit(new CInfantry(1, "Soviet Riflemen", 6, 2, 100, 4, 0, primary, support, Upos, csdl_setup, units, this));
	Upos.x = 6;
	Upos.y = 3;
	hexes_[Upos.x + width*Upos.y].setUnit(new CVehicle(1, "T-90A", 9, 2, 100, 4, 0, 0, tankprimary, nothing, Upos, csdl_setup, units, this));
	Upos.x = 6;
	Upos.y = 2;
	hexes_[Upos.x + width*Upos.y].setUnit(new CVehicle(1, "T-90A", 9, 2, 100, 4, 0, 0, tankprimary, nothing, Upos, csdl_setup, units, this));
	Upos.x = 5;
	Upos.y = 3;
	hexes_[Upos.x + width*Upos.y].setUnit(new CVehicle(1, "T-90A", 9, 2, 100, 4, 0, 0, tankprimary, nothing, Upos, csdl_setup, units, this));
	Upos.x = 5;
	Upos.y = 2;
	hexes_[Upos.x + width*Upos.y].setUnit(new CVehicle(1, "MSTA", 6, 2, 100, 4, 0, 1, artilleryprimary, artillerysupport, Upos, csdl_setup, units, this));
	Upos.x = 5;
	Upos.y = 5;
	hexes_[Upos.x + width*Upos.y].setUnit(new CAircraft(1, "Ka-50 Akula", 12, 4, 100, 4, 0, gunshipprimary, gunshipsupport, Upos, csdl_setup, units, this));
	Upos.x = 7;
	Upos.y = 2;
	hexes_[Upos.x + width*Upos.y].setUnit(new CAircraft(1, "Ka-50 Akula", 12, 4, 100, 4, 0, gunshipprimary, gunshipsupport, Upos, csdl_setup, units, this));
	*/
	/*
	int increment = 0;
	for (int i=1; i<height-2; i++)
	{
		for (int k=1; k<width-2; k++)
		{
			Upos.x = k;
			Upos.y = i;
			hexes_[Upos.x + width*Upos.y].setUnit(new CInfantry(0, "US Riflemen", 6, 100, 4, 0, primary, support, Upos, csdl_setup, units, this));
		}
	}
	*/

	endTurnButton_ = new CGUI(csdl_setup, csdl_setup->getWidth()-150, csdl_setup->getHeight()-75, 150, 75, onScreenButtonAssets_, 1, 2, "End Turn", ACTION_ENDTURN);

	setFog();
}

CEnvironment::~CEnvironment(void)
{
	//Is this necessary? Who knows. You probably do need to iterate through all elements of an array and delete those first.
	for (int i=0; i<mapSize; i++)
	{
		delete &hexes_[i];
	}
	delete[] hexes_;
	delete actionMenu_;
	delete endTurnButton_;
	delete actionMenu_;
	delete cursor;
	hexes_ = NULL;
	GUI = NULL;
	currentlySelectedUnit_ = NULL;
	csdl_setup = NULL;
	actionMenu_ = NULL;
	cursor = NULL;
	endTurnButton_ = NULL;
	renderer = NULL;
	attackerHex_ = NULL;
	defenderHex_ = NULL;
}

void CEnvironment::calculatePixelDimensions(int y)
{
	pixelHeight = (y * height) + (HEX_SIZE/4.0) + GAME_PIXEL_BUFFER*2;
	pixelWidth = (HEX_SIZE * width) + GAME_PIXEL_BUFFER*2;

	//For smaller maps or higher resolution screens, we want to fill the remaining blank space with the filler background
	if (pixelHeight < csdl_setup->getHeight())
		pixelHeight = csdl_setup->getHeight();
	if (pixelWidth < csdl_setup->getWidth())
		pixelWidth = csdl_setup->getWidth();
}

void CEnvironment::getAdjacentHexes(int x, int y, customPoint adjacent[])
{
	for (int i=0; i<6; i++)
	{
		adjacent[i].x = x;
		adjacent[i].y = y;
	}
	adjacent[0].x--;
	if (y % 2 == 0)
	{
		adjacent[1].x--;
		adjacent[1].y--;
		adjacent[2].y--;
		adjacent[4].y++;
		adjacent[5].x--;
		adjacent[5].y++;
	}
	else
	{
		adjacent[1].y--;
		adjacent[2].x++;
		adjacent[2].y--;
		adjacent[4].x++;
		adjacent[4].y++;
		adjacent[5].y++;
	}
	adjacent[3].x++;
}

void CEnvironment::drawTopLayer(float camX, float camY)
{
	cursor->setWaveAnimate(0, 4, 0, 120);
	cursor->drawSprite(camX, camY);
	paintFog(camX, camY);
	for (int i=0; i<mapSize; i++)
	{
		if (hexes_[i].existsUnit() && hexes_[i].getUnit()->getOwner() == currentPlayer)
			hexes_[i].getUnit()->drawSprite(camX, camY);
		else if (existsEnemyAt(i) && hexes_[i].isFogged() == false)
			hexes_[i].getUnit()->drawSprite(camX, camY);
	}
	if (actionMenu_ != NULL)
		actionMenu_->draw();
}

bool CEnvironment::paintMovementTile(int x, int y, float camX, float camY)
{
	imageCamera_.w = HEX_SIZE;
	imageCamera_.h = HEX_SIZE;
	imageCamera_.y = (tileHeight_*y) - camY;
	if (y % 2 == 0)
	{
		imageCamera_.x = (x*HEX_SIZE) - camX;
	}
	else
	{
		imageCamera_.x = (x*HEX_SIZE) + halfHex_ - camX;
	}
	SDL_RenderCopy(renderer, terrain, &moveHighlight_, &imageCamera_);
	return true;
}

bool CEnvironment::paintTargetableTile(int x, int y, float camX, float camY)
{
	imageCamera_.w = HEX_SIZE;
	imageCamera_.h = HEX_SIZE;
	imageCamera_.y = (tileHeight_*y) - camY;
	if (y % 2 == 0)
	{
		imageCamera_.x = (x*HEX_SIZE) - camX;
	}
	else
	{
		imageCamera_.x = (x*HEX_SIZE) + halfHex_ - camX;
	}
	SDL_RenderCopy(renderer, terrain, &attackHighlight_, &imageCamera_);
	return true;
}

bool CEnvironment::paintSupportableTile(int x, int y, float camX, float camY)
{
	imageCamera_.w = HEX_SIZE;
	imageCamera_.h = HEX_SIZE;
	imageCamera_.y = (tileHeight_*y) - camY;
	if (y % 2 == 0)
	{
		imageCamera_.x = (x*HEX_SIZE) - camX;
	}
	else
	{
		imageCamera_.x = (x*HEX_SIZE) + halfHex_ - camX;
	}
	SDL_RenderCopy(renderer, terrain, &supportHighlight_, &imageCamera_);
	return true;
}

void CEnvironment::paintEndTile(int x, int y, float camX, float camY)
{
	imageCamera_.w = HEX_SIZE;
	imageCamera_.h = HEX_SIZE;
	imageCamera_.y = (tileHeight_*y) - camY;
	if (y % 2 == 0)
	{
		imageCamera_.x = (x*HEX_SIZE) - camX;
	}
	else
	{
		imageCamera_.x = (x*HEX_SIZE) + halfHex_ - camX;
	}
	SDL_RenderCopy(renderer, terrain, &endHighlight_, &imageCamera_);
}

bool CEnvironment::isValidTile(int x, int y)
{
	if (hexes_[x + width*y].getType() == "NULL")
		return false;
	else
		return (x < width-1 && x > 0 && y < height-1 && y > 0);
}

void CEnvironment::draw(float camX, float camY)
{

	imageCamera_.w = pixelWidth;
	imageCamera_.h = pixelHeight;
	imageCamera_.x = 0 - camX;
	imageCamera_.y = 0 - camY;

	SDL_RenderCopy(renderer, backgroundTexture, NULL, &imageCamera_); //This is the underlying NULL background texture

	for (int i = 0; i < mapSize; i++)
		hexes_[i].drawHex(camX, camY);
}

bool CEnvironment::selected(float camX, float camY, int mouseX, int mouseY, int &x_pos, int &y_pos)
{
	customPoint gameWorldMouse;
	gameWorldMouse.x = mouseX + camX;
	gameWorldMouse.y = mouseY + camY;
	//Once we apply these camera values to gameWorldMouse, it is now within the game world. 
	gameWorldMouse = hitTest(gameWorldMouse);
	if (isValidTile(gameWorldMouse.x, gameWorldMouse.y))
	{
		//hexes_[gameWorldMouse.x + width*gameWorldMouse.y].setSelected(true);
		//cout << hexes_[gameWorldMouse.x + width*gameWorldMouse.y].getType() << endl;
		if (!unitHasMovedException())
			setCursorPosition(gameWorldMouse.x, gameWorldMouse.y);
		x_pos = gameWorldMouse.x;
		y_pos = gameWorldMouse.y;
		actUponSelect(x_pos, y_pos, camX, camY);
		return true;
	}
	return false;
}

bool CEnvironment::unitHasMovedException()
{
	if (currentlySelectedUnit_ != NULL && currentlySelectedUnit_->didMoveThisTurn())
		return true;
	return false;
}

void CEnvironment::setCursorPosition(int x, int y)
{
	if (y % 2 == 0)
		cursor->setPosition(x*HEX_SIZE, (tileHeight_)*y);
	else
		cursor->setPosition(x*HEX_SIZE+halfHex_, (tileHeight_)*y);
}

void CEnvironment::selectedStateToggle(customPoint gameMouse)
{
	if (actionMenu_ == NULL)
		return;
	else
		actionMenu_->toggleButtonState(gameMouse);
}

bool CEnvironment::selectedAction(int mouseX, int mouseY)
{
	customPoint screenMouse;
	screenMouse.x = mouseX;
	screenMouse.y = mouseY;
	int action = actionMenu_->getButtonPressed(screenMouse);
	if (currentlySelectedUnit_ != NULL) //Just a catcher for some out of the world exception
		doAction(action);
	return true;
}

void CEnvironment::actUponSelect(int x, int y, float camX, float camY)
{
	int index = x + width*y;
	//If a unit is selected and a player selects a valid attack tile from the unit's attack options
	if (currentlySelectedUnit_ != NULL && currentlySelectedUnit_->getWeaponToUse(x, y) != NONE)
		doCombat(getHex(currentlySelectedUnit_->getPosition().x, currentlySelectedUnit_->getPosition().y), &hexes_[index], currentlySelectedUnit_->getWeaponToUse(x, y));
	//If there exists a friendly unit on the hex the player selected
	else if (hexes_[index].existsUnit() && hexes_[index].getUnit()->getOwner() == currentPlayer)
	{
		//If no unit is selected...
		if (currentlySelectedUnit_ == NULL) 
		{
			//...and the unit at the tile has not moved yet (if the unit didn't end its turn)
			if (!hexes_[index].getUnit()->didMoveThisTurn())
			{
				currentlySelectedUnit_ = hexes_[index].getUnit();					//Set the currentUnit pointer to this unit
				currentlySelectedUnit_->selectUnit();								//Initialize unit for turn
				showActionMenu(false, csdl_setup->getWidth()-ACTION_MENU_WIDTH, 0); //Show the action menu
			}
			//...and it already moved, then we do nothing. We don't select a unit that's done its moves.
		}
		//If there already is a unit selected...
		else
		{
			//...and the unit didn't move that turn (if the unit didn't end its turn)
			if (!currentlySelectedUnit_->didMoveThisTurn())
			{
				doAction(ACTION_CANCEL);												//Deselect the previous unit
				if(!hexes_[index].getUnit()->didMoveThisTurn())							//If the unit at the hex did not move that turn
				{
					currentlySelectedUnit_ = hexes_[index].getUnit();					//Set the currentUnit pointer to the new unit
					currentlySelectedUnit_->selectUnit();								//Initialize unit for turn 
					showActionMenu(false, csdl_setup->getWidth()-ACTION_MENU_WIDTH, 0); //Show the action menu
				}
			}
			//...and the unit did move that turn...
			else if (currentlySelectedUnit_->didMoveThisTurn())
			{
				//...and it happens to be the unit you moved earlier...
				if (hexes_[index].getUnit() == currentlySelectedUnit_)
					doAction(ACTION_STAY); //do stay
			}
			//...and the unit already moved, we do nothing. Don't select another friendly unit while it's still in move state.
		}
	}
	//If there is already a unit selected and the unit hasn't moved yet
	else if (currentlySelectedUnit_ != NULL && !currentlySelectedUnit_->didMoveThisTurn())
	{
		//If the tile selected is a valid move tile and there exists no unit on that tile
		if (currentlySelectedUnit_->validMoveTile(x, y) && !hexes_[x + width*y].existsUnit())
		{
			printf("%s moving to %d, %d\n", currentlySelectedUnit_->getUnitName().c_str(), x, y);
			currentlySelectedUnit_->moveTo(x, y);								//Move the unit to that tile
			showActionMenu(false, csdl_setup->getWidth()-ACTION_MENU_WIDTH, 0);	//Show the new action menu
		}
		else
			doAction(ACTION_CANCEL); //Do a cancel action on the current unit. Player doesn't want to move the unit if he selects
									 //a hex that is outside a unit's move range
	}
	return;
}

customPoint CEnvironment::hitTest(customPoint gameMouse)
{
	customPoint result;
	int row = (int)(gameMouse.y/tileHeight_);
	int col = (int)(gameMouse.x/HEX_SIZE);
	gameMouse.x -= (col * HEX_SIZE);
	gameMouse.y -= (row * tileHeight_);
	if (gameMouse.y < upperThird_)
	{
		if (row % 2 == 0)
		{
			if (gameMouse.x < halfHex_ && gameMouse.y < -(gameMouse.x/2) +upperThird_)
			{
				row--;
				col--;
			}
			else if (gameMouse.x >= halfHex_ && (gameMouse.x - halfHex_)/2 > gameMouse.y)
				row--;
			//NOTE: Dividing these coordinates by 2 to compare them against the slope of the hex, which is calculated as 0.5
		}
		else
		{
			if (gameMouse.x < halfHex_ && gameMouse.y > (gameMouse.x/2))
				col--;
			else if (gameMouse.y < (gameMouse.x/2) && gameMouse.y < -((gameMouse.x - halfHex_)/2) +upperThird_)
				row--;
		}
	}
	else if (row % 2 != 0 && gameMouse.x < halfHex_)
		col--;
	result.x = col;
	result.y = row;
	return result;
}

bool CEnvironment::hitTestMenu(customPoint gameMouse)
{
	if (actionMenu_ == NULL)
		return false;
	else if (actionMenu_->mouseOverMenu(gameMouse))
		return true;
	return false;
}

void CEnvironment::endTurn()
{
	hideCursor();
	doAction(ACTION_CANCEL);
	for (int i=0; i<mapSize; i++)
	{
		if (hexes_[i].existsUnit() && hexes_[i].getUnit()->getOwner() == currentPlayer)
			hexes_[i].getUnit()->activate();
	}
	currentPlayer++;
	if (currentPlayer >= numberOfPlayers)
		currentPlayer = 0;
	setFog();
}

void CEnvironment::drawUtilityTiles(float camX, float camY)
{
	if (currentlySelectedUnit_ != NULL)
	{
		currentlySelectedUnit_->paintAttack(camX, camY);
		if (currentlySelectedUnit_->selectedForMove())
			currentlySelectedUnit_->paintMovement(camX, camY);
		//printf("%d, %d\n", currentlySelectedUnit_->getPixelPosition().x, currentlySelectedUnit_->getPixelPosition().y);
	}
}

bool CEnvironment::traversable(int x, int y, int moveRange, int unitType, int owner)
{
	if (anyEnemyOnTile(x, y, owner) != NULL)
		return false;
	switch(unitType)
	{
	case UNIT_INFANTRY:
		if (moveRange - hexes_[x + width*y].getMoveCost().infantry >= 0)
			return true;
		break;
	case UNIT_VEHICLE:
		if (moveRange - hexes_[x + width*y].getMoveCost().vehicle >= 0)
			return true;
		break;
	case UNIT_AIRCRAFT:
		if (moveRange - hexes_[x + width*y].getMoveCost().air >= 0)
			return true;
		break;
	}
	return false;
}

CUnit *CEnvironment::anyUnitOnTile(int x, int y)
{
	if (hexes_[x + width*y].existsUnit())
		return hexes_[x + width*y].getUnit();
	return NULL;
}

CUnit *CEnvironment::anyEnemyOnTile(int x, int y, int player)
{
	if (hexes_[x + width*y].existsUnit() && hexes_[x + width*y].getUnit()->getOwner() != player)
		return hexes_[x + width*y].getUnit();
	return NULL;
}

bool CEnvironment::existsEnemyAt(int index)
{
	if (hexes_[index].existsUnit() && hexes_[index].getUnit()->getOwner() != currentPlayer)
		return true;
	return false;
}

int CEnvironment::getMoveCost(int x, int y, int unitType)
{
	switch(unitType)
	{
	case UNIT_INFANTRY:
		return hexes_[x + width*y].getMoveCost().infantry;
	case UNIT_VEHICLE:
		return hexes_[x + width*y].getMoveCost().vehicle;
	case UNIT_AIRCRAFT:
		return hexes_[x + width*y].getMoveCost().air;
	}
	return NON_PASSABLE;
}

void CEnvironment::showActionMenu(bool canAttack, int x, int y)
{
	if (actionMenu_ != NULL)
		closeActionMenu();
	endTurnButton_->setActionToDo(ACTION_CANCEL);
	endTurnButton_->setLabel("Cancel");
	actionMenu_ = new CActionMenu(csdl_setup, actionMenuAssets_, x, y, ACTION_MENU_WIDTH);
	actionMenu_->addElement("Disband", ACTION_DISBAND);
	if (currentlySelectedUnit_->didMoveThisTurn())
		actionMenu_->addElement("Stay", ACTION_STAY);
}

void CEnvironment::destroyUnitIfNecessary(CTileData* hex)
{
	if (hex->getUnit()->getHP() <= 0)
	{
		printf("%s destroyed at %d, %d\n", hex->getUnit()->getUnitName().c_str(), hex->getUnit()->getPosition().x, hex->getUnit()->getPosition().y);
		hex->destroyUnit();
		return;
	}
	hex->getUnit()->setDefaultState(true);
	hex->getUnit()->setFloaterState(true);
	hex->getUnit()->updateHPLabel();
}

void CEnvironment::checkForCombatState()
{
	if (!isInNumberFloater_ && SDL_GetTicks() - animationLimit_ > 1500)
	{
		isInNumberFloater_ = true;
		destroyUnitIfNecessary(attackerHex_);
		destroyUnitIfNecessary(defenderHex_);
		animationLimit_ = SDL_GetTicks();
	}
	else if (isInNumberFloater_ && SDL_GetTicks() - animationLimit_ > 1500)
	{
		isInNumberFloater_ = false;
		isInCombat_ = false;
		if (attackerHex_->existsUnit())
		{
			attackerHex_->getUnit()->deleteFloater();
			attackerHex_->getUnit()->endUnitActions();
		}
		if (defenderHex_->existsUnit())
			defenderHex_->getUnit()->deleteFloater();
		attackerHex_ = NULL;
		defenderHex_ = NULL;
		unselectCurrentlySelectedUnit(); //wrapper function that deselects and nullifies the current unit pointer
	}
}

void CEnvironment::update(float camX, float camY)
{
	if (isInCombat_)
		checkForCombatState();
	//defining the context to draw on
	csdl_setup->useFullscreen();
	draw(camX, camY);
	drawUtilityTiles(camX, camY);
	drawTopLayer(camX, camY);
	drawOnScreenMenu(camX, camY);
	//paintMovementTile(3, 3, camX, camY);
}

void CEnvironment::closeActionMenu()
{
	delete actionMenu_;
	actionMenu_ = NULL;
	endTurnButton_->setActionToDo(ACTION_ENDTURN);
	endTurnButton_->setLabel("End Turn");
}

CTileData *CEnvironment::getHex(int x, int y)
{
	return &hexes_[x + y*width];
}

void CEnvironment::paintFog(float camX, float camY)
{
	imageCamera_.w = HEX_SIZE;
	imageCamera_.h = HEX_SIZE;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			imageCamera_.y = ((tileHeight_)*i) - camY;
			if (i % 2 == 0)
			{
				imageCamera_.x = (j*HEX_SIZE) - camX;
			} 
			else
			{
				imageCamera_.x = (j*HEX_SIZE) + (halfHex_) - camX;
			}
			if (hexes_[j + width * i].isFogged())
				SDL_RenderCopy(renderer, terrain, &endHighlight_, &imageCamera_);
		}
	}
}

void CEnvironment::setFog()
{
	for (int i=0; i<mapSize; i++)
		if (localBoard[i] != 0)
			hexes_[i].setFog(true);
	displayDefog();
}

void CEnvironment::displayDefog()
{
	for (int i=0; i<mapSize; i++)
	{
		if (hexes_[i].existsUnit() && hexes_[i].getUnit()->getOwner() == currentPlayer)
			hexes_[i].getUnit()->setDefog();
	}
}

void CEnvironment::drawOnScreenMenu(float camX, float camY)
{
	endTurnButton_->draw();
}

void CEnvironment::HUDButtonToggle(customPoint gameMouse)
{
	if (endTurnButton_->selected(gameMouse))
		endTurnButton_->stateSelected();
}

void CEnvironment::HUDButtonAction(customPoint gameMouse)
{
	if (endTurnButton_->selected(gameMouse))
		doAction(endTurnButton_->getActionToDo());
}

bool CEnvironment::hitTestButton(customPoint gameMouse)
{
	if (endTurnButton_->selected(gameMouse))
		return true;
	return false;
}

void CEnvironment::setButtonElementsDefault()
{
	if (actionMenu_ != NULL)
		actionMenu_->setToDefaultState();
	endTurnButton_->stateDefault();
}

void CEnvironment::doAction(int action)
{
	//printf("Beep!\n");
	switch(action)
	{
	case ACTION_STAY:
		if (currentlySelectedUnit_ != NULL)
		{
			currentlySelectedUnit_->endUnitActions();
			printf("%s staying at %d, %d\n", currentlySelectedUnit_->getUnitName().c_str(), currentlySelectedUnit_->getPosition().x, currentlySelectedUnit_->getPosition().y);
			unselectCurrentlySelectedUnit();
			setFog();
		}
		closeActionMenu();
		break;
	case ACTION_CANCEL:
		//return cursor to old position only if the player moved the unit and wanted to cancel that action
		if (currentlySelectedUnit_ != NULL)
		{
			bool moveCursor = false;
			if (currentlySelectedUnit_->didMoveThisTurn())
				moveCursor = true;
			currentlySelectedUnit_->doCancel();
			if (moveCursor)
				setCursorPosition(currentlySelectedUnit_->getPosition().x, currentlySelectedUnit_->getPosition().y);
			printf("%s canceled\n", currentlySelectedUnit_->getUnitName().c_str());
			unselectCurrentlySelectedUnit();
		}
		closeActionMenu();
		break;
	case ACTION_ENDTURN:
		printf("%d's turn ended\n", currentPlayer);
		endTurn();
		//*exit_ = true;
		break;
	}
	return;
}

void CEnvironment::doCombat(CTileData *attackerHex, CTileData *defenderHex, int weaponSelected)
{
	printf("%s attacking from %d, %d against %s at %d, %d\n", 
		attackerHex->getUnit()->getUnitName().c_str(), attackerHex->getUnit()->getPosition().x, attackerHex->getUnit()->getPosition().y,
		defenderHex->getUnit()->getUnitName().c_str(), defenderHex->getUnit()->getPosition().x, defenderHex->getUnit()->getPosition().y);

	attackerHex_ = attackerHex;
	defenderHex_ = defenderHex;

	int damage = 0;
	int retaliateDamage = 0;
	isInCombat_ = true;

	hideCursor();
	closeActionMenu();
	unselectCurrentlySelectedUnit();
	//attackerHex->getUnit()->hideUnitActionTiles();
	setFog();
	
	int retaliationWeapon = defenderHex->getUnit()->attackerInRangeForRetaliation(attackerHex->getUnit()); //See what weapon can target the attacker

	damage = attackerHex->getUnit()->calculateRawDamage(weaponSelected); //Ammo consumption is calculated in CUnit's calculateRawDamage function
	defenderHex->getUnit()->prepareDamageFloater(damage);
	attackerHex->getUnit()->startCombatAnimation();

	if (retaliationWeapon != NONE) //The attacker attacks back when a unit is within range of one of their weapons systems that can fire after move
	{
		retaliateDamage = defenderHex->getUnit()->calculateRawDamage(retaliationWeapon);
		defenderHex->getUnit()->startCombatAnimation();
		printf("%s defending from %d, %d\n", defenderHex->getUnit()->getUnitName().c_str(), defenderHex->getUnit()->getPosition().x, defenderHex->getUnit()->getPosition().y);
	}
	attackerHex->getUnit()->prepareDamageFloater(retaliateDamage);
	defenderHex->getUnit()->decrementHP(damage);
	attackerHex->getUnit()->decrementHP(retaliateDamage);
	animationLimit_ = SDL_GetTicks();
}

void CEnvironment::hideCursor()
{
	cursor->setPosition(pixelWidth, pixelHeight); //move the cursor to the far bottom right corner of the screen (off screen - hiding it)
}

void CEnvironment::unselectCurrentlySelectedUnit()
{
	if (currentlySelectedUnit_)
	{
		currentlySelectedUnit_->deselectUnit();
		currentlySelectedUnit_ = NULL;
	}
}

int getSign(int arg)
{
	if (arg < 0)
		return -1;
	else
		return 1;
}

bool CEnvironment::blockedLineOfSight(int originX, int originY, int targetX, int targetY)
{
	int deltaX = 2*(targetX-originX) + abs(targetY % 2) - abs(targetX % 2);
	int deltaY = targetY - originY;

	int Xsign = getSign(originX);
	int Ysign = getSign(originY);

	int changeY = abs(deltaY) * HEX_SIZE;
	int changeX = abs(deltaX) * HEX_SIZE;

	customPoint current;
	current.x = originX;
	current.y = originY;

	int debug = 0;

	int deviation = (-2)*(changeX);
	while (current.x != targetX && current.y != targetY)
	{
		if (debug > 30)
			system("PAUSE");
		printf("%d\n", debug);
		if (deviation >= 0)
		{
			current = getAdjacentHexAtDirection(current.x, current.y, -(Xsign), Ysign);
			deviation = deviation - 3*changeY - 3*changeX;
		}
		else
		{
			deviation = deviation + 3*changeY;
			if (deviation > -(changeX))
			{
				current = getAdjacentHexAtDirection(current.x, current.y, Xsign, Ysign);
				deviation = deviation - 3*changeX;
			}
			else
			{
				if (deviation < -3*changeY)
				{
					current = getAdjacentHexAtDirection(current.x, current.y, Xsign, -(Ysign));
					deviation = deviation + 3*changeX;
				}
				else
				{
					current = getAdjacentHexAtDirection(current.x, current.y, 0, Xsign);
					deviation = deviation + 3*changeY;
				}
			}
		}
		debug++;
		if (hexes_[current.x + width*current.y].getIsObstacle())
			return true;
	}
	return false;
}

customPoint CEnvironment::getAdjacentHexAtDirection(int x, int y, int i_dir, int j_dir)
{
	customPoint adjacent;
	adjacent.x = x;
	adjacent.y = y;
	if (y % 2 == 0)
	{
		switch(i_dir)
		{
		case -1:
			if (j_dir == -1)
			{
				adjacent.x--;
				adjacent.y--;
			}
			else
			{
				adjacent.x--;
				adjacent.y++;
			}
			break;
		case 0:
			if (j_dir == -1)
				adjacent.x--;
			else
				adjacent.x++;
			break;
		case 1:
			if (j_dir == -1)
				adjacent.y--;
			else
				adjacent.y++;
			break;
		}
	}
	else
	{
		switch(i_dir)
		{
		case -1:
			if (j_dir == -1)
				adjacent.y--;
			else
				adjacent.y++;
			break;
		case 0:
			if (j_dir == -1)
				adjacent.x--;
			else
				adjacent.x++;
			break;
		case 1:
			if (j_dir == -1)
			{
				adjacent.y--;
				adjacent.x++;
			}
			else
			{
				adjacent.y++;
				adjacent.x++;
			}
			break;
		}
	}
	return adjacent;
}
