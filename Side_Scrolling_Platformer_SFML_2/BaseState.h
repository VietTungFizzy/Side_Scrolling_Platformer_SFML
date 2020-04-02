#pragma once
#include<SFML/Graphics.hpp>

class StateManager;
class BaseState
{
	friend class StateManager;
public:
	BaseState(StateManager * l_StateMgr) : m_StateMgr(l_StateMgr), m_transcend(false), m_transparent(false){}
	~BaseState();
	virtual void Update(const sf::Time & l_time) = 0;
	virtual void Render() = 0;

	virtual void Activate() = 0;
	virtual void Deactivate() = 0;

	virtual void OnDestroy() = 0;
	virtual void OnCreate() = 0;

	StateManager * GetStateManager() { return m_StateMgr; }
public:
	bool m_transcend;
	bool m_transparent;

private:
	StateManager * m_StateMgr;
	sf::View m_view;
};

