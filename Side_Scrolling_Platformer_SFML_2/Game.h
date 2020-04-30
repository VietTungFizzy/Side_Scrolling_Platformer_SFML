#pragma once
#include"StateManager.h"
#include"SharedContext.h"
#include"TextureManager.h"
#include"Map.h"
#include"SpriteSheet.h"

#define FRAME_RATE 1.0f / 60.0f
class Game
{
public:
	Game();
	~Game();

	void Render();
	void Update();
	
	void RestartClock(); //Make game more smoothe by use clock

	Window * GetWindow() { return m_window; }
	sf::Time GetElaspedTime() { return m_time; }
private:

	Map * m_map;
	StateManager m_StateMgr;
	SharedContex m_Context;

	Window * m_window;
	sf::Time m_time;
	sf::Clock m_clock;
};

