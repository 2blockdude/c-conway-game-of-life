#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAP_HEIGHT 69
#define MAP_WIDTH 212

typedef struct tile
{
	int alive;
	int neighbors;
}tile;

void init_map(tile *m);
void update(tile **m);
void draw_map(tile *m);

void build_glider(tile *m, int x, int y);
void build_blinker(tile *m, int x, int y);
void build_toad(tile *m, int x, int y);
void build_beacon(tile *m, int x, int y);
void build_random(tile *m);

double deltaTime = 0;
double fps = 0;

int main()
{
	tile *map = malloc(sizeof(tile) * MAP_HEIGHT * MAP_WIDTH);

	srand(clock());
	init_map(map);
	build_random(map);

	while (1)
	{
		clock_t begin = clock();

		update(&map);
		draw_map(map);

		clock_t end = clock();

		deltaTime = (double)(end - begin) / CLOCKS_PER_SEC;
		fps = 1.0f / deltaTime;
	}

	free(map);

	return 0;
}

void init_map(tile *m)
{
	tile (*map)[MAP_WIDTH] = (tile (*)[MAP_WIDTH])&m[0];

	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			map[i][j].alive = 0;
			map[i][j].neighbors = 0;
		}
	}
}

void update(tile **m)
{
	tile *n = malloc(sizeof(tile) * MAP_HEIGHT * MAP_WIDTH);
	
	tile (*newMap)[MAP_WIDTH] = (tile (*)[MAP_WIDTH])&n[0];
	tile (*oldMap)[MAP_WIDTH] = (tile (*)[MAP_WIDTH])&(*m)[0];

	// update
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			newMap[i][j].alive = oldMap[i][j].alive;
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
							newMap[i][j].neighbors += oldMap[y][x].alive;
				}
			}

			// update state
			if (newMap[i][j].neighbors <= 1)	// 1 or less dies
				newMap[i][j].alive = 0;
			if (newMap[i][j].neighbors >= 4)	// 4 or more dies
				newMap[i][j].alive = 0;
			if (newMap[i][j].neighbors == 3)	// 3 becomes alive
				newMap[i][j].alive = 1;
			// anything else then it should stay the same
		}
	}

	free(*m);
	*m = n;
}

void draw_map(tile *m)
{
	tile (*map)[MAP_WIDTH] = (tile (*)[MAP_WIDTH])&m[0];

	printf("%lf", fps);
	printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
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

void build_glider(tile *m, int x, int y)
{
	tile (*map)[MAP_WIDTH] = (tile (*)[MAP_WIDTH])&m[0];
	map[0 + y][1 + x].alive = 1;
	map[1 + y][2 + x].alive = 1;
	map[2 + y][0 + x].alive = 1;
	map[2 + y][1 + x].alive = 1;
	map[2 + y][2 + x].alive = 1;
}

void build_blinker(tile *m, int x, int y)
{
	tile (*map)[MAP_WIDTH] = (tile (*)[MAP_WIDTH])&m[0];
	map[1 + y][1 + x].alive = 1;
	map[1 + y][2 + x].alive = 1;
	map[1 + y][0 + x].alive = 1;
}

void build_random(tile *m)
{
	tile (*map)[MAP_WIDTH] = (tile (*)[MAP_WIDTH])&m[0];
	for (int i = 0; i < MAP_HEIGHT; i++)
		for (int j = 0; j < MAP_WIDTH; j++)
			map[i][j].alive = rand() % 2;
}
