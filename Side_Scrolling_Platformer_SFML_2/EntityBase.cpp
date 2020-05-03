#include"EntityBase.h"
#include"EntityManager.h"

EntityBase::EntityBase(EntityManager * l_entityMgr) : 
	m_entityMgr(l_entityMgr), m_name("BaseEntity"),
	m_type(EntityType::Base), m_referenceTile(nullptr),
	m_currentState(EntityState::Idle), m_id(0),
	m_collidingX(false), m_collidingY(false)
{
}

EntityBase::~EntityBase()
{
}

void EntityBase::SetPosition(const float & l_x, const float & l_y)
{
	m_position = sf::Vector2f(l_x, l_y);
	UpdateAABB();
}

void EntityBase::SetPosition(const sf::Vector2f & l_pos)
{
	m_position = l_pos;
	UpdateAABB();
}

void EntityBase::SetSize(const float & l_x, const float & l_y)
{
	m_size = sf::Vector2f(l_x, l_y);
	UpdateAABB();
}

void EntityBase::SetState(const EntityState & l_state)
{
	if (m_currentState == EntityState::Dying) return;
	m_currentState = l_state;
}

void EntityBase::Move(float l_x, float l_y)
{
	m_position = m_positionOld;
	m_position += sf::Vector2f(l_x, l_y);
	sf::Vector2u mapSize = m_entityMgr->GetContext()->m_Map->GetMapSize();
	if (m_position.x < 0)
	{
		m_position.x = 0;
	}
	else if (m_position.x > (mapSize.x + 1) * TILE_SIZE)
	{
		m_position.x = (mapSize.x + 1) * TILE_SIZE;
	}

	if (m_position.y < 0)
	{
		m_position.y = 0;
	}
	else if (m_position.y > (mapSize.y + 1) * TILE_SIZE)
	{
		m_position.y = (mapSize.y + 1) * TILE_SIZE;
		SetState(EntityState::Dying);
	}

	UpdateAABB();
}

void EntityBase::AddVelocity(float l_x, float l_y)
{
	m_velocity += sf::Vector2f(l_x, l_y);
	if (std::abs(m_velocity.x) > m_maxVelocity.x)
	{
		if (m_velocity.x < 0) m_velocity.x = -m_maxVelocity.x;
		else m_velocity.x = m_maxVelocity.x;
	}

	if (std::abs(m_velocity.y) > m_maxVelocity.y)
	{
		if (m_velocity.y < 0) m_velocity.y = -m_maxVelocity.y;
		else m_velocity.y = m_maxVelocity.y;
	}
}

void EntityBase::Accelerate(float l_x, float l_y)
{
}

void EntityBase::SetAcceleration(float l_x, float l_y)
{
}

void EntityBase::ApplyFriction(float l_x, float l_y)
{
}

void EntityBase::Update(float l_dt)
{
}

void EntityBase::UpdateAABB()
{
}

void EntityBase::CheckCollision()
{
}

void EntityBase::ResolveCollision()
{
}
