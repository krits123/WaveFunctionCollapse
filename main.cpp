#define OLC_PGE_APPLICATION
#include "Resources\olcPixelGameEngine.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include "Cell.h"
#include "Tile.h"
#include <cstdlib>
#include <time.h>

#define WIDTH  1064
#define HEIGHT  1064

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
// - Implement Entropy Propagation for Each Iteration || DONE
// - Define Width and height and tile size better
// - Tile and Cell Classes have redudant and useless info, make them minimal and better
// - Maybe don t draw everything from the start on each loop || DONE
// - Draw something if it was never drawn before || DONE
// - Implement roattion of encoding 
// - Don t roatet everything some tiles have 90 or 180 rotatinal symmetry || DONE
// - Implement rap around or Define stoping blocks for the edges
// - Better use of setters and getters for classes not every member shoyld be public
// - FPS is too low should reach at least 15 - 20 (or >30) || DONE 
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
	std::vector<std::vector<int>> LUT;

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

	
	// This Function Updates the entropies of the entire Tile Map Starting From the Tile at (x,y) which just collpased and propagates the information outwards
	void updateEntropies(int x, int y) {


		int maxItr = std::max({ x,y,gridSize - x - 1,gridSize - y - 1 });



		int idx = 1;

		while (idx <= maxItr) {



			int x1;
			int y1;
			// RIGHT MAX DIST
			x1 = x + idx;
			y1 = y;
			if (checkInBounds(x1, y1)) {

				updateFromNeibhor(x1, y1, x1 -1, y1,LEFT);
			}


			// DOWN MAX DIST
			x1 = x;
			y1 = y + idx;
			if (checkInBounds(x1, y1)) {

				updateFromNeibhor(x1, y1, x1, y1 - 1,UP);
			}

			// LEFT MAX DIST
			x1 = x - idx;
			y1 = y;
			if (checkInBounds(x1, y1)) {

				updateFromNeibhor(x1, y1, x1 + 1, y1,RIGHT);
			}

			// TOP MAX DIST
			x1 = x;
			y1 = y - idx;
			if (checkInBounds(x1, y1)) {

				updateFromNeibhor(x1, y1, x1, y1 + 1,DOWN);
			}


			for (int i = 1;i < idx;i++) {


				// RIGHT + UP
				x1 = x + i;
				y1 = y - idx + i;
				if (checkInBounds(x1,y1)) {

					updateFromNeibhor(x1, y1, x1 - 1, y1,LEFT);

					updateFromNeibhor(x1, y1, x1 , y1 + 1,DOWN);

				}

				// RIGHT + DOWN
				x1 = x + i;
				y1 = y + idx - i;
				if (checkInBounds(x1, y1)) {

					updateFromNeibhor(x1, y1, x1 - 1, y1,LEFT);

					updateFromNeibhor(x1, y1, x1, y1 - 1,UP);

				}
				// LEFT + DOWN
				x1 = x - i;
				y1 = y + idx - i;
				if (checkInBounds(x1, y1)) {

					updateFromNeibhor(x1, y1, x1 + 1, y1,RIGHT);

					updateFromNeibhor(x1, y1, x1, y1 -1,UP);

				}
				// LEFT + UP
				x1 = x - i;
				y1 = y - idx + i;
				if (checkInBounds(x1, y1)) {

					updateFromNeibhor(x1, y1, x1 + 1, y1,RIGHT);

					updateFromNeibhor(x1, y1, x1, y1 + 1,DOWN);

				}




			}


			idx++;
		}


	}

	// This Function Updates the valiable options of the cell at (x,y) based on the options of the cell at (x1,y1).
	// The 2 cells are neibhors 
	// The dir variable stores on which side of the current cell (x,y) is the cell (x1,y1) . It is redudant but it helps avoiding some checks which makes the code faster and clean.
	void updateFromNeibhor(int x, int y, int x1, int y1,int dir) {


		if (cellGrid[indexOfCell(x, y)]->entropy() > tiles.size())
			return;

		int adjDir = (dir + 2) % 4;
		int idx = indexOfCell(x1, y1);
		Cell* cellAdj = cellGrid[idx];

		std::vector<int> toRemove;
		std::vector<int> targetOptions = cellGrid[indexOfCell(x, y)]->m_options;


		for (int i = 0;i < targetOptions.size();i++) {
			Tile* currentTile = tiles[targetOptions[i]];
			int target = currentTile->m_sides[dir];
			bool removeOpt = true;
			for (int j = 0;j < cellAdj->m_options.size();j++) {

				if (tiles[cellAdj->m_options[j]]->m_sides[adjDir] == target) {
					removeOpt = false;
					break;
				}
					
			}
			if (removeOpt)
				toRemove.push_back(targetOptions[i]);

		}

		for (int i = 0;i < toRemove.size();i++) {
			cellGrid[indexOfCell(x, y)]->removeOption(toRemove[i]);
		}

		

		return;
	}

	bool checkInBounds(int x, int y) {
		return   (y < gridSize) && (y >= 0) && (x < gridSize) && (x >= 0);
	}

	

	


	// For each available tile generate a LUT that maps [tile,side] => [matching tiles] 
	void generateTileLUT() {

		int cols = 4;
		int rows = tiles.size();

		for (int i = 0;i < rows;i++) {
			for (int j = 0;j < cols;j++) {

				std::vector<int> matches;

				int target = tiles[i]->m_sides[j];
				int dir = (j + 2) % 4;  // This variable containts the matching direction. 

				for (int tid = 0; tid < tiles.size();tid++) {
					if (target == tiles[tid]->m_sides[dir])
						matches.push_back(tid);
				}

				LUT.push_back(matches);

			}
		}


	}

	void printLUT() {


		std::string dirs[4] = { "UP","RIGHT","DOWN","LEFT" };

		for (int i = 0; i < tiles.size();i++) {
			for (int j = 0; j < 4;j++) {
				std::vector<int> matches(getFromLUT(j, i));
				std::cout << "Tile from id :" << i << " and dir : " << dirs[j] << " matches with tiles : ";
				for (int k = 0;k < matches.size();k++) {
					std::cout << matches[k] << " ";
				}
				std::cout << std::endl;
			}
			std::cout << "------------------------------------------------------------" << std::endl;
		}



	}

	std::vector<int> getFromLUT(int dir, int tileID) {


		return LUT[4 * tileID + dir];

	}

public:






	bool OnUserCreate() override
	{

		/*std::vector<olc::vi2d> ps;
		getManhatanDistancesConsts(1, ps);
		
		std::cout << "Pairs with dist 1 :  " << std::endl;
		for (int i = 0; i < ps.size();i++)
			std::cout << ps[i].x << " ," << ps[i].y << std::endl;
		ps.clear();
		getManhatanDistancesConsts(2, ps);
		std::cout << "Pairs with dist 1 :  " << std::endl;
		for (int i = 0; i < ps.size();i++)
			std::cout << ps[i].x << " ," << ps[i].y << std::endl;
		ps.clear();
		getManhatanDistancesConsts(3, ps);
		std::cout << "Pairs with dist 1 :  " << std::endl;
		for (int i = 0; i < ps.size();i++)
			std::cout << ps[i].x << " ," << ps[i].y << std::endl;
		ps.clear();
		getManhatanDistancesConsts(4, ps);
		std::cout << "Pairs with dist 1 :  " << std::endl;
		for (int i = 0; i < ps.size();i++)
			std::cout << ps[i].x << " ," << ps[i].y << std::endl;
		*/

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
		/*
		std::string G = "GGG";
		std::string BBB= "BBB";
		std::string BGG = "BGG";
		std::string GGB = "GGB";
		std::string GLG = "GLG";
		std::string GAG = "GAG";
		*/
		int G   = 0;
		int BBB = 1;
		int BGG = 2;
		int GGB = 3;
		int GLG = 4;
		int GAG = 5;


		// 2 Blanks 
		tiles.push_back(new Tile(0, BBB, BBB, BBB, BBB, sprs[0], this));
		tiles.push_back(new Tile(1, G, G, G, G, sprs[1], this));



		// Tile 2 
		
		
		tiles.push_back(new Tile(2, G, GLG, G, G, sprs[2], this));
		tiles.push_back(new Tile(3, G, G, GLG, G, sprs[3], this));
		tiles.push_back(new Tile(4, G, G, G, GLG, sprs[4], this));
		tiles.push_back(new Tile(5, GLG, G, G, G, sprs[5], this));


		// Tile 3 
		

		tiles.push_back(new Tile(6, G, GAG, G, GAG, sprs[6], this));
		tiles.push_back(new Tile(7, GAG, G, GAG, G, sprs[7], this));




		// Tile 4
		

		tiles.push_back(new Tile(8, BGG, GLG, BGG,BBB, sprs[10], this));
		tiles.push_back(new Tile(9, BBB, BGG, GLG, BGG, sprs[11], this));
		tiles.push_back(new Tile(10, GGB, BBB, GGB, GLG, sprs[12], this));
		tiles.push_back(new Tile(11, GLG, GGB, BBB, GGB, sprs[13], this));

		// Tile 5
		
		tiles.push_back(new Tile(12, BGG, G, G, BGG, sprs[14], this));
		tiles.push_back(new Tile(13, GGB, BGG, G,G, sprs[15], this));
		tiles.push_back(new Tile(14, G, GGB, GGB, G, sprs[16], this));
		tiles.push_back(new Tile(15, G, G, BGG, GGB, sprs[17], this));

		// Tile 6

		tiles.push_back(new Tile(16, G, GLG, G, GLG, sprs[18], this));
		tiles.push_back(new Tile(17, GLG, G, GLG, G, sprs[19], this));

		// Tile 7

		tiles.push_back(new Tile(18, GAG, GLG, GAG, GLG, sprs[22], this));
		tiles.push_back(new Tile(19, GLG, GAG, GLG, GAG, sprs[23], this));
		

		// Tile 8

		tiles.push_back(new Tile(20, GAG, G, GLG, G, sprs[26], this));
		tiles.push_back(new Tile(21, G, GAG, G, GLG, sprs[27], this));
		tiles.push_back(new Tile(22, GLG, G, GAG, G, sprs[28], this));
		tiles.push_back(new Tile(23, G, GLG, G, GAG, sprs[29], this));

		// Tile 9

		tiles.push_back(new Tile(24, GLG, GLG, G, GLG, sprs[30], this));
		tiles.push_back(new Tile(25, GLG, GLG, GLG, G, sprs[31], this));
		tiles.push_back(new Tile(26, G, GLG, GLG, GLG, sprs[32], this));
		tiles.push_back(new Tile(27, GLG, G, GLG, GLG, sprs[33], this));

		// Tile 10

		tiles.push_back(new Tile(28, GLG, GLG, GLG, GLG, sprs[34], this));
		tiles.push_back(new Tile(29, GLG, GLG, GLG, GLG, sprs[35], this));
		

		// Tile 11

		tiles.push_back(new Tile(30, GLG, GLG, G, G, sprs[38], this));
		tiles.push_back(new Tile(31, G, GLG, GLG, G, sprs[39], this));
		tiles.push_back(new Tile(32, G, G, GLG, GLG, sprs[40], this));
		tiles.push_back(new Tile(33, GLG, G, G, GLG, sprs[41], this));

		// Tile 12

		tiles.push_back(new Tile(34, G, GLG, G, GLG, sprs[42], this));
		tiles.push_back(new Tile(35, GLG, G, GLG, G, sprs[43], this));
		


		//sprs.push_back(new olc::Sprite("circuit\\13.png"));
		//tiles.push_back(new Tile(46, -1, -1, -1, -1,sprs[46], this));

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


		//updateNeibsEntropy(x0, y0);
		updateEntropies(x0, y0);
		std::vector<Cell*> cellGridCopy(cellGrid);

		std::sort(cellGridCopy.begin(), cellGridCopy.end(), cmpFun);



		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		//Clear(olc::BLACK);
	

		// Draw the grid

		if (GetKey(olc::Key::W).bReleased) {
			std::vector<Cell*> cellGridCopy(cellGrid);

			std::sort(cellGridCopy.begin(), cellGridCopy.end(), cmpFun);
			int midx = 0;
			for (int i = 0;i < cellGridCopy.size() - 1;i++) {
				if (cellGridCopy[i + 1]->entropy() > cellGridCopy[i]->entropy()) {
					midx = i;
					break;
				}
			}
			int cellIdx = 0;
			if (midx > 0)
				cellIdx = rand() % midx;
			if (cellGridCopy[cellIdx]->entropy() <= tiles.size()) {
				//std::cout << "Got here \n";
				cellGridCopy[cellIdx]->pickRandOption(tiles);
				updateEntropies(cellGridCopy[cellIdx]->m_x, cellGridCopy[cellIdx]->m_y);
			}
			for (int i = 0;i < cellGrid.size();i++) {
				cellGrid[i]->drawCell(olc::WHITE);
			}
		}

		if (GetKey(olc::Key::D).bReleased) {
			int a = -1;
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