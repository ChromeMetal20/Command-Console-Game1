#pragma once
#include "Point.h"



enum class ActorColor
{
	Regular = 7,
	Green = 10,
	Blue = 9,
	Red = 12,
	SolidGreen = 34,
	SolidBlue = 153,
	SolidRed = 68,
};

enum class ActorType
{
	Door,
	Enemy,
	Goal,
	Key,
	Money,
	Player
};

class PlaceableActor
{
public:
	PlaceableActor(int x, int y, ActorColor color = ActorColor::Regular);
	virtual ~PlaceableActor();

	int GetXPosition();
	int GetYPosition();
	int* GetXPositionPointer();
	int* GetYPositionPointer();
	void SetPosition(int x, int y);

	ActorColor GetColor() { return m_color; }

	void Remove() { m_IsActive = false; }
	bool IsActive() { return m_IsActive; }
	void Place(int x, int y);

	virtual ActorType GetType() = 0;
	virtual void Draw() = 0;
	virtual void Update()
	{

	}

protected:
	Point* m_pPosition;

	bool m_IsActive;
	ActorColor m_color;
};