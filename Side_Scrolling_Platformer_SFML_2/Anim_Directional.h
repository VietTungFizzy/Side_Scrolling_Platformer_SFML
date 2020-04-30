#pragma once
#include "Anim_Base.h"
class Anim_Directional :
	public Anim_Base
{
public:
	Anim_Directional();
	~Anim_Directional();

	// Inherited via Anim_Base
	virtual void FrameStep() override;
	virtual void CropSprite() override;
	virtual void ReadIn(std::stringstream & l_stream) override;
};

