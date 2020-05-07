#include "EntityManager.h"
#include"Player.h"

EntityManager::EntityManager(SharedContex * l_context, unsigned int l_maxEntity) :
	m_context(l_context), m_maxEntity(l_maxEntity), m_idCounter(0)
{
	//LoadEnemyType("EnemyList.list");
	//RegisterEntity<Enemy>(EntityType::Enemy);
	RegisterEntity<Player>(EntityType::Player);
}

EntityManager::~EntityManager()
{
	Purge();
}

void EntityManager::Add(const EntityType & l_type, const std::string & l_name)
{
}

EntityBase * EntityManager::Find(unsigned int l_id)
{
	return nullptr;
}

EntityBase * EntityManager::Fine(const std::string & l_name)
{
	return nullptr;
}

void EntityManager::Remove(unsigned int l_id)
{
}

void EntityManager::Update(float l_dt)
{
}

void EntityManager::Draw()
{
}

void EntityManager::Purge()
{
}

SharedContex * EntityManager::GetContext()
{
	return nullptr;
}

void EntityManager::ProcessRemovals()
{
}

void EntityManager::LoadEnemyType(const std::string & l_name)
{
}

void EntityManager::EntityCollisionCheck()
{
}
