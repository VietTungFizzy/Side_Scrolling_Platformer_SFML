#pragma once
#include<SFML/Graphics.hpp>
#include<unordered_map>
#include<functional>
enum class EventType
{
	KeyPress = sf::Event::KeyPressed,
	KeyRelease = sf::Event::KeyReleased,
	Closed = sf::Event::Closed,
	KeyBoard = sf::Event::Count + 1
};
struct EventInfo
{
	union
	{
		int m_code;
	};
};
struct EventDetails
{
	EventDetails(const std::string & l_name) : m_name(l_name) { Clear(); }
	std::string m_name;
	int m_KeyCode;
	void Clear()
	{
		m_name = "";
		m_KeyCode = -1;
	}
};
using Events = std::vector<std::pair<EventType, EventInfo>>;

struct Binding
{
	Binding(const std::string & l_name) : m_name(l_name) , m_EventDetails(l_name) 
	{
	}
	void BindEvent(EventType l_type, EventInfo l_info = EventInfo())
	{
		m_Events.emplace_back(l_type, l_info);
	}

	int m_CurrentEvents;
	std::string m_name;
	EventDetails m_EventDetails;
	Events m_Events;
};

using Bindings = std::unordered_map<std::string, Binding*>;
using CallBackContainer = std::unordered_map<std::string, std::function<void(EventDetails *)>>;
enum class StateType;
using StateCallBack = std::unordered_map<StateType, CallBackContainer>;

class EventManager
{
public:
	EventManager();
	~EventManager();

	template <class T>
	bool AddCallBack(StateType l_type ,std::string l_name, void(T::*l_function)(EventDetails*), T * l_instance)
	{
		std::unordered_map<StateType, CallBackContainer>::iterator tmp = m_CallBacks.emplace(l_type, CallBackContainer()).first;
		std::function<void(EventDetails *)> bindFunc = std::bind(l_function, l_instance, std::placeholders::_1); // ==> function object of l_instance->l_function(_1)
		return tmp->second.emplace(l_name, bindFunc).second;
		
	}

	void RemoveCallBack(std::string l_name)
	{
		std::unordered_map<StateType, CallBackContainer>::iterator tmp = m_CallBacks.find(m_CurrentState);
		if (tmp == m_CallBacks.end()) return;
		tmp->second.erase(l_name);
	}

	bool AddBinding(Binding * l_bindings);
	bool RemoveBinding(std::string l_name);

	void HandleEvent(sf::Event l_event); //Deal with Event from SFML/System
	
	// Check if any binding codition is satisfied
	// if so, call method from Callbacks
	void Update();

public:
	StateType m_CurrentState;
private:
	void LoadBinding();

	Bindings m_Bindings;
	StateCallBack m_CallBacks;
};

