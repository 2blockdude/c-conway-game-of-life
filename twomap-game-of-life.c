#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_HEIGHT 69
#define MAX_WIDTH 212

void init_map();
void update();
void draw_map();

void build_glider(int x, int y);
void build_blinker(int x, int y);
void build_toad(int x, int y);
void build_beacon(int x, int y);
void build_shape(char *shape, int x, int y);
void build_random();

char *cellOutput;
char *cellBuffer;

double deltaTime = CLOCKS_PER_SEC;
double fps = 0;

int main()
{
	srand(clock());

	init_map();

	char shape[] = 
		" # \n" \
		"  #\n" \
		"###\n";

	build_shape(shape, 0, 0);
	//build_random();

	while (1)
	{
		clock_t begin = clock();

		update();
		draw_map();

		for (int i = 0; i < 49999999; i++);

		clock_t end = clock();

		deltaTime = (double)(end - begin) / CLOCKS_PER_SEC;
		fps = 1.0f / deltaTime;
	}

	return 0;
}

void init_map()
{
	cellOutput = malloc(sizeof(char) * MAX_HEIGHT * MAX_WIDTH);
	cellBuffer = malloc(sizeof(char) * MAX_HEIGHT * MAX_WIDTH);

	for (int i = 0; i < MAX_HEIGHT * MAX_WIDTH; i++)
		cellBuffer[i] = cellOutput[i] = 0;
}

void update()
{
	// update alive state
	for (int i = 0; i < MAX_HEIGHT * MAX_WIDTH; i++)
	{
		cellOutput[i] = cellBuffer[i];

		int x = i % MAX_WIDTH;
		int y = (i - x) / MAX_WIDTH;

		int neighbors =
			cellBuffer[(i - MAX_WIDTH - 1)]	+ cellBuffer[(i - MAX_WIDTH)]	+ cellBuffer[(i - MAX_WIDTH + 1)]	+
			cellBuffer[(i - 1)]					+ 0									+ cellBuffer[(i + 1)]					+
			cellBuffer[(i + MAX_WIDTH - 1)]	+ cellBuffer[(i + MAX_WIDTH)]	+ cellBuffer[(i + MAX_WIDTH + 1)];

		if (neighbors >= 4 || neighbors <= 1)
			cellOutput[i] = 0;
		else if (neighbors == 3)
			cellOutput[i] = 1;

	}

	char *temp = cellOutput;
	cellOutput = cellBuffer;
	cellBuffer = temp;
}

void draw_map()
{
	printf("%lf", fps);
	printf("------------------------------------------------------------------------\n");
	for (int i = 0; i < MAX_HEIGHT * MAX_WIDTH; i++)
	{
		if (cellOutput[i])
			printf("#");
		else
			printf(" ");

		if ((i % MAX_WIDTH) == (MAX_WIDTH - 1))
			printf("\n");
	}
}

void build_shape(char *shape, int x, int y)
{
	for (int i = 0; i < MAX_HEIGHT && *shape; i++, shape++)
	{
		for (int j = 0; j < MAX_WIDTH && *shape != '\n'; j++, shape++)
		{
			if (*shape == '#')
				cellBuffer[i * MAX_WIDTH + j] = 1;
			else
				cellBuffer[i * MAX_WIDTH + j] = 0;
		}
	}
}

void build_random()
{
	for (int i = 0; i < MAX_HEIGHT * MAX_WIDTH; i++)
		cellBuffer[i] = rand() % 2;
}
