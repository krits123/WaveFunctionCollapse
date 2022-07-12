#include "Cell.h"



Cell::Cell(int x_, int y_,int x,int y,int w,int h, int totalOptions,olc::PixelGameEngine* engine)
{
	m_isDrawn = false;
	m_height = h;
	m_width = w;
	m_posx = x;
	m_posy = y;
	m_x = x_;
	m_y = y_;
	m_engine = engine;

	m_totalOptions = totalOptions;

	for (int i = 0;i < totalOptions;i++)
		m_options.push_back(i);

	m_tile = nullptr;

}


Cell::Cell(int x_, int y_,int x, int y, int w, int h,Tile *tile ,int tileId,int totalOptions,olc::PixelGameEngine* engine)
{
	m_isDrawn = false;
	m_height = h;
	m_width = w;
	m_posx = x;
	m_posy = y;
	m_x = x_;
	m_y = y_;
	m_engine = engine;

	m_totalOptions = totalOptions;

	m_options.push_back(tileId);

	m_tile = tile;
}

Cell::~Cell()
{
}

void Cell::drawCell(olc::Pixel c)
{

	if (m_isDrawn){
		m_engine->DrawRect(m_posx, m_posy, m_width, m_height, c);
		return;
	}
	if (m_tile) {
		m_tile->drawTile(m_posx, m_posy);
		m_isDrawn = true;
	}
	m_engine->DrawRect(m_posx, m_posy, m_width, m_height, c);
	return;
}

void Cell::removeOption(int opt) {
	std::vector<int>::iterator position = std::find(m_options.begin(), m_options.end(), opt);
	if (position != m_options.end()) // == myVector.end() means the element was not found
		m_options.erase(position);
}

int Cell::entropy()
{
	if (m_tile)
		return INT_MAX;
	else
		return m_options.size();
}

void Cell::pickRandOption(std::vector<Tile*> tiles)
{
	if (m_options.size() == 0) {
		m_tile = tiles[0];
		m_options.push_back(0);
	}
	else if(m_options.size() == 1)
		m_tile = tiles[m_options[0]];
	else {
		int idx = rand() % m_options.size();
		int tmp = m_options[idx];
		m_tile = tiles[tmp];
		m_options.clear();
		m_options.push_back(tmp);
	}
}
