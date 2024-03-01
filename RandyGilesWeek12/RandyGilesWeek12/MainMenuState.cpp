#include "MainMenuState.h"

#include <iostream>
#include <conio.h>

#include "StateMachineExampleGame.h"

constexpr int kEscapeKey = 27;

constexpr char kPlay = '1';
constexpr char kHighScore = '2';
constexpr char kSettings = '3';
constexpr char kQuit = '4';

MainMenuState::MainMenuState(StateMachineExampleGame* pOwner)
	: m_pOwner(pOwner)
{

}

bool MainMenuState::Update(bool processInput)
{
	bool shouldQuit = false;
	if (processInput)
	{
		int input = _getch();
		if (input == kEscapeKey || (char)input == kQuit)
		{
			shouldQuit = true;
		}
		else if ((char)input == kPlay)
		{
			m_pOwner->LoadScene(StateMachineExampleGame::SceneName::Gameplay);
		}
		else if ((char)input == kHighScore)
		{
			m_pOwner->LoadScene(StateMachineExampleGame::SceneName::Highscore);
		}
		else if ((char)input == kSettings)
		{
			m_pOwner->LoadScene(StateMachineExampleGame::SceneName::Settings);
		}
	}
	return shouldQuit;
}

void MainMenuState::Draw()
{
	system("cls");
	std::cout << std::endl << std::endl << std::endl;
	std::cout << "		   - - - MAIN MENU - - -" << std::endl << std::endl;
	std::cout << "             " << kPlay << ".Play " << std::endl;
	std::cout << "             " << kHighScore << ".Highscore " << std::endl;
	std::cout << "             " << kSettings << ".Settings " << std::endl;
	std::cout << "             " << kQuit << ".Quit " << std::endl;
}