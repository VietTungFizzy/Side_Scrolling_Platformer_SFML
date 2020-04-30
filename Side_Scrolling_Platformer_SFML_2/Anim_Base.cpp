#include "Anim_Base.h"
#include"SpriteSheet.h"


Anim_Base::Anim_Base() :
	m_frameCurrent(0), m_frameEnd(0),
	m_frameStart(0), m_frameRow(0),
	m_frameTime(0.f), m_elapsedTime(0.f),
	m_frameActionStart(-1), m_frameActionEnd(-1),
	m_loop(false),m_play(false)
{
}


Anim_Base::~Anim_Base()
{
}

void Anim_Base::Play()
{
	m_play = true;
}

void Anim_Base::Stop()
{
	m_play = false;
	Reset();
}

void Anim_Base::Reset()
{
	m_frameCurrent = m_frameStart;
	m_elapsedTime = 0.f;
	CropSprite();
}

void Anim_Base::Pause()
{
	m_play = false;
}

void Anim_Base::Update(const float & l_dt)
{
	if (!m_play) return;
	m_elapsedTime += l_dt;
	if (m_elapsedTime < m_frameTime) return;
	FrameStep();
	CropSprite();
	m_elapsedTime = 0.f;
}

void Anim_Base::SetSpriteSheet(SpriteSheet * l_sheet)
{
	m_spriteSheet = l_sheet;
}

void Anim_Base::SetFrame(const unsigned int & l_frame)
{
	if ((l_frame >= m_frameStart && l_frame <= m_frameEnd) ||	 //Animation go forward 
		(l_frame <= m_frameStart && l_frame >= m_frameEnd))		 //Animation go backward	
	{
		m_frameCurrent = l_frame;
	}
}

bool Anim_Base::IsInAction()
{
	//Any of these value are -1 ==> Action always perform
	if (m_frameActionEnd == -1 || m_frameActionStart == -1) return true;
	bool isWithinRanged = (m_frameCurrent <= m_frameActionEnd && m_frameCurrent >= m_frameActionStart);
	return isWithinRanged;
}
