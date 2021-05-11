#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_HEIGHT 68
#define MAX_WIDTH 212

void init_map();
void update();
void draw_map();

void build_shape(char *shape, int x, int y);
void build_glider(int x, int y);
void build_blinker(int x, int y);
void build_toad(int x, int y);
void build_beacon(int x, int y);
void build_ant(int x, int y);
void build_random();

char *cellOutput;
char *cellBuffer;

unsigned int frame = 0;
double deltaTime = 0;
double fps = 0;

int main()
{
	srand(clock());

	init_map();
		build_glider(0, 0);

	while (1)
	{
		clock_t begin = clock();

		update();
		draw_map();

		//if (frame % 14 == 0)

		for (int i = 0; i < 99999999; i++);

		clock_t end = clock();

		deltaTime = (double)(end - begin) / CLOCKS_PER_SEC;
		fps = 1.0f / deltaTime;
		frame++;
	}

	free(cellOutput);
	free(cellBuffer);

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

		// gets columns and rows
		int x = i % MAX_WIDTH;			// column
		int y = (i - x) / MAX_WIDTH;	// row

		// wrap around left right
		int left = (x - 1) < 0 ? MAX_WIDTH - 1 : x - 1;
		int right = (x + 1) % MAX_WIDTH;

		// wrap around above below
		int above = (y - 1) < 0 ? MAX_HEIGHT - 1 : y - 1;
		int below = (y + 1) % MAX_HEIGHT;

		int neighbors =
			// get above
			cellBuffer[above * MAX_WIDTH + left]	+ cellBuffer[above * MAX_WIDTH + x]	+ cellBuffer[above * MAX_WIDTH + right]	+
			// get middle
			cellBuffer[y * MAX_WIDTH + left]			+ 0											+ cellBuffer[y * MAX_WIDTH + right]			+
			// get below
			cellBuffer[below * MAX_WIDTH + left]	+ cellBuffer[below * MAX_WIDTH + x]	+ cellBuffer[below * MAX_WIDTH + right];

		// cell rules
		if (neighbors >= 4 || neighbors <= 1)
			cellOutput[i] = 0;
		else if (neighbors == 3)
			cellOutput[i] = 1;

	}

	// swap output and buffer
	char *temp = cellOutput;
	cellOutput = cellBuffer;
	cellBuffer = temp;
}

void draw_map()
{
	printf("%lf\t|%lf\t|%d\n", fps, deltaTime, frame);
	printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

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
				cellBuffer[(i + y) * MAX_WIDTH + (j + x)] = 1;
			else
				cellBuffer[(i + y) * MAX_WIDTH + (j + x)] = 0;
		}
	}
}

void build_glider(int x, int y)
{
	char shape[] = 
		" # \n" \
		"  #\n" \
		"###\n";

	build_shape(shape, x, y);
}

void build_blinker(int x, int y)
{
	char shape[] =
		" # \n" \
		" # \n" \
		" # ";

	build_shape(shape, x, y);
}

void build_toad(int x, int y)
{
	char shape[] =
		" ###\n" \
		"### ";

	build_shape(shape, x, y);
}

void build_beacon(int x, int y)
{
	char shape[] =
		"  ##\n" \
		"  ##\n" \
		"##  \n" \
		"##  ";

	build_shape(shape, x, y);
}

void build_ant(int x, int y)
{
	char shape[] =
		"##  \n" \
		"  ##\n" \
		"  ##\n" \
		"##  ";

	build_shape(shape, x, y);
}

void build_random()
{
	for (int i = 0; i < MAX_HEIGHT * MAX_WIDTH; i++)
		cellBuffer[i] = rand() % 2;
}
