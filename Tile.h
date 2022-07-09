#pragma once
#include "Resources\olcPixelGameEngine.h"
#include <iostream>
#include <vector>
#include <string>



class Tile
{
public:
	std::vector<std::string> m_sides;
private:
	olc::Sprite* m_tileSprite;
	olc::PixelGameEngine* m_engine;
	
	int m_id;
public:
	Tile(int id,std::string sUP, std::string sRIGHT, std::string sDOWN, std::string sLEFT,  olc::Sprite* tileSprite, olc::PixelGameEngine* engine);
	~Tile();
	void drawTile(int x,int y);
	int id();
};

