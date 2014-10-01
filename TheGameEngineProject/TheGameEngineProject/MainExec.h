#pragma once
#include <fstream>
#include "GameState.h"
#include "stdafx.h"
#include "SDL_Setup.h"

class CMainExec
{
public:
	CMainExec(int p_screenW, int p_screenH);
	~CMainExec(void);

	void setNextState(int gameState);

	void exec();
private:
	void changeState();

	int stateID;
	int nextState;

	//TO BE MOVED
	int board[1024];
	customPoint boardDimensions;

	bool exit;

	int mouseX, mouseY;

	int screenW;
	int screenH;

	CSDL_Setup* csdl_setup;

	CGameState *currentState;
};

