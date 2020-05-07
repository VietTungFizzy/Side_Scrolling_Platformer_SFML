#pragma once
#include"Window.h"
#include"EventManager.h"
#include"TextureManager.h"
#include"EntityManager.h"

class Map;
class SharedContex
{
public:
	SharedContex() :m_window(nullptr),m_EventManager(nullptr), m_TextureManager(nullptr),
					m_Map(nullptr), m_EntityManager(nullptr){}
	Window * m_window;
	EventManager * m_EventManager;
	TextureManager * m_TextureManager;
	Map * m_Map;
	EntityManager * m_EntityManager;
};