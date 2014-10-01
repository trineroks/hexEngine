// TheGameEngineProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MainExec.h"

int main(int argc, char *argv[])
{
	CMainExec* main = new CMainExec(1000, 600);
	main->setNextState(STATE_GAME);
	main->exec();
	delete main;

	return 0;
}


