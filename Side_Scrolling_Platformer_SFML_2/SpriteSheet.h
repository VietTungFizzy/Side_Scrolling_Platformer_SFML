#include"SharedContext.h"
#include"Direction.h"

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
	
	// Not needed yet

	/*
	Animations m_animations;
	Anim_Base * m_animationCurrent;
	*/

public: // Inteface functions
	SpriteSheet(TextureManager * l_textureMgr);
	~SpriteSheet();

	void CropSprite(const sf::IntRect & l_rect);
	bool LoadSheet(const std::string & l_file);
	void ReleaseSheet();

	//Not needed yet
	/*
	Anim_Base * GetCurrentAnim();
	bool SetAnimation(const std::string  & l_name,
					const bool & l_play = false,
					const bool & l_loop = false)
	*/

	void Update(float l_dt);
	void Draw(sf::RenderWindow * l_wind);
};