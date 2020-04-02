#pragma once
#include<SFML/Graphics.hpp>
#include<iostream>
#include"EventManager.h"

class Window
{
public:
	Window();
	Window(std::string l_title, sf::Vector2u l_size);
	~Window();
	
	void ProcessEvent();

	void BeginDraw(); //Clear screen
	void EndDraw(); //Draw screen
	void Draw(const sf::Drawable & l_obj);

	void ToggleFullScreen();

	bool IsWindowClosed() { return  m_isClosed; }
	sf::Vector2u GetWindowSize() { return m_size; }
	EventManager * GetEventManager() { return m_EventMgr; }
	sf::RenderWindow * GetWindowRender() { return &m_window; }
	sf::FloatRect GetViewSpace();
private:
	void Create();
	void Close(EventDetails * l_details = nullptr) { m_isClosed = true; m_window.close(); }

	EventManager * m_EventMgr;
	sf::View m_viewSpace;
	std::string m_title;
	sf::Vector2u m_size;
	sf::RenderWindow m_window;
	bool m_isFullScreen;
	bool m_isClosed;
};

