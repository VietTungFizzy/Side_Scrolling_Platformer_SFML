#include "StateManager.h"

StateManager::StateManager(SharedContext * l_Shared) : m_Shared(l_Shared)
{
}

StateManager::~StateManager()
{
	while (m_Container.begin() != m_Container.end())
	{
		delete m_Container.begin()->second;
		m_Container.erase(m_Container.begin());
	}
}

void StateManager::SwitchState(const StateType & l_type)
{
	m_Shared->m_EventManager->m_CurrentState = l_type;

	if (HasState(l_type))
	{
		for (std::vector<std::pair<StateType, BaseState*>>::iterator itr = m_Container.begin() ; 
				itr != m_Container.end() ; itr++)
		{
			if (itr->first == l_type)
			{
				m_Container.back().second->Deactivate();
				BaseState * tmp = itr->second;
				m_Container.erase(itr);
				m_Container.emplace_back(l_type, tmp);
				tmp->Activate();
				m_Shared->m_window->GetWindowRender()->setView(tmp->m_view);
				break;
			}
		}
	}
	else
	{
		if (m_Container.size() > 0) m_Container.back().second->Deactivate();
		CreateState(l_type);
		m_Container.back().second->Activate();
		m_Shared->m_window->GetWindowRender()->setView(m_Container.back().second->m_view);
	}
}

void StateManager::Update(const sf::Time & l_time)
{
	if (m_Container.empty()) return;
	if (m_Container.back().second->m_transcend && m_Container.size() > 1)
	{
		std::vector<std::pair<StateType, BaseState*>>::iterator tmp = m_Container.end();
		while (tmp != m_Container.begin())
		{
			if (tmp != m_Container.end())
			{
				if (!tmp->second->m_transcend)
					break;
			}
			tmp--;
		}

		while (tmp != m_Container.end())
		{
			tmp->second->Update(l_time);
			tmp++;
		}
	}
	else
		m_Container.back().second->Update(l_time);
}

void StateManager::Render()
{
	if (m_Container.empty()) return;
	if (m_Container.back().second->m_transparent && m_Container.size() > 1)
	{
		std::vector<std::pair<StateType, BaseState*>>::iterator tmp = m_Container.end();
		while (tmp != m_Container.begin())
		{
			if (tmp != m_Container.end())
			{
				if (!tmp->second->m_transparent)
					break;
			}
			tmp--;
		}

		while (tmp != m_Container.end())
		{
			m_Shared->m_window->GetWindowRender()->setView(tmp->second->m_view);
			tmp->second->Render();
			tmp++;
		}
	}
	else
		m_Container.back().second->Render();
}

void StateManager::CreateState(const StateType & l_type)
{
	std::unordered_map<StateType, std::function<BaseState*(void)>>::iterator itr = m_Factory.find(l_type);
	if (itr == m_Factory.end()) return;
	BaseState * tmp = itr->second();
	m_Container.emplace_back(l_type, tmp);
	tmp->OnCreate();
	tmp->m_view = m_Shared->m_window->GetWindowRender()->getDefaultView();
}

void StateManager::RemoveState(const StateType & l_type)
{
	m_ToRemove.push_back(l_type);
}

bool StateManager::HasState(const StateType & l_type)
{
	for (std::vector<std::pair<StateType, BaseState*>>::value_type itr : m_Container)
	{
		if (itr.first == l_type)
		{
			for (std::vector<StateType>::value_type itr_1 : m_ToRemove)
			{
				if (itr_1 == l_type) return false;
			}
			return true;
		}
	}
	return false;
}

void StateManager::DeleteRemovingState()
{
	while (m_ToRemove.begin() != m_ToRemove.end())
	{
		StateType tmp = *m_ToRemove.begin();
		
		for (std::vector<std::pair<StateType, BaseState*>>::iterator itr = m_Container.begin(); 
				itr != m_Container.end(); itr++)
		{
			if (itr->first == tmp)
			{
				itr->second->OnDestroy();
				delete itr->second;
				m_Container.erase(itr);
				break;
			}
		}

		m_ToRemove.erase(m_ToRemove.begin());
	}
}
