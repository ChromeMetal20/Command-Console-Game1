#include "LoseState.h"
#include "MainMenuState.h"

#include <iostream>
#include <conio.h>

#include "StateMachineExampleGame.h"

LoseState::LoseState(StateMachineExampleGame* pOwner)
	: m_pOwner(pOwner)
{

}

bool LoseState::Update(bool processInput)
{
	if (processInput)
	{
		int input = _getch();
		m_pOwner->LoadScene(StateMachineExampleGame::SceneName::MainMenu);
	}
	return false;
}

void LoseState::Draw()
{
	system("cls");
	std::cout << std::endl << std::endl << std::endl;
	std::cout << "		   - - - GAME OVER - - -" << std::endl << std::endl; 
	std::cout << "		  Better luck next time..." << std::endl << std::endl << std::endl;
	std::cout << "	Return to menu by pressing any key" << std::endl << std::endl;
}