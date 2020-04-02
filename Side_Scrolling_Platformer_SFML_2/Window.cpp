#include "Window.h"
#include<iostream>


Window::Window()
{
	m_size = sf::Vector2u(800, 600);
	m_title = "Default";
	m_isClosed = false;
	Create();
	m_EventMgr = new EventManager();
	m_EventMgr->AddCallBack(StateType(0), "Window_close", &Window::Close, this);
}

Window::Window(std::string l_title, sf::Vector2u l_size)
{
	m_size = l_size;
	m_title = l_title;
	m_isClosed = false;
	Create();
	m_EventMgr = new EventManager();
	m_EventMgr->AddCallBack(StateType(0), "Window_close", &Window::Close, this);
}


Window::~Window()
{
	m_window.close();
}

void Window::ProcessEvent()
{
	sf::Event _event;
	while (m_window.pollEvent(_event))
	{
		if (_event.type == sf::Event::Closed)
		{
			m_isClosed = true;
			m_window.close();
		}
		
		m_EventMgr->HandleEvent(_event);
	}

	m_EventMgr->Update();
}

void Window::BeginDraw()
{
	m_window.clear();
}

void Window::EndDraw()
{
	m_window.display();
}

void Window::Draw(const sf::Drawable & l_obj)
{
	m_window.draw(l_obj);
}

void Window::ToggleFullScreen()
{
	m_isFullScreen = true;

	m_window.close();
	Create();
}

sf::FloatRect Window::GetViewSpace()
{
	sf::Vector2f viewCenter = m_viewSpace.getCenter();
	sf::Vector2f viewSize = m_viewSpace.getSize();
	sf::Vector2f viewHalfSize(viewSize.x / 2, viewSize.y / 2);
	return sf::FloatRect(viewCenter - viewHalfSize,viewSize);
}

void Window::Create()
{
	unsigned int  style = (m_isFullScreen ? sf::Style::Fullscreen : sf::Style::Default);
	m_window.create(sf::VideoMode(m_size.x, m_size.y), m_title, style);
	m_viewSpace = m_window.getDefaultView();
	std::cout << "Create window with size(" << m_size.x << " " << m_size.y << ") Title: " << 
		m_title << " with " << (m_isFullScreen ? "Full screen" : "Default") << std::endl;
}
