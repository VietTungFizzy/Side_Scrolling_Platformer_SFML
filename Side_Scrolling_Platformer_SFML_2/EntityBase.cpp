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
	m_acceleration += sf::Vector2f(l_x,l_y);
}

void EntityBase::SetAcceleration(float l_x, float l_y)
{
	m_acceleration = sf::Vector2f(l_x, l_y);
}

void EntityBase::ApplyFriction(float l_x, float l_y)
{
	if (m_velocity.x != 0)
	{
		if (std::abs(m_velocity.x) - std::abs(l_x) < 0) { m_velocity.x = 0; }
		else
		{
			if (m_velocity.x < 0) m_velocity.x += l_x;
			else m_velocity.x -= l_x;
		}
	}

	if (m_velocity.y != 0)
	{
		if (std::abs(m_velocity.y) - std::abs(l_y) < 0) { m_velocity.y = 0; }
		else
		{
			if (m_velocity.y < 0) m_velocity.y += l_x;
			else m_velocity.y -= l_y;
		}
	}
}

void EntityBase::Update(float l_dt)
{
	//Obtain data for calculate friction
	Map * map = m_entityMgr->GetContext()->m_Map;
	float gravity = map->GetGravity();
	Accelerate(0, gravity);
	AddVelocity(m_acceleration.x * l_dt, m_acceleration.y * l_dt);
	SetAcceleration(0.f, 0.f);

	//Calculate friction
	sf::Vector2f frictionValue;
	if (m_referenceTile)
	{
		frictionValue = m_referenceTile->m_friction;
		if (m_referenceTile->m_deadly) SetState(EntityState::Dying);
	}
	else if (map->GetDefaultTile())
	{
		frictionValue = map->GetDefaultTile()->m_friction;
	}
	else
	{
		frictionValue = m_friction;
	}

	float friction_x = (m_speed.x * frictionValue.x) * l_dt;
	float friction_y = (m_speed.y * frictionValue.y) * l_dt;

	//Apply friction to update Entity's position
	ApplyFriction(friction_x, friction_y);
	sf::Vector2f deltaPos = m_velocity * l_dt;
	Move(deltaPos.x, deltaPos.y);

	//Checking collision
	m_collidingX = false;
	m_collidingY = false;
	CheckCollision();
	ResolveCollision();
}

void EntityBase::UpdateAABB()
{
	//the origin of the bounding box is left at the top-left corner and the entity's
	//position is set to (width / 2, height)
	m_AABB = sf::FloatRect(	m_position.x - (m_size.x / 2),
							m_position.y - m_size.y,
							m_size.x,
							m_size.y);
}

void EntityBase::CheckCollision()
{
	Map * map = m_entityMgr->GetContext()->m_Map;
	
	int fromX = std::floor(m_AABB.left / TILE_SIZE);
	int toX = std::floor((m_AABB.left + m_AABB.width) / TILE_SIZE);
	int fromY = std::floor(m_AABB.top /TILE_SIZE);
	int toY = std::floor((m_AABB.top + m_AABB.height) / TILE_SIZE);

	for (int x = fromX; x <= toX; x++)
	{
		for (int y = fromY; y <= toY; y++)
		{
			Tile * tile = map->GetTile(x, y);
			if (!tile) continue;
			sf::FloatRect tileBounds(x * TILE_SIZE, y*TILE_SIZE, TILE_SIZE, TILE_SIZE);
			sf::FloatRect intersection;

			m_AABB.intersects(tileBounds, intersection);
			float area = intersection.width * intersection.height;

			CollisionElement e(area, tile->m_data, tileBounds);
			m_collisions.emplace_back(e);

			if (tile->m_wrap && m_type == EntityType::Player)
			{
				map->LoadNext();
			}
		}
	}
}

void EntityBase::ResolveCollision()
{
}

bool SortCollisions(const CollisionElement & l_1, const CollisionElement & l_2)
{
	return l_1.m_area > l_2.m_area;
}
