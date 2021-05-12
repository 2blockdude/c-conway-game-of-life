#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#define MAX_HEIGHT 10
#define MAX_WIDTH 10

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

unsigned int frame = 0;	// number of frames
double delta = 0;			// time between frames in milliseconds
double fps = 0;			// frames per second
clock_t cycles = 0;		// cycles per frame
long int delay = 0;		// delay per frame
int maxFps = 1000;		// max fps

double tdelta = 0;		// true delta
double tfps = 0;			// true fps

int main(int argc, char *argv[])
{
	if (argc == 2)
		maxFps = atoi(argv[1]);
	srand(clock());

	init_map();
	build_random();

	while (1)
	{
		struct timespec start;
		struct timespec stop;

		clock_gettime(CLOCK_MONOTONIC, &start);

		clock_t begin = clock();

		update();
		draw_map();

		clock_t end = clock();

		clock_gettime(CLOCK_MONOTONIC, &stop);

		cycles = end - begin;
		delta = (double)cycles / CLOCKS_PER_SEC * 1000;
		fps = 1000.0f / delta;

		tdelta = (double)(stop.tv_nsec - start.tv_nsec) / 1000000;
		tfps = 1000.0f / tdelta;

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
	printf("fps:\t%4.6f\n", fps);
	printf("tfps:\t%4.6f\n", tfps);
	printf("delta:\t%4.6lfms\n", delta);
	printf("tdelta:\t%4.6lfms\n", tdelta);
	printf("cpf:\t%0.6d\n", cycles);
	printf("delay:\t%0.6lums\n", delay);
	printf("frame:\t%0.6d\n", frame);
	printf("------------\n");
	//printf("fps: %4.6lf | cpf: %0.6d | delta: %4.6lfms | delay: %0.6lums | frame: %0.6d | tfps: %4.6f\n", fps, cycles, delta, delay, frame, tfps);
	//printf("-------------------------------------------------------------------------------------------------\n");

	for (int i = 0; i < MAX_HEIGHT * MAX_WIDTH; i++)
	{
		if (cellOutput[i])
			printf("#");
		else
			printf(" ");

		if ((i % MAX_WIDTH) == (MAX_WIDTH - 1))
			printf("\n");
	}

	printf("------------\n");
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
