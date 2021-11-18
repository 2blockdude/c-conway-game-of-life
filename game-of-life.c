#include <stdio.h>
#include <stdlib.h>

#define MAX_HEIGHT 50
#define MAX_WIDTH 50

void init_map();
void update();
void draw_map();

void build_shape(char *shape, int x, int y);
void build_glider(int x, int y);
void build_blinker(int x, int y);
void build_toad(int x, int y);
void build_beacon(int x, int y);
void build_ant(int x, int y);
void build_growth(int x, int y);
void build_random();

char *cellOutput;
char *cellBuffer;

int main(int argc, char *argv[])
{
	srand(1000);

	init_map();
	build_random();

	while (1)
	{
		update();
		draw_map();
	}

	free(cellOutput);
	free(cellBuffer);

	return 0;
}

void init_map()
{
	cellOutput = calloc(MAX_HEIGHT * MAX_WIDTH, sizeof(char));
	cellBuffer = calloc(MAX_HEIGHT * MAX_WIDTH, sizeof(char));
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
			cellBuffer[above * MAX_WIDTH + left] +
			cellBuffer[above * MAX_WIDTH + x] +
			cellBuffer[above * MAX_WIDTH + right] +
			// get middle
			cellBuffer[y * MAX_WIDTH + left] +
			cellBuffer[y * MAX_WIDTH + right] +
			// get below
			cellBuffer[below * MAX_WIDTH + left] +
			cellBuffer[below * MAX_WIDTH + x] +
			cellBuffer[below * MAX_WIDTH + right];

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

void build_growth(int x, int y)
{
	char shape[] =
		"######## #####   ###      ####### #####";

	build_shape(shape, x, y);
}

void build_random()
{
	for (int i = 0; i < MAX_HEIGHT * MAX_WIDTH; i++)
		cellBuffer[i] = rand() % 2;
}
