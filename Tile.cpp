#include "Tile.h"



Tile::Tile(int id, int sUP, int sRIGHT, int sDOWN, int sLEFT, olc::Sprite* tileSprite, olc::PixelGameEngine* engine)
{
	m_engine = engine;
	m_sides.push_back(sUP);
	m_sides.push_back(sRIGHT);
	m_sides.push_back(sDOWN);
	m_sides.push_back(sLEFT);

	m_tileSprite = tileSprite;

	m_id = id;
}


Tile::~Tile()
{
}

void Tile::drawTile(int x,int y)
{
	m_engine->DrawSprite(x, y, m_tileSprite);
}

int Tile::id()
{
	return m_id;
}
