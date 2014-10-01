#include "stdafx.h"
#include "Player.h"


CPlayer::CPlayer()
{
}


CPlayer::~CPlayer()
{
	playerName_ = NULL;
}

void CPlayer::initializeValues(char p_playerName[], int p_team)
{
	playerName_ = p_playerName;
	team_ = p_team;
}