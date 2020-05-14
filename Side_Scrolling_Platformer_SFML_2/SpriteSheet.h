#pragma once
#include"SharedContext.h"
#include"Direction.h"
#include"Anim_Base.h"

using Animations = std::unordered_map<std::string, Anim_Base *>;
class SpriteSheet
{
private: //Data members
	std::string  m_textureName;
	sf::Sprite m_sprite;
	sf::Vector2i m_spriteSize;
	sf::Vector2f m_SpriteScale;
	Direction m_direction;
	std::string m_animationType;
	TextureManager * m_textureMgr;
	

	Animations m_animations;
	Anim_Base * m_animationCurrent;

public:

public: // Inteface functions
	SpriteSheet(TextureManager * l_textureMgr);
	~SpriteSheet();

	void CropSprite(const sf::IntRect & l_rect);
	bool LoadSheet(const std::string & l_file);
	void ReleaseSheet();

	void SetSpriteSize(const sf::Vector2i & l_size);
	sf::Vector2i GetSpriteSize();
	void SetSpritePosition(const sf::Vector2f & l_pos);
	void SetDirection(const Direction & l_dir);
	Direction GetDirection();
	
	Anim_Base * GetCurrentAnim();
	bool SetAnimation(const std::string  & l_name,
		const bool & l_play = false,
		const bool & l_loop = false);

	void Update(float l_dt);
	void Draw(sf::RenderWindow * l_wind);
};