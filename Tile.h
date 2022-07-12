#pragma once
#include "Resources\olcPixelGameEngine.h"
#include <iostream>
#include <vector>
#include <string>



class Tile
{
public:
	std::vector<int> m_sides;
private:
	olc::Sprite* m_tileSprite;
	olc::PixelGameEngine* m_engine;
	
	int m_id;
public:
	Tile(int id,int sUP,int sRIGHT, int sDOWN, int sLEFT,  olc::Sprite* tileSprite, olc::PixelGameEngine* engine);
	~Tile();
	void drawTile(int x,int y);
	int id();
};

