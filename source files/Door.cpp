#include "Door.h"
#include <iostream>
#include <Windows.h>


Door::Door(int x, int y, ActorColor color, ActorColor closedColor)
	: PlaceableActor(x, y, color)
	, m_isOpen(false)
	, m_closedColor(closedColor)

{

}

void Door::Draw()
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	if (m_isOpen)

	SetConsoleTextAttribute(console, (int)m_color);

	else

	SetConsoleTextAttribute(console, (int)m_closedColor);

	std::cout << "|";
	SetConsoleTextAttribute(console, (int)ActorColor::Regular);
}