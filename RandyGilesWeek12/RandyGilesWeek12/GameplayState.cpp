#include "GameplayState.h"
#include <iostream>
#include <conio.h>
#include "Player.h"
#include "Level.h"
#include <assert.h>
#include <AudioManager.h>

#include "Enemy.h"
#include "Key.h"
#include "Door.h"
#include "Money.h"
#include "Goal.h"
#include "Utility.h"
#include "StateMachineExampleGame.h"

using namespace std;

constexpr int kArrowInput = 224;
constexpr int kLeftArrow = 75;
constexpr int kRightArrow = 77;
constexpr int kUpArrow = 72;
constexpr int kDownArrow = 80;
constexpr int kEscapeKey = 27;

GameplayState::GameplayState(StateMachineExampleGame* pOwner)
	: m_pOwner(pOwner)
	, m_beatLevel(false)
	, m_skipFrameCount(0)
	, m_currentLevel(0)
	, m_pLevel(nullptr)
{
	m_LevelNames.push_back("Level1.txt");
	m_LevelNames.push_back("Level2.txt");
	m_LevelNames.push_back("Level3.txt");
}

GameplayState::~GameplayState()
{
	delete m_pLevel;
	m_pLevel = nullptr;
}

bool GameplayState::Load()
{
	if (m_pLevel)
	{
		delete m_pLevel;
		m_pLevel = nullptr;
	}

	m_pLevel = new Level();
	return m_pLevel->Load(m_LevelNames.at(m_currentLevel), m_Player.GetXPositionPointer(), m_Player.GetYPositionPointer());
}

void GameplayState::Enter()
{
	Load();
}

bool GameplayState::Update(bool processInput)
{
	if (processInput && !m_beatLevel)
	{
		int input = _getch();
		int arrowInput = 0;
		int newPlayerX = m_Player.GetXPosition();
		int newPlayerY = m_Player.GetYPosition();

		// One of the arrow keys were pressed
		if (input == kArrowInput)
		{
			arrowInput = _getch();
		}
		if ((input == kArrowInput && arrowInput == kLeftArrow) || (char)input == 'A' || (char)input == 'a')
		{
			newPlayerX--;
		}
		else if ((input == kArrowInput && arrowInput == kRightArrow) || (char)input == 'D' || (char)input == 'd')
		{
			newPlayerX++;
		}
		else if ((input == kArrowInput && arrowInput == kUpArrow) || (char)input == 'W' || (char)input == 'w')
		{
			newPlayerY--;
		}
		else if ((input == kArrowInput && arrowInput == kDownArrow) || (char)input == 'S' || (char)input == 's')
		{
			newPlayerY++;
		}
		else if (input == kEscapeKey)
		{
			m_pOwner->LoadScene(StateMachineExampleGame::SceneName::MainMenu);
		}
		else if ((char)input == 'Z' || (char)input == 'z')
		{
			m_Player.DropKey();
		}
		// If position never changed
		if (newPlayerX == m_Player.GetXPosition() && newPlayerY == m_Player.GetYPosition())
		{

		}
		else
		{
			HandleCollision(newPlayerX, newPlayerY);
		}
	}

	if (m_beatLevel)
	{

		++m_skipFrameCount;
		if (m_skipFrameCount > kFramesToSkip)
		{
			m_beatLevel = false;
			m_skipFrameCount = 0;

			++m_currentLevel;
			if (m_currentLevel == m_LevelNames.size())
			{
				Utility::WriteHighScore(m_Player.GetMoney());
				AudioManager::GetInstance()->PlayWinSound();
				m_pOwner->LoadScene(StateMachineExampleGame::SceneName::Win);
			}
			else
			{
				Load();
			}
		}
	}
	return false;
}

void GameplayState::HandleCollision(int newPlayerX, int newPlayerY)
{
	bool isGameDone = false;
	PlaceableActor* collidedActor = m_pLevel->UpdateActors(newPlayerX, newPlayerY);
	if (collidedActor != nullptr && collidedActor->IsActive())
	{
		switch (collidedActor->GetType())
		{
		case ActorType::Enemy:
		{
			Enemy* collidedEnemy = dynamic_cast<Enemy*>(collidedActor);
			assert(collidedEnemy);
			AudioManager::GetInstance()->PlayLoseLivesSound();
			collidedEnemy->Remove();
			m_Player.SetPosition(newPlayerX, newPlayerY);

			m_Player.DecrementLives();
			if (m_Player.GetLives() < 0)
			{
				AudioManager::GetInstance()->PlayLoseSound();
				m_pOwner->LoadScene(StateMachineExampleGame::SceneName::Lose);
			}
			break;
		}
		case ActorType::Money:
		{
			Money* collidedMoney = dynamic_cast<Money*>(collidedActor);
			assert(collidedMoney);
			AudioManager::GetInstance()->PlayMoneySound();
			collidedMoney->Remove();
			m_Player.AddMoney(collidedMoney->GetWorth());
			m_Player.SetPosition(newPlayerX, newPlayerY);
			break;
		}
		case ActorType::Key:
		{
			Key* collidedKey = dynamic_cast<Key*>(collidedActor);
			assert(collidedKey);
			if (!m_Player.HasKey())
			{
				m_Player.PickupKey(collidedKey);
				AudioManager::GetInstance()->PlayKeyPickupSound();
				collidedKey->Remove();
				m_Player.SetPosition(newPlayerX, newPlayerY);
				//PlayKeyPickupSound();
			}
			break;
		}
		case ActorType::Door:
		{
			Door* collidedDoor = dynamic_cast<Door*>(collidedActor);
			assert(collidedDoor);
			if (!collidedDoor->IsOpen())
			{
				if (m_Player.HasKey(collidedDoor->GetColor()))
				{
					AudioManager::GetInstance()->PlayDoorOpenSound();
					collidedDoor->Open();
					collidedDoor->Remove();
					m_Player.UseKey();
					m_Player.SetPosition(newPlayerX, newPlayerY);
				}
				else
				{
					AudioManager::GetInstance()->PlayDoorClosedSound();
				}
			}
			else
			{
				m_Player.SetPosition(newPlayerX, newPlayerY);
			}
			break;
		}
		case ActorType::Goal:
		{
			Goal* collidedGoal = dynamic_cast<Goal*>(collidedActor);
			assert(collidedGoal);
			AudioManager::GetInstance()->PlayWinSound();
			collidedGoal->Remove();
			m_Player.SetPosition(newPlayerX, newPlayerY);
			m_beatLevel = true;
			break;
		}
		default:
			break;
		}
	}
	else if (m_pLevel->IsSpace(newPlayerX, newPlayerY)) // no collision
	{
		m_Player.SetPosition(newPlayerX, newPlayerY);
	}
	else if (m_pLevel->IsWall(newPlayerX, newPlayerY))
	{

	}
};

void GameplayState::Draw()
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	system("cls");

	m_pLevel->Draw();

	// Set cursor position for player
	COORD actorCursorPosition;
	actorCursorPosition.X = m_Player.GetXPosition();
	actorCursorPosition.Y = m_Player.GetYPosition();
	SetConsoleCursorPosition(console, actorCursorPosition);
	m_Player.Draw();

	// Set the cursor to the end of the level
	COORD currentCursorPosition;
	currentCursorPosition.X = 0;
	currentCursorPosition.Y = m_pLevel->GetHeight();
	SetConsoleCursorPosition(console, currentCursorPosition);

	DrawHUD(console);
}

void GameplayState::DrawHUD(const HANDLE& console)
{
	cout << endl;

	// Top Border
	for (int i = 0; i < m_pLevel->GetWidth(); ++i)
	{
		cout << Level::WAL;
	}
	cout << endl;

	// Left Border
	cout << Level::WAL;

	cout << " wasd-move " << Level::WAL << " z-drop key " << Level::WAL;
	cout << " $:" << m_Player.GetMoney() << " " << Level::WAL;
	cout << " lives:" << m_Player.GetLives() << " " << Level::WAL;
	cout << "keys:";
	if (m_Player.HasKey())
	{
		m_Player.GetKey()->Draw();
	}
	else
	{
		cout << " ";
	}
	
	// Bottom border
	
	// Right border
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(console, &csbi);

	COORD pos;
	pos.X = m_pLevel->GetWidth() - 1;
	pos.Y = csbi.dwCursorPosition.Y;
	SetConsoleCursorPosition(console, pos);
	
	cout << Level::WAL;
	cout << endl;
	
	for (int i = 0; i < m_pLevel->GetWidth(); ++i)
	{
		cout << Level::WAL;
	}
	cout << endl;
}