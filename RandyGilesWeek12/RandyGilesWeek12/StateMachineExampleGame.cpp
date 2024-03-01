#include "StateMachineExampleGame.h"
#include "MainMenuState.h"
#include "GameplayState.h"
#include "Game1.h"
#include "SettingsState.h"
#include "HighScoreState.h"
#include "WinState.h"
#include "LoseState.h"


StateMachineExampleGame::StateMachineExampleGame(Game* pOwner)
	: m_pOwner(pOwner)
	, m_pCurrentState(nullptr)
	, m_pNewState(nullptr)
{

}

bool StateMachineExampleGame::Init()
{
	LoadScene(SceneName::MainMenu);
	return true;
}
bool StateMachineExampleGame::UpdateCurrentState(bool processInput)
{
	bool done = false;
	if (m_pNewState != nullptr)
	{

		ChangeState(m_pNewState);
		m_pNewState = nullptr;

	}
	if (m_pCurrentState != nullptr)
	{
		done = m_pCurrentState->Update(processInput);
	}
	return done;
}

void StateMachineExampleGame::DrawCurrentState()
{
	if (m_pCurrentState)
	{
		m_pCurrentState->Draw();
	}
}
void StateMachineExampleGame::ChangeState(GameState* pNewState)
{
	if (m_pCurrentState)
	{
		m_pCurrentState->Exit();
	}
	delete m_pCurrentState;
	m_pCurrentState = pNewState;
	pNewState->Enter();
}

void StateMachineExampleGame::CleanUp()
{
	if (m_pCurrentState)
	{
		m_pCurrentState->Exit();
		delete m_pCurrentState;
	}
}
void StateMachineExampleGame::LoadScene(SceneName scene)
{
	switch (scene)
	{
		case SceneName::MainMenu:
			m_pNewState = new MainMenuState(this);
			break;
		case SceneName::Gameplay:
			m_pNewState = new GameplayState(this);
			break;
		case SceneName::Settings:
			m_pNewState = new SettingsState(this);
			break;
		case SceneName::Highscore:
			m_pNewState = new HighScoreState(this);
			break;
		case SceneName::Win:
			m_pNewState = new WinState(this);
			break;
		case SceneName::Lose:
			m_pNewState = new LoseState(this);
			break;
		default:
			break;
	}
}
			