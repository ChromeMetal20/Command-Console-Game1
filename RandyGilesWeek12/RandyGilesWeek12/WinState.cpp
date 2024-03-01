#include "WinState.h"
#include "MainMenuState.h"

#include <iostream>
#include <conio.h>

#include "StateMachineExampleGame.h"

WinState::WinState(StateMachineExampleGame* pOwner)
	: m_pOwner(pOwner)
{

}

bool WinState::Update(bool processInput)
{
	if (processInput)
	{
		int input = _getch();
		m_pOwner->LoadScene(StateMachineExampleGame::SceneName::MainMenu);
	}
	return false;
}

void WinState::Draw()
{
	system("cls");
	std::cout << std::endl << std::endl << std::endl;
	std::cout << "		   - - - Well done - - -" << std::endl << std::endl;
	std::cout << "		  You have beaten the game." << std::endl << std::endl << std::endl;
	std::cout << "	Return to menu by pressing any key" << std::endl << std::endl;
}