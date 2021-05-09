#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAP_HEIGHT 69
#define MAP_WIDTH 212

typedef struct tile
{
	int alive;
	int neighbors;
}tile;

void init_map();
void update();
void draw_map();

void build_glider(int x, int y);
void build_blinker(int x, int y);
void build_toad(int x, int y);
void build_beacon(int x, int y);
void build_random();

tile mapOne[MAP_HEIGHT][MAP_WIDTH];
tile mapTwo[MAP_HEIGHT][MAP_WIDTH];
tile (*map)[MAP_WIDTH] = (tile (*)[MAP_WIDTH])&mapOne[0];
tile (*newMap)[MAP_WIDTH] = (tile (*)[MAP_WIDTH])&mapTwo[0];

clock_t deltaTime = CLOCKS_PER_SEC;
double secPerFrame = 0;
double fps = 0;

int main()
{
	srand(clock());

	init_map();
	build_random();

	int s = 0;
	unsigned int f = 0;

	while (1)
	{
		clock_t begin = clock();

		update();
		draw_map();

		clock_t end = clock();

		deltaTime = end - begin;
		secPerFrame = (double)deltaTime / (double)CLOCKS_PER_SEC;
		fps = (double)1 / secPerFrame;
	}

	return 0;
}

void init_map()
{
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			map[i][j].alive = 0;
			map[i][j].neighbors = 0;
		}
	}
}

void update()
{
	// update alive state
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			newMap[i][j].alive = map[i][j].alive;
			newMap[i][j].neighbors = 0;

			// check all neighbors
			for (int m = 0; m < 3; m++)
			{
				for (int n = 0; n < 3; n++)
				{
					int x = (j - 1) + n;
					int y = (i - 1) + m;
					if (y >= 0 && y < MAP_HEIGHT && x >= 0 && x < MAP_WIDTH)
						if (y != i || x != j)
							newMap[i][j].neighbors += map[y][x].alive;
				}
			}

			if (newMap[i][j].neighbors <= 1)	// 1 or less dies
				newMap[i][j].alive = 0;
			if (newMap[i][j].neighbors >= 4)	// 4 or more dies
				newMap[i][j].alive = 0;
			if (newMap[i][j].neighbors == 3)	// 3 becomes alive
				newMap[i][j].alive = 1;
			// anything else then it should stay the same
		}
	}

	// swap new and old map
	tile (*temp)[MAP_WIDTH] = map;
	map = newMap;
	newMap = temp;
}

void draw_map()
{
	printf("%lf", fps);
	printf("------------------------------------------------------------------------\n");
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			if (map[i][j].alive)
				printf("%d", map[i][j].neighbors);
			else
				printf(" ", map[i][j].neighbors);

			if (j >= MAP_WIDTH - 1)
				printf("\n");
		}
	}
}

void build_glider(int x, int y)
{
	map[0 + y][1 + x].alive = 1;
	map[1 + y][2 + x].alive = 1;
	map[2 + y][0 + x].alive = 1;
	map[2 + y][1 + x].alive = 1;
	map[2 + y][2 + x].alive = 1;
}

void build_blinker(int x, int y)
{
	map[1 + y][1 + x].alive = 1;
	map[1 + y][2 + x].alive = 1;
	map[1 + y][0 + x].alive = 1;
}

void build_random()
{
	for (int i = 0; i < MAP_HEIGHT; i++)
		for (int j = 0; j < MAP_WIDTH; j++)
			map[i][j].alive = rand() % 2;
}
