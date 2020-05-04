#pragma once
#include<SFML/Graphics.hpp>
#include"SharedContext.h"
#include"BaseState.h"
#define TILE_SIZE 32
#define TILE_WIDTH 576
#define TILE_HEIGHT 32
struct TileInfo
{
	TileInfo(SharedContex * l_shared, std::string l_name = "", unsigned int l_id = 0) :
		m_shared(l_shared) , m_textureName(l_name) , m_id(l_id)
	{
		if (m_textureName == "") return;
		TextureManager * textureMgr = m_shared->m_TextureManager;
		if (textureMgr->RequireResource(m_textureName) == false) return;

		int index_x = m_id % (TILE_WIDTH / TILE_SIZE);
		int index_y = m_id / (TILE_WIDTH / TILE_SIZE);
		sf::IntRect spriteBound;
		spriteBound.left = index_x * TILE_SIZE;
		spriteBound.top = index_y * TILE_SIZE;
		spriteBound.width = TILE_SIZE;
		spriteBound.height = TILE_SIZE;

		m_sprite.setTextureRect(spriteBound);
		m_sprite.setTexture(*textureMgr->GetResource(m_textureName));
	}
	~TileInfo()
	{
		TextureManager * textureMgr = m_shared->m_TextureManager;
		textureMgr->ReleaseResource(m_textureName);
	}

	SharedContex * m_shared; //Access to Texture Manager
	sf::Sprite m_sprite;
	unsigned int m_id;
	std::string m_textureName;
	sf::Vector2f m_friction;

	bool m_deadly;
};
struct Tile
{
	TileInfo * m_data;
	bool m_wrap;
};

using TileMap = std::unordered_map<unsigned int, Tile*>;	//unsigned int --> coordinates in 1D
using TileSet = std::unordered_map<unsigned int, TileInfo*>; //unsigned int --> id

class Map
{
public:
	Map(SharedContex * l_context);
	~Map();
	void Update(float l_dt);
	void Draw();
	void LoadMap(const std::string & l_path);
	void LoadTile(const std::string & l_path);
	void LoadNext();

	//Getter and Setter
	sf::Vector2u GetMapSize();
	float GetGravity();
	TileInfo * GetDefaultTile();
	Tile * GetTile(const unsigned int & x, const unsigned int & y);

private:
	void PurgeMap();
	void PurgeTileSet();
private:
	TileMap m_tileMap;
	TileSet m_tileSet;
	TileInfo m_defaultTile; // Contain default properties for generic tile
	sf::Sprite m_background;
	float m_gravity;
	sf::Vector2f m_friction;
	sf::Vector2u m_mapSize;
	std::string m_nextMapName;
	std::string m_backgroundTexture;
	std::string m_tileName;
	SharedContex * m_context;
	BaseState * m_currentState;
	bool m_loadNextMap;
};

