#include"Character.h"

class Player : public Character
{
public:
	Player(EnityManager * l_entityMgr);
	~Player();
	
	void OnEntityCollision(EntityBase * l_collider, bool l_attack);
	void React(EventDetail * l_details);
};