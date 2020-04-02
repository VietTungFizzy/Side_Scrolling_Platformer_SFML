#include "EventManager.h"
#include"Utils.h"
#include<iostream>
#include<sstream>
#include<fstream>

EventManager::EventManager()
{
	LoadBinding();
}

EventManager::~EventManager()
{
}

bool EventManager::AddBinding(Binding * l_bindings)
{
	if (m_Bindings.find(l_bindings->m_name) != m_Bindings.end())
	{
		std::cout << "Duplicate Bindings " << l_bindings->m_name << std::endl;
		return false;
	}

	return m_Bindings.emplace(l_bindings->m_name, l_bindings).second;
}

bool EventManager::RemoveBinding(std::string l_name)
{
	std::unordered_map<std::string, Binding*>::iterator tmp = m_Bindings.find(l_name);
	if (tmp == m_Bindings.end()) return false;

	delete tmp->second;
	m_Bindings.erase(tmp);
	
	std::cout << "Remove Successful\n";

	return true;
}

void EventManager::HandleEvent(sf::Event l_event)
{
	for (std::unordered_map<std::string,Binding*>::value_type & itr : m_Bindings)
	{
		Binding * bind = itr.second;
		for (std::vector<std::pair<EventType, EventInfo>>::value_type & event_itr : bind->m_Events)
		{
			if ((EventType)l_event.type != event_itr.first) continue;

			switch (event_itr.first)
			{
			case EventType::KeyPress:
			case EventType::KeyRelease:
			{
				if (event_itr.second.m_code == l_event.key.code)
				{
					if (bind->m_EventDetails.m_KeyCode != -1)
					{
						bind->m_EventDetails.m_KeyCode = l_event.key.code;
					}
					bind->m_CurrentEvents++;
				}
			}break;
			case EventType::Closed:
			{

			} break;
			default:
				break;
			}
		}
	}
}

void EventManager::Update()
{
	for (std::unordered_map<std::string, Binding*>::value_type & itr : m_Bindings)
	{
		Binding * bind = itr.second;
		for (std::vector<std::pair<EventType, EventInfo>>::value_type & event_itr : bind->m_Events)
		{
			switch (event_itr.first)
			{
			case EventType::KeyBoard:
			{
				if (sf::Keyboard::isKeyPressed((sf::Keyboard::Key)event_itr.second.m_code))
				{
					if (bind->m_EventDetails.m_KeyCode != -1)
					{
						bind->m_EventDetails.m_KeyCode = event_itr.second.m_code;
					}
					bind->m_CurrentEvents++;

				}
			}break;
			default:
				break;
			}
		}

		if (bind->m_CurrentEvents == bind->m_Events.size())
		{
			std::unordered_map<StateType, CallBackContainer>::iterator tmp = m_CallBacks.find(m_CurrentState);
			if (tmp != m_CallBacks.end())
			{
				std::unordered_map<std::string, std::function<void(EventDetails*)>>::iterator tmp_1 = tmp->second.find(bind->m_name);
				if (tmp_1 != tmp->second.end()) tmp_1->second(&bind->m_EventDetails);
			}

			tmp = m_CallBacks.find(StateType(0));
			if (tmp != m_CallBacks.end())
			{
				std::unordered_map<std::string, std::function<void(EventDetails*)>>::iterator tmp_1 = tmp->second.find(bind->m_name);
				if (tmp_1 != tmp->second.end()) tmp_1->second(&bind->m_EventDetails);
			}
		}

		bind->m_CurrentEvents = 0;
		bind->m_EventDetails.Clear();
	}
}

void EventManager::LoadBinding()
{
	std::ifstream input;
	input.open(Utils::GetWorkingDirection() + "Configuration/config.cfg");
	if (!input.is_open())
	{
		std::cout << "Can't open " << Utils::GetWorkingDirection() + "Configuration/config.cfg" << std::endl;
		return;
	}

	std::string line;
	std::string delimiter = ":";
	while (std::getline(input, line))
	{
		if (line[0] == '|') continue;
		std::stringstream keystream(line);
		std::string name;
		keystream >> name;
		Binding * bind = new Binding(name);

		while (!keystream.eof())
		{
			std::string value;
			keystream >> value;
			int start = 0;
			int end = value.find(delimiter);
			if (end == std::string::npos)
			{
				delete bind;
				break;
			}

			EventType eventType = (EventType)std::stoi(value.substr(start, end - start));
			start = end + delimiter.length();
			end = value.find(delimiter, start);
			EventInfo info;
			info.m_code = std::stoi(value.substr(start, end - start));
			
			bind->BindEvent(eventType, info);
		}

		if (AddBinding(bind) == false)
		{
			std::cout << "Fail to add Binding: " << name << std::endl;
			delete bind;
		}

		bind = nullptr;
	}

	input.close();
}
