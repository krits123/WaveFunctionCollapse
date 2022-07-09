#define OLC_PGE_APPLICATION
#include "Resources\olcPixelGameEngine.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include "Cell.h"
#include "Tile.h"
#include <cstdlib>
#include <time.h>

#define WIDTH  952
#define HEIGHT  952

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

// TODO:
// - optimize options selection
// - maybe use better data strucutres to store options 
// - maybe avoid sorting at each loop
// - Could use results of previous sorting 
// - Consider searching in options with binary search since its most ilkely sorted by design
// - Maybe options are stored in a list for easy removal and finding 
// - Implement Backtracking for non-complete sets of tiles
// - Define Width and height and tile size better
// - Tile and Cell Classes have redudant and useless info, make them minimal and better
// - Maybe don t draw everything from the start on each loop || DONE
// - Draw something if it was never drawn before
// - Implement roattion of encoding 
// - Don t roatet everything some tiles have 90 or 180 rotatinal symmetry
// - Implement rap around or Define stoping blocks for the edges
// - Better use of setters and getters for classes not every member shoyld be public
// - Tidy up updateNeibsEntropy(x,y) through better acces of cell options and tiles encoding
// - FPS is too low should reach at least 15 - 20 (or >30) || DONE 
// - Move the code on github and on a seperate project 
// - Check if tile pngs exist
// - Store them in the tiles vector with an automatic way

bool cmpFun(Cell* c1, Cell* c2) {
	return (c1->entropy() < c2->entropy());
}
class Generator : public olc::PixelGameEngine
{
public:
	Generator()
	{
		sAppName = "Wave Function Collapse Basic Tilling";
	}
private:
	olc::Sprite* spr0;
	olc::Sprite* spr1;
	olc::Sprite* spr2;
	olc::Sprite* spr3;
	olc::Sprite* spr4;
	olc::Sprite* spr5;
	olc::Sprite* spr6;
	olc::Sprite* spr7;
	olc::Sprite* spr8;


	std::vector<olc::Sprite*> sprs;

	int tileSize = 56;
	int gridSize;

	std::vector<Cell*> cellGrid;
	std::vector<Tile*> tiles;

	void rotateSprite90(olc::Sprite* spr1, olc::Sprite* spr2) {

		if (spr1->width != spr1->height)
			return;
		int w = spr1->width;
		std::cout << spr2->width << std::endl;

		std::cout << spr2->height << std::endl;
		for (int i = 0;i < spr1->width;i++) {
			for (int j = 0;j < spr1->height;j++) {

				spr2->SetPixel(w - j, i, spr1->GetPixel(i, j));
			}
		}
	}

	void rotateSprite180(olc::Sprite* spr1, olc::Sprite* spr2) {

		if (spr1->width != spr1->height)
			return;
		int w = spr1->width;
		std::cout << spr2->width << std::endl;

		std::cout << spr2->height << std::endl;
		for (int i = 0;i < spr1->width;i++) {
			for (int j = 0;j < spr1->height;j++) {

				spr2->SetPixel(w - i, w - j, spr1->GetPixel(i, j));
			}
		}
	}

	void rotateSprite270(olc::Sprite* spr1, olc::Sprite* spr2) {

		if (spr1->width != spr1->height)
			return;
		int w = spr1->width;
		std::cout << spr2->width << std::endl;

		std::cout << spr2->height << std::endl;
		for (int i = 0;i < spr1->width;i++) {
			for (int j = 0;j < spr1->height;j++) {

				spr2->SetPixel(j, w - i, spr1->GetPixel(i, j));
			}
		}
	}

	int indexOfCell(int x, int y) {
		return (y) + (x)* gridSize;
	}

	void updateNeibsEntropy(int x, int y) {

		// TODO:
		// Implement rap around or Define stoping blocks for the edges


		// Update Top Neibhor if he exists
		if (y > 0) {

			int idx = indexOfCell(x, y - 1);
			Cell* cellAdj = cellGrid[idx];

			std::vector<int> toRemove;
			std::string target = cellGrid[indexOfCell(x, y)]->m_tile->m_sides[UP];

			for (int i = 0;i < cellAdj->m_options.size();i++) {

				if (   tiles[cellAdj->m_options[i]]->m_sides[DOWN] != target    )
					toRemove.push_back(cellAdj->m_options[i]);
			}


			for (int i = 0;i < toRemove.size();i++) {
				cellAdj->removeOption(toRemove[i]);
			}

		}


		// Update Right Neibhor if He exists
		if (x < gridSize-1) {

			int idx = indexOfCell(x+1, y );
			Cell * cellAdj = cellGrid[idx];

			std::vector<int> toRemove;
			std::string target = cellGrid[indexOfCell(x, y)]->m_tile->m_sides[RIGHT];

			for (int i = 0;i < cellAdj->m_options.size();i++) {

				if (tiles[cellAdj->m_options[i]]->m_sides[LEFT] != target)
					toRemove.push_back(cellAdj->m_options[i]);
			}


			for (int i = 0;i < toRemove.size();i++) {
				cellAdj->removeOption(toRemove[i]);
			}

		}

		// Update Down Neibhor if He exists
		if (y < gridSize - 1) {

			int idx = indexOfCell(x, y+1);
			Cell* cellAdj = cellGrid[idx];

			std::vector<int> toRemove;
			std::string target = cellGrid[indexOfCell(x, y)]->m_tile->m_sides[DOWN];

			for (int i = 0;i < cellAdj->m_options.size();i++) {

				if (tiles[cellAdj->m_options[i]]->m_sides[UP] != target)
					toRemove.push_back(cellAdj->m_options[i]);
			}


			for (int i = 0;i < toRemove.size();i++) {
				cellAdj->removeOption(toRemove[i]);
			}

		}


		// Update Down Neibhor if He exists
		if (x>0) {

			int idx = indexOfCell(x-1, y);
			Cell * cellAdj = cellGrid[idx];

			std::vector<int> toRemove;
			std::string target = cellGrid[indexOfCell(x, y)]->m_tile->m_sides[LEFT];

			for (int i = 0;i < cellAdj->m_options.size();i++) {

				if (tiles[cellAdj->m_options[i]]->m_sides[RIGHT] != target)
					toRemove.push_back(cellAdj->m_options[i]);
			}


			for (int i = 0;i < toRemove.size();i++) {
				cellAdj->removeOption(toRemove[i]);
			}

		}

	}


public:






	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		srand(time(0));

		std::string s0 = "000";
		std::string s1 = "010";

		// Load Tiles and Rotate the Sprites and Store them
		/*spr0 = new olc::Sprite("tile0.png");

		spr1 = new olc::Sprite("tile1.png");
		spr2 = new olc::Sprite("tile1.png");
		spr3 = new olc::Sprite("tile1.png");
		spr4 = new olc::Sprite("tile1.png");

		spr5 = new olc::Sprite("tile2.png");
		spr6 = new olc::Sprite("tile2.png");
		spr7 = new olc::Sprite("tile2.png");
		spr8 = new olc::Sprite("tile2.png");

		rotateSprite90(spr1, spr2);
		rotateSprite180(spr1, spr3);
		rotateSprite270(spr1, spr4);

		rotateSprite90(spr5, spr6);
		rotateSprite180(spr5, spr7);
		rotateSprite270(spr5, spr8);

		// Create the Tile Objects and Store the Side codes

		// BLANK
		tiles.push_back(new Tile(0,s0, s0, s0, s0, spr0, this));

		// UP
		tiles.push_back(new Tile(1,s1, s1, s0, s1, spr1, this));

		// RIGHT
		tiles.push_back(new Tile(2,s1, s1, s1, s0, spr2, this));

		// DOWN
		tiles.push_back(new Tile(3,s0, s1, s1, s1, spr3, this));

		// LEFT
		tiles.push_back(new Tile(4,s1, s0, s1, s1, spr4, this));

		// UP
		tiles.push_back(new Tile(5, s1, s0, s0, s1, spr5, this));

		// RIGHT
		tiles.push_back(new Tile(6, s1, s1, s0, s0, spr6, this));

		// DOWN
		tiles.push_back(new Tile(7, s0, s1, s1, s0, spr7, this));

		// LEFT
		tiles.push_back(new Tile(8, s0, s0, s1, s1, spr8, this));
		*/

		sprs.push_back(new olc::Sprite("circuit\\0.png"));
		sprs.push_back(new olc::Sprite("circuit\\1.png"));

		for (int i = 2; i < 13;i++) {
			std::string s =  std::to_string(i);

			

			olc::Sprite *spr1 = new olc::Sprite( "circuit\\" + s+ ".png");
			olc::Sprite *spr2 = new olc::Sprite("circuit\\" + s + ".png");
			olc::Sprite *spr3 = new olc::Sprite("circuit\\" + s + ".png");
			olc::Sprite *spr4 = new olc::Sprite("circuit\\" + s + ".png");


			
			rotateSprite90(spr1, spr2);
			rotateSprite180(spr1, spr3);
			rotateSprite270(spr1, spr4);

			sprs.push_back(spr1);
			sprs.push_back(spr2);
			sprs.push_back(spr3);
			sprs.push_back(spr4);
			
		}
		// TODO:
		// This is extremly BAD!!!!
		// I need to HARDCODE the side encoding 
		// SHould be done with a confige FILE
		std::string G = "GGG";

		std::string BBB= "BBB";
		// 2 Blanks 
		tiles.push_back(new Tile(0, BBB, BBB, BBB, BBB, sprs[0], this));
		tiles.push_back(new Tile(1, G, G, G, G, sprs[1], this));



		// Tile 2 
		std::string GLG = "GLG";
		
		tiles.push_back(new Tile(3, G, GLG, G, G, sprs[2], this));
		tiles.push_back(new Tile(4, G, G, GLG, G, sprs[3], this));
		tiles.push_back(new Tile(5, G, G, G, GLG, sprs[4], this));
		tiles.push_back(new Tile(6, GLG, G, G, G, sprs[5], this));


		// Tile 3 
		std::string GAG = "GAG";

		tiles.push_back(new Tile(7, G, GAG, G, GAG, sprs[6], this));
		tiles.push_back(new Tile(8, GAG, G, GAG, G, sprs[7], this));
		tiles.push_back(new Tile(9, G, GAG, G, GAG, sprs[8], this));
		tiles.push_back(new Tile(10, GAG, G, GAG, G, sprs[9], this));



		// Tile 4
		std::string BGG = "BGG";
		std::string GGB = "GGB";

		tiles.push_back(new Tile(11, BGG, GLG, BGG,BBB, sprs[10], this));
		tiles.push_back(new Tile(12, BBB, BGG, GLG, BGG, sprs[11], this));
		tiles.push_back(new Tile(13, GGB, BBB, GGB, GLG, sprs[12], this));
		tiles.push_back(new Tile(14, GLG, GGB, BBB, GGB, sprs[13], this));

		// Tile 5
		
		tiles.push_back(new Tile(15, BGG, G, G, BGG, sprs[14], this));
		tiles.push_back(new Tile(16, GGB, BGG, G,G, sprs[15], this));
		tiles.push_back(new Tile(17, G, GGB, GGB, G, sprs[16], this));
		tiles.push_back(new Tile(18, G, G, BGG, GGB, sprs[17], this));

		// Tile 6

		tiles.push_back(new Tile(19, G, GLG, G, GLG, sprs[18], this));
		tiles.push_back(new Tile(20, GLG, G, GLG, G, sprs[19], this));
		tiles.push_back(new Tile(21, G, GLG, G, GLG, sprs[20], this));
		tiles.push_back(new Tile(22, GLG, G, GLG, G, sprs[21], this));

		// Tile 7

		tiles.push_back(new Tile(23, GAG, GLG, GAG, GLG, sprs[22], this));
		tiles.push_back(new Tile(24, GLG, GAG, GLG, GAG, sprs[23], this));
		tiles.push_back(new Tile(25, GAG, GLG, GAG, GLG, sprs[24], this));
		tiles.push_back(new Tile(26, GLG, GAG, GLG, GAG, sprs[25], this));

		// Tile 8

		tiles.push_back(new Tile(27, GAG, G, GLG, G, sprs[26], this));
		tiles.push_back(new Tile(28, G, GAG, G, GLG, sprs[27], this));
		tiles.push_back(new Tile(29, GLG, G, GAG, G, sprs[28], this));
		tiles.push_back(new Tile(30, G, GLG, G, GAG, sprs[29], this));

		// Tile 9

		tiles.push_back(new Tile(31, GLG, GLG, G, GLG, sprs[30], this));
		tiles.push_back(new Tile(32, GLG, GLG, GLG, G, sprs[31], this));
		tiles.push_back(new Tile(33, G, GLG, GLG, GLG, sprs[32], this));
		tiles.push_back(new Tile(34, GLG, G, GLG, GLG, sprs[33], this));

		// Tile 10

		tiles.push_back(new Tile(35, GLG, GLG, GLG, GLG, sprs[34], this));
		tiles.push_back(new Tile(36, GLG, GLG, GLG, GLG, sprs[35], this));
		tiles.push_back(new Tile(37, GLG, GLG, GLG, GLG, sprs[36], this));
		tiles.push_back(new Tile(38, GLG, GLG, GLG, GLG, sprs[37], this));

		// Tile 11

		tiles.push_back(new Tile(39, GLG, GLG, G, G, sprs[38], this));
		tiles.push_back(new Tile(40, G, GLG, GLG, G, sprs[39], this));
		tiles.push_back(new Tile(41, G, G, GLG, GLG, sprs[40], this));
		tiles.push_back(new Tile(42, GLG, G, G, GLG, sprs[41], this));

		// Tile 12

		tiles.push_back(new Tile(43, G, GLG, G, GLG, sprs[42], this));
		tiles.push_back(new Tile(44, GLG, G, GLG, G, sprs[43], this));
		tiles.push_back(new Tile(45, G, GLG, G, GLG, sprs[44], this));
		tiles.push_back(new Tile(46, GLG, G, GLG, G, sprs[45], this));


		sprs.push_back(new olc::Sprite("circuit\\13.png"));
		tiles.push_back(new Tile(46, BBB, BBB, BBB, BBB,sprs[46], this));

		int x0 = 5;
		int y0 = 4;
		// Create the grid
		gridSize = WIDTH / tileSize;

		for (int x = 0;x < gridSize;x++) {
			for (int y = 0;y < gridSize;y++) {

				if (x == x0 && y == y0) {
					int id = 2;
					cellGrid.push_back(new Cell(x,y,x * tileSize, y * tileSize, tileSize, tileSize, tiles[id], id, tiles.size(), this));
				}
				else 
					cellGrid.push_back(new Cell(x,y,x* tileSize,y* tileSize,tileSize,tileSize,tiles.size(),this));

			}
		}



		// Update Entropy of Neibhors of Original Tile


		updateNeibsEntropy(x0, y0);

		std::vector<Cell*> cellGridCopy(cellGrid);

		std::sort(cellGridCopy.begin(), cellGridCopy.end(), cmpFun);


	//	std::cout << "Options of 1st cell : " << cellGridCopy[1]->m_options.size() << std::endl;
		//for (int i = 0; i < cellGridCopy[1]->m_options.size();i++)
		//	std::cout << cellGridCopy[1]->m_options[i] << std::endl;
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		//Clear(olc::BLACK);
	

		// Draw the grid


		std::vector<Cell*> cellGridCopy(cellGrid);

		std::sort(cellGridCopy.begin(), cellGridCopy.end(), cmpFun);
		int midx = 0;
		for (int i = 0;i < cellGridCopy.size()-1;i++) {
			if (cellGridCopy[i + 1]->entropy() > cellGridCopy[i]->entropy()) {
				midx = i;
				break;
			}
		}
		int cellIdx = 0;
		if (midx > 0)
			cellIdx = rand() % midx;
		if (cellGridCopy[cellIdx]->entropy() <= tiles.size()) {
			cellGridCopy[cellIdx]->pickRandOption(tiles);
			updateNeibsEntropy(cellGridCopy[cellIdx]->m_x, cellGridCopy[cellIdx]->m_y);
		}
		for (int i = 0;i < cellGrid.size();i++) {
			cellGrid[i]->drawCell(olc::WHITE);
		}

		return true;
	}
};


int main()
{
	Generator demo;
	if (demo.Construct(WIDTH, HEIGHT, 1, 1))
		demo.Start();

	return 0;
}