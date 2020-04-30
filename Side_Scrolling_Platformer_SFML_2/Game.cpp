#include "Game.h"



Game::Game() : m_clock(), m_StateMgr(&m_Context)
{
	m_window = new Window("2D Side Platform", sf::Vector2u(1280, 720));
	m_StateMgr.GetSharedContext()->m_window = m_window;
	m_StateMgr.GetSharedContext()->m_EventManager = m_window->GetEventManager();
	m_StateMgr.GetSharedContext()->m_TextureManager = new TextureManager();
	m_map = new Map(&m_Context);
	
	m_map->LoadTile("Configuration/tile.cfg");
	m_map->LoadMap("Configuration/Map/map1.map");	

}


Game::~Game()
{
}

void Game::Render()
{
	m_window->BeginDraw();

	m_StateMgr.Render();
	m_Context.m_Map->Draw();
	

	m_window->EndDraw();
}

void Game::Update()
{
	if (m_time.asSeconds() >= FRAME_RATE)
	{
		m_window->ProcessEvent();
		m_StateMgr.Update(m_time);
		m_Context.m_Map->Update(m_time.asSeconds());
		m_time -= sf::seconds(FRAME_RATE);
	}
	
}

void Game::RestartClock()
{
	m_StateMgr.DeleteRemovingState();
	m_time += m_clock.getElapsedTime();
}
