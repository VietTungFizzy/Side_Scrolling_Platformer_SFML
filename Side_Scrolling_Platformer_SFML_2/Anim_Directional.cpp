#include "Anim_Directional.h"
#include"SpriteSheet.h"


Anim_Directional::Anim_Directional()
{
}


Anim_Directional::~Anim_Directional()
{
}

void Anim_Directional::FrameStep()
{
	//Check whether the animation go forward or backward
	if (m_frameStart < m_frameEnd) m_frameCurrent++;
	else m_frameCurrent--;

	if ((m_frameStart > m_frameEnd && m_frameCurrent < m_frameEnd) || //Out of bound in go-backward animation
		(m_frameStart < m_frameEnd && m_frameCurrent > m_frameEnd)) //Out of bound in go-forward animation
	{
		if (m_loop)
		{
			m_frameCurrent = m_frameStart;
			return;
		}

		m_frameCurrent = m_frameEnd;
		Pause();
	}
}

void Anim_Directional::CropSprite()
{
	sf::Vector2i spriteSize = m_spriteSheet->GetSpriteSize();
	sf::IntRect rect(
		spriteSize.x * m_frameCurrent,
		spriteSize.y * (m_frameRow + (short)m_spriteSheet->GetDirection()),
		spriteSize.x,
		spriteSize.y
	);

	m_spriteSheet->CropSprite(rect);
}

void Anim_Directional::ReadIn(std::stringstream & l_stream)
{
	l_stream >> m_frameStart
		>> m_frameEnd
		>> m_frameRow
		>> m_frameTime
		>> m_frameActionStart
		>> m_frameActionEnd;
}
