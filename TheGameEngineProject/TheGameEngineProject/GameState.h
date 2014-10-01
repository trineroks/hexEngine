#pragma once
#include "stdafx.h"
#include "InputController.h"

class CGameState
{
public:
	CGameState();
	virtual void GameExec() = 0; //Why is this equal to 0?
	~CGameState(void);
private:
	int screenW;
	int screenH;
};

