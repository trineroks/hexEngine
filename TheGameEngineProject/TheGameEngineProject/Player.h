#pragma once
class CPlayer
{
public:
	CPlayer();
	~CPlayer();
	void initializeValues(char p_playerName[], int p_team);

	char* getPlayerName() { return playerName_; }
private:
	char* playerName_;
	int team_;
};

