#pragma once
#include"BaseState.h"
#include"SharedContext.h"
#include<unordered_map>
#include<functional>
enum class StateType
{
	Intro = 1, Pause
};

using StateContainer = std::vector<std::pair<StateType, BaseState*>>;
using StateStack = std::vector<StateType>;
using StateFactory = std::unordered_map<StateType, std::function<BaseState*(void)>>;

class StateManager
{
public:
	StateManager(SharedContex * l_Shared);
	~StateManager();

	void SwitchState(const StateType & l_type);
	void Update(const sf::Time & l_time);
	void Render();
	
	void CreateState(const StateType & l_type);
	void RemoveState(const StateType & l_type);
	
	bool HasState(const StateType & l_type);

	SharedContex * GetSharedContext() { return m_Shared; }


	void DeleteRemovingState();
private:

	template<class T>
	void RegisterState(const StateType & l_type)
	{
		m_Factory[l_type] = [this]() -> BaseState *
		{
			return new T(this);
		};
	}
private:
	SharedContex * m_Shared;
	StateFactory m_Factory;
	StateStack m_ToRemove;
	StateContainer m_Container;
};

