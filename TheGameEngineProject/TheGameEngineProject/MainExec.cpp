#include "StdAfx.h"
#include "MainExec.h"
#include "Main.h"
#include "MainMenu.h"

CMainExec::CMainExec(int p_screenW, int p_screenH)
{
	screenW = p_screenW;
	screenH = p_screenH;

	currentState = NULL;
	nextState = NULL;

	exit = false;

	stateID = STATE_NULL;

	mouseX = 0; mouseY = 0;

	/*
	board[0] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 3, 0,
		0, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 3, 0,
		0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 0,
		0, 2, 2, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 0,
		0, 1, 2, 1, 1, 2, 1, 1, 3, 3, 1, 2, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 0,
		0, 1, 1, 1, 3, 1, 1, 1, 3, 1, 1, 1, 1, 0,
		0, 1, 2, 1, 3, 3, 1, 1, 1, 1, 1, 2, 1, 0,
		0, 1, 1, 3, 3, 1, 1, 2, 1, 1, 1, 2, 2, 0,
		0, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0,
		0, 3, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 0,
		0, 3, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};
	*/

	boardDimensions.x = 14;
	boardDimensions.y = 14;

	//stateID = STATE_GAME;
	//currentState = new CMain(csdl_setup);
	csdl_setup = new CSDL_Setup(exit, screenW, screenH);
}

void CMainExec::changeState()
{
	if (nextState != STATE_NULL)
	{
		if (nextState != STATE_EXIT)
			delete currentState;
		switch (nextState)
		{
		case STATE_MENU:
			currentState = new CMainMenu(csdl_setup, this, &mouseX, &mouseY);
			break;
		case STATE_GAME:
			currentState = new CMain(csdl_setup, board, boardDimensions, &mouseX, &mouseY, false, 2, NULL, this);
			break;
		}
		stateID = nextState;
		nextState = STATE_NULL;
	}
}

void CMainExec::setNextState(int gameState)
{
	if (nextState != STATE_EXIT)
		nextState = gameState;
}

void CMainExec::exec()
{
	//Mix_PlayMusic(csdl_setup->getMusic(), 1);
	while (!exit && csdl_setup->getMainEvent()->type != SDL_QUIT)
	{
		changeState();
		SDL_GetMouseState(&mouseX, &mouseY);
		csdl_setup->start();
		currentState->GameExec();
		csdl_setup->end();
	}
}

CMainExec::~CMainExec(void)
{
	delete csdl_setup;
	delete currentState;
	SDL_Quit();
}
