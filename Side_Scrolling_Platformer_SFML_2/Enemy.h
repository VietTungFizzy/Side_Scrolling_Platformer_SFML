#pragma once
#include "Character.h"
class Enemy :
	public Character
{
	// Inherited via Character
	virtual void OnEntityCollision(EntityBase * l_collider, bool l_attack) override;
};

