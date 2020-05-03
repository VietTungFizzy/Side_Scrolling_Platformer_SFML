#pragma once
#include"Map.h"
#include"Direction.h"
#include"SpriteSheet.h"

struct CollisionElement
{
	CollisionElement(float l_area,TileInfo * l_CollisonTile,const sf::FloatRect & l_tileBounds ) :
	m_area(l_area), m_CollisonTile(l_CollisonTile), m_tileBounds(l_tileBounds) {}
	sf::FloatRect m_tileBounds;
	float m_area;
	TileInfo * m_CollisonTile;	
};
using Collisions = std::vector<CollisionElement>;

enum class EntityType { Base, Enemy, Player };
enum class EntityState {
	Idle, Walking, Jumping, Attacking, Hurt, Dying
};
class EntityManager;
class EntityBase{
	friend class EntityManager;
public:
	EntityBase(EntityManager * l_entityMgr);
	virtual ~EntityBase();
	//Getter and setter
	void SetPosition(const float & l_x, const float & l_y);
	void SetPosition(const sf::Vector2f & l_pos);
	void SetSize(const float & l_x, const float & l_y);
	void SetState(const EntityState & l_state);
	//other function
	void Move(float l_x, float l_y);
	void AddVelocity(float l_x, float l_y);
	void Accelerate(float l_x, float l_y);
	void SetAcceleration(float l_x, float l_y);
	void ApplyFriction(float l_x, float l_y);
	virtual void Update(float l_dt);
	virtual void Draw(sf::RenderWindow * l_window )=0;
protected:
	//Methods
	void UpdateAABB();
	void CheckCollision();
	void ResolveCollision();

	//Method for what THIS entity does TO the l_collider entity
	virtual void OnEntityCollision(EntityBase * l_collider, bool l_attack) =0;

protected:
	std::string m_name;
	EntityType m_type;
	unsigned int m_id; //Entity id in the entity manager
	sf::Vector2f m_position; // Current position
	sf::Vector2f m_positionOld; //Position before entity moved
	sf::Vector2f m_velocity; // Current velocity
	sf::Vector2f m_maxVelocity; // Maximum velocity
	sf::Vector2f m_speed; // Value of acceleration
	sf::Vector2f m_acceleration; // Current acceleration
	sf::Vector2f m_friction; // Friction value
	TileInfo * m_referenceTile; //Tile underneath entity
	sf::Vector2f m_size; // Size of collision box
	sf::FloatRect m_AABB; // The bounding box for collision
	EntityState m_currentState; // Current entity state

	//Flags for remembering axis collisions
	bool m_collidingX;
	bool m_collidingY;

	Collisions m_collisions;
	EntityManager * m_entityMgr;
};