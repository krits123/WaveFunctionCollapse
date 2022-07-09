#pragma once
#include "Resources\olcPixelGameEngine.h"
#include "Tile.h"
class Cell
{
public:
	int m_totalOptions;
	std::vector<int> m_options;
	Tile* m_tile;
	int m_posx;
	int m_posy;
	int m_x;
	int m_y;

private:
	int m_width;
	int m_height;
	bool m_isDrawn;
	
	
	olc::PixelGameEngine* m_engine;
	
public:
	Cell(int x_,int y_,int x, int y, int w, int h, int totalOptions, olc::PixelGameEngine* engine);
	Cell(int x_, int y_,int x, int y, int w, int h, Tile* tile, int tileId, int totalOptions, olc::PixelGameEngine* engine);
	~Cell();
	void drawCell(olc::Pixel c);
	void removeOption(int opt);
	int entropy();
	void pickRandOption(std::vector<Tile*> tiles);
};

