#include"SpriteSheet.h"
#include"Anim_Directional.h"

SpriteSheet::SpriteSheet(TextureManager * l_textureMgr) : 
	m_textureMgr(l_textureMgr), m_SpriteScale(1.f,1.f),
	m_direction(Direction::Right),m_animationCurrent(nullptr)
{
}

SpriteSheet::~SpriteSheet()
{
	ReleaseSheet();
}

void SpriteSheet::CropSprite(const sf::IntRect & l_rect)
{
	m_sprite.setTextureRect(l_rect);
}

bool SpriteSheet::LoadSheet(const std::string & l_file)
{
	std::ifstream sheet;
	sheet.open(Utils::GetWorkingDirection() + l_file);
	if (!sheet.is_open())
	{
		std::cout << "! Failed to loading spritesheet: " << Utils::GetWorkingDirection() + l_file
			<< std::endl;
		return false;
	}
	ReleaseSheet();
	std::string line;
	while (std::getline(sheet, line))
	{
		if (line[0] == '|') continue;
		std::stringstream keystream(line);
		std::string type;
		keystream >> type;
		if (type == "TEXTURE")
		{
			if (m_textureName != "")
			{
				std::cout << "! Duplicate texture entries in: " << l_file << std::endl;
				continue;
			}
			std::string texture;
			keystream >> texture;
			if (!m_textureMgr->RequireResource(texture))
			{
				std::cout << "! Can't load texture " << texture << std::endl;
				continue;
			}
			m_textureName = texture;
			m_sprite.setTexture(*m_textureMgr->GetResource(texture));
		}
		else if (type == "SIZE")
		{
			keystream >> m_spriteSize.x >> m_spriteSize.y;
			SetSpriteSize(m_spriteSize);
		}
		else if (type == "SCALE")
		{
			keystream >> m_SpriteScale.x >> m_SpriteScale.y;
			m_sprite.setScale(m_SpriteScale);
		}
		else if (type == "ANIMATION_TYPE")
		{
			keystream >> m_animationType;
		}
		else if (type == "ANIMATION")
		{
			std::string name;
			keystream >> name;
			if (m_animations.find(name) != m_animations.end())
			{
				std::cout << "! Duplicate animation: " << name << std::endl;
				continue;
			}

			Anim_Base * newAnimation = nullptr;
			if (m_animationType == "Directional")
			{
				newAnimation = new Anim_Directional();
			}
			else
			{
				std::cout << "! Unknown animation type: " << m_animationType << std::endl;
				keystream;
			}
			keystream >> *newAnimation;
			newAnimation->SetSpriteSheet(this);
			newAnimation->m_name = name;
			newAnimation->Reset();

			m_animations.emplace(name, newAnimation);

			if (m_animationCurrent) continue;
			m_animationCurrent = newAnimation;
			m_animationCurrent->Play();
		}
	}
	std::cout << "Load success!!!!" << std::endl;
}

void SpriteSheet::ReleaseSheet()
{
	m_textureMgr->ReleaseResource(m_textureName);
	m_animationCurrent = nullptr;
	while(m_animations.begin() != m_animations.end())
	{
		delete m_animations.begin()->second;
		m_animations.erase(m_animations.begin());
	}
	
}

void SpriteSheet::SetSpriteSize(const sf::Vector2i & l_size)
{
	m_spriteSize = l_size;
	m_sprite.setOrigin(m_spriteSize.x / 2, m_spriteSize.y);
}

sf::Vector2i SpriteSheet::GetSpriteSize()
{
	return m_spriteSize;
}

void SpriteSheet::SetSpritePosition(const sf::Vector2f & l_pos)
{
	m_sprite.setPosition(l_pos);
}

void SpriteSheet::SetDirection(const Direction & l_dir)
{
	if (l_dir == m_direction) return;
	m_direction = l_dir;
	m_animationCurrent->CropSprite();
}

Direction SpriteSheet::GetDirection()
{
	return m_direction;
}

Anim_Base * SpriteSheet::GetCurrentAnim()
{
	return m_animationCurrent;
}

bool SpriteSheet::SetAnimation(const std::string & l_name, const bool & l_play, const bool & l_loop)
{
	std::unordered_map<std::string, Anim_Base*>::iterator itr = m_animations.find(l_name);
	if (itr == m_animations.end()) return false;
	if (itr->second == m_animationCurrent) return false;
	if (m_animationCurrent) m_animationCurrent->Stop();
	m_animationCurrent->SetLoop(l_loop);
	if (l_play) m_animationCurrent->Play();
	m_animationCurrent->CropSprite();
	return true;
}

void SpriteSheet::Update(float l_dt)
{
	m_animationCurrent->Update(l_dt);
}

void SpriteSheet::Draw(sf::RenderWindow * l_wind)
{
	l_wind->draw(m_sprite);
}
