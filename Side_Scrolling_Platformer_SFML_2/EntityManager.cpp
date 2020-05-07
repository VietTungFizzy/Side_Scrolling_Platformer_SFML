#include "EntityManager.h"
#include"Player.h"
#include"Enemy.h"

EntityManager::EntityManager(SharedContex * l_context, unsigned int l_maxEntity) :
	m_context(l_context), m_maxEntity(l_maxEntity), m_idCounter(0)
{
	LoadEnemyType("EnemyList.list");
	RegisterEntity<Enemy>(EntityType::Enemy);
	RegisterEntity<Player>(EntityType::Player);
}

EntityManager::~EntityManager()
{
	Purge();
}

int EntityManager::Add(const EntityType & l_type, const std::string & l_name)
{
	std::unordered_map<EntityType, std::function<EntityBase*(void)>>::iterator itr = m_entityFactory.find(l_type);
	if (itr == m_entityFactory.end())
	{
		std::cout << "Can't find entity type in entity factory\n";
		return -1;
	}
	EntityBase * entity = itr->second();
	entity->m_id = m_idCounter;
	if (l_name != "") entity->m_name = l_name;
	m_entities.emplace(m_idCounter, entity);
	if (l_type == EntityType::Enemy)
	{
		std::unordered_map<std::string, std::string>::iterator itr = m_enemyTypes.find(l_name);
		if (itr != m_enemyTypes.end())
		{
			Enemy * enemy = (Enemy * )entity;
			enemy->Load(itr->second);
		}
	}

	m_idCounter++;
	return m_idCounter - 1;
}

EntityBase * EntityManager::Find(unsigned int l_id)
{
	std::unordered_map<unsigned int, EntityBase *>::iterator itr = m_entities.find(l_id);
	if(itr == m_entities.end()) return nullptr;
	return itr->second;
}

EntityBase * EntityManager::Fine(const std::string & l_name)
{
	for (std::unordered_map<unsigned int, EntityBase *>::value_type & itr : m_entities)
	{
		if (itr.second->m_name == l_name) return itr.second;
	}
	return nullptr;
}

void EntityManager::Remove(unsigned int l_id)
{
	m_entitiesToRemove.emplace_back(l_id);
}

void EntityManager::Update(float l_dt)
{
	for (std::unordered_map<unsigned int, EntityBase *>::value_type & itr : m_entities)
	{
		itr.second->Update(l_dt);
	}
	EntityCollisionCheck();
	ProcessRemovals();
}

void EntityManager::Draw()
{
	sf::RenderWindow * window = m_context->m_window->GetWindowRender();
	sf::FloatRect viewSpace = m_context->m_window->GetViewSpace();
	for (std::unordered_map<unsigned int, EntityBase *>::value_type & itr : m_entities)
	{
		if (!viewSpace.intersects(itr.second->m_AABB)) continue;
		itr.second->Draw(window);
	}
	
}

void EntityManager::Purge()
{
	for (std::unordered_map<unsigned int, EntityBase *>::value_type & itr : m_entities)
	{
		delete itr.second;
	}
	m_entities.clear();
	m_idCounter = 0;
}

SharedContex * EntityManager::GetContext()
{
	return m_context;
}

void EntityManager::ProcessRemovals()
{
	while (m_entitiesToRemove.begin() != m_entitiesToRemove.end())
	{
		unsigned int id = m_entitiesToRemove.back();
		std::unordered_map<unsigned int, EntityBase *>::iterator itr = m_entities.find(id);
		if (itr != m_entities.end())
		{
			std::cout << "Discarding entity id[" << itr->second->m_id << "]\n";
			delete itr->second;
			m_entities.erase(itr);
		}
		m_entitiesToRemove.pop_back();
	}
	
}

void EntityManager::LoadEnemyType(const std::string & l_name)
{
}

void EntityManager::EntityCollisionCheck()
{
	if (m_entities.empty()) return;
	for (std::unordered_map<unsigned int, EntityBase *>::iterator itr = m_entities.begin();
		itr != m_entities.end(); itr++)
	{
		for (std::unordered_map<unsigned int, EntityBase *>::iterator itr1 = std::next(itr);
			itr1 != m_entities.end(); itr1++)
		{
			if (itr->first == itr1->first) continue;

			//Regular bounding box collision
			if (itr->second->m_AABB.intersects(itr1->second->m_AABB))
			{
				itr->second->OnEntityCollision(itr1->second, false);
				itr1->second->OnEntityCollision(itr->second, false);
			}
			EntityType itr_type = itr->second->GetType();
			EntityType itr1_type = itr1->second->GetType();

			if (itr_type == EntityType::Enemy || itr_type == EntityType::Player)
			{
				Character * character = (Character *)itr->second;
				if (character->m_attackAABB.intersects(itr1->second->m_AABB))
				{
					character->OnEntityCollision(itr1->second, true);
				}
			}

			if (itr1_type == EntityType::Player || itr1_type == EntityType::Enemy)
			{
				Character * character = (Character *)itr1->second;
				if (character->m_attackAABB.intersects(itr1->second->m_AABB))
				{
					character->OnEntityCollision(itr->second, true);
				}
			}
		}

	}
}
