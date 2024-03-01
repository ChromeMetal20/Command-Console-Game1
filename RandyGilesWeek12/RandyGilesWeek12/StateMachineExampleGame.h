#pragma once
#include "GameStateMachine.h"

class Game;
class GameStates;

class StateMachineExampleGame : public GameStateMachine
{
public:
	enum class SceneName
	{
		None,
		MainMenu,
		Gameplay,
		Settings,
		Highscore,
		Lose,
		Win
	};
private:
	Game* m_pOwner;

	GameState* m_pCurrentState;
	GameState* m_pNewState;

public:
	StateMachineExampleGame(Game* pOwner);

	virtual bool Init() override;
	virtual bool UpdateCurrentState(bool processInput = true) override;
	virtual void DrawCurrentState() override;
	virtual void ChangeState(GameState* pNewState) override;
	virtual void CleanUp() override;
	void LoadScene(SceneName scene);
};