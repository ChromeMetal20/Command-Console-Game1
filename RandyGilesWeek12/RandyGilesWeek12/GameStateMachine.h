#pragma once
#include "GameState.h"

class GameState;

class GameStateMachine
{
public:
	virtual ~GameStateMachine() = default;

	virtual bool Init() = 0;
	virtual bool UpdateCurrentState(bool processInput = true) = 0;
	virtual void DrawCurrentState() = 0;
	virtual void ChangeState(GameState* pNewState) = 0;
	virtual void CleanUp() = 0;
};