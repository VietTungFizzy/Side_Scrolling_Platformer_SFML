#include "Map.h"

Map::Map(SharedContex * l_context) : 
	m_context(l_context) , m_currentState(nullptr),
	m_backgroundTexture("") ,m_gravity(0.0),
	m_loadNextMap(false) , m_nextMapName(""),
	m_defaultTile(l_context), m_tileName("")
{
	m_context->m_Map = this;
}

Map::~Map()
{
	PurgeMap();
	PurgeTileSet();
}

void Map::Update(float l_dt)
{
	if (m_loadNextMap)
	{
		PurgeMap();
		m_loadNextMap = false;
		if (m_nextMapName != "")
		{
			LoadMap("Configuration/Map/" + m_nextMapName);
		}
		else
		{
			//Switch state
		}
		m_nextMapName = "";
	}
	sf::FloatRect viewSpace = m_context->m_window->GetViewSpace();
	m_background.setPosition(viewSpace.left, viewSpace.top);
}

void Map::Draw()
{
	Window * window = m_context->m_window;
	sf::FloatRect viewSpace = m_context->m_window->GetViewSpace();
	
	window->Draw(m_background);

	sf::Vector2i tileBegin(floor(viewSpace.left / TILE_SIZE), floor(viewSpace.top / TILE_SIZE));
	sf::Vector2i tileEnd(ceil(viewSpace.width / TILE_SIZE), ceil(viewSpace.height / TILE_SIZE));

	for (int y = tileBegin.y; y < tileEnd.y; y++)
	{
		for (int x = tileBegin.x; x < tileEnd.y; x++)
		{
			if (x < 0 || y < 0) continue;
			Tile * tile = GetTile(x,y);
			if (tile == nullptr) continue;

			sf::Sprite tileSprite = tile->m_data->m_sprite;
			
			tileSprite.setPosition(x * TILE_SIZE, y * TILE_SIZE);
			window->Draw(tileSprite);
		}
	}
}

void Map::LoadMap(const std::string & l_path)
{
	std::ifstream file;
	file.open(Utils::GetWorkingDirection() + l_path);
	if (!file.is_open()) {
		std::cout << "! Failed loading map file: "
			<< Utils::GetWorkingDirection() << l_path << std::endl;
		return;
	}
	std::string line;
	while (std::getline(file, line))
	{
		if (line[0] == '|') { continue; }
		std::stringstream keystream(line);
		std::string type;
		keystream >> type;
		if (type == "TILE") {
			int tileId = 0;
			keystream >> tileId;
			if (tileId < 0) {
				std::cout << "! Bad tile id: "<< tileId << std::endl;
				continue;
			}
			std::unordered_map<unsigned int, TileInfo*>::iterator itr = m_tileSet.find(tileId);
			if (itr == m_tileSet.end()) {
				std::cout << "! Tile id(" << tileId << ") was not found in tileset." << std::endl;
				continue;
			}
			sf::Vector2i tileCoords;
			keystream >> tileCoords.x >> tileCoords.y;
			if (tileCoords.x > m_mapSize.x || tileCoords.y > m_mapSize.y)
			{
				std::cout << "! Tile is out of range: " <<
					tileCoords.x << " " << tileCoords.y << std::endl;
				continue;
			}
			Tile* tile = new Tile();
			// Bind properties of a tile from a set.
			tile->m_data = itr->second;
			unsigned int coordinate = tileCoords.x * m_mapSize.x + tileCoords.y;
			if (!m_tileMap.emplace(coordinate, tile).second)
			{
				// Duplicate tile detected!
				std::cout << "! Duplicate tile! : " << tileCoords.x
					<< "" << tileCoords.y << std::endl;
				delete tile;
				tile = nullptr;
				continue;
			}
			std::string wrap;
			keystream >> wrap;
			if (wrap != "") { tile->m_wrap = true; }
		}
		else if (type == "BACKGROUND") 
		{
			if (m_backgroundTexture != "") { continue; }
			keystream >> m_backgroundTexture;
			if (!m_context->m_TextureManager->
				RequireResource(m_backgroundTexture))
			{
				m_backgroundTexture = "";
				continue;
			}
			sf::Texture* texture = m_context->m_TextureManager->GetResource(m_backgroundTexture);
			m_background.setTexture(*texture);

			sf::Vector2f viewSize = (sf::Vector2f)m_context->m_window->GetWindowSize();
			sf::Vector2u textureSize = texture->getSize();
			sf::Vector2f scaleFactors;
			scaleFactors.x = viewSize.x / textureSize.x;
			scaleFactors.y = viewSize.y / textureSize.y;
			m_background.setScale(scaleFactors);
		}
		else if (type == "SIZE")
		{
			float x, y;
			keystream >> x >> y;
			if (x <= 0 || y <= 0) continue;
			m_mapSize = sf::Vector2f(x, y);
		}
		else if (type == "NEXTMAP")
		{
			keystream >> m_nextMapName;
		}
		else if (type == "GRAVITY")
		{
			keystream >> m_gravity;
		}
		else if (type == "DEFAULT_FRICTION")
		{
			keystream >> m_defaultTile.m_friction.x >> m_defaultTile.m_friction.y;
		}
	}
}

Tile * Map::GetTile(const unsigned int & x, const unsigned int & y)
{
	unsigned int coordinate = x * m_mapSize.x + y;
	std::unordered_map<unsigned int, Tile*>::iterator tmp = m_tileMap.find(coordinate);
	return (tmp != m_tileMap.end() ? tmp->second : nullptr);
}

void Map::PurgeMap()
{
	for (std::unordered_map<unsigned int, Tile*>::value_type & itr : m_tileMap)
	{
		delete itr.second;
	}
	m_tileMap.clear();
	
	//--->Delete Entity????

	if (m_backgroundTexture == "") return;
	m_context->m_TextureManager->ReleaseResource(m_backgroundTexture);
	m_backgroundTexture = "";
}

void Map::PurgeTileSet()
{
	for (std::unordered_map<unsigned int, TileInfo*>::value_type & itr : m_tileSet)
	{
		delete itr.second;
	}
	m_tileSet.clear();
}

void Map::LoadTile(const std::string & l_path)
{
	std::ifstream file;
	file.open(Utils::GetWorkingDirection() + l_path);
	if (!file.is_open()) {
		std::cout << "! Failed loading tile set file: "
			<< Utils::GetWorkingDirection() << l_path << std::endl;
		return;
	}
	std::string line;

	//Load texture name from file
	if (std::getline(file, line))
	{
		std::stringstream keystream(line);
		std::string type;
		keystream >> type;
		if (type == "TEXTURE")
		{
			if (m_tileName == "")
			{
				keystream >> m_tileName;
				if (m_context->m_TextureManager->RequireResource(m_tileName) == false)
				{
					m_tileName = "";
				}
			}
		}
	}


	while (std::getline(file, line)) 
	{
		if (line[0] == '|') { continue; }
		
		std::stringstream keystream(line);
		
		int tileId;
		keystream >> tileId;
		if (tileId < 0) { continue; }
		
		TileInfo* tile = new TileInfo(m_context, m_tileName , tileId);

		keystream >> tile->m_textureName >> tile->m_friction.x >> tile->m_friction.y;
		
		std::string deadly;
		keystream >> deadly;
		tile->m_deadly = false;
		if (deadly == "DEADLY") { tile->m_deadly = true; }

		if (!m_tileSet.emplace(tileId, tile).second) 
		{
			// Duplicate tile detected!
			std::cout << "! Duplicate tile type: "
				<< tile->m_textureName << std::endl;
			delete tile;
		}
	}
	file.close();
}

void Map::LoadNext()
{
	m_loadNextMap = true;
}
