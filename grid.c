#include <jaglib.h>
#include <string.h>
#include <jagcore.h>
#include <stdlib.h>
#include <stdio.h>
#include "grid.h"
#include "globals.h"
#include "imgobjects.h"

#define size 4
#define TARGET 2048

static uint8_t *block[] = { block_nul, block_nul, block2, block4, block8, block16, block32, block64, block128, block256, block512,
block1024, block2048, block4096, block8192, block16384, block32768, block65536, block131072 };
static uint16_t log2(uint32_t value)
{
	uint16_t result = 0;
	if (value < 2)
		return 0;
	do
	{
		value >>= 1;
		++result;
	} while (!(value & 1));
	return result;
}

static struct Tile cells[size][size];
static struct Tile *cellList[size*size];
static op_bmp_object bmpList[size*size];
static op_bmp_object backbuffer[size*size];
static volatile uint16_t refreshFromBackbuffer;
static uint32_t baseOlAdr;
static uint32_t score;
static uint32_t best;
static uint16_t over;
static uint16_t keepPlaying;
static uint16_t won;

struct Tile
{
	uint32_t value;
	op_bmp_object *tile;
	uint16_t dx;
	uint16_t dy;
	uint16_t orgx;
	uint16_t orgy;
};

void GridKeepPlaying()
{
	keepPlaying = TRUE;
}

enum Game GridGameState()
{
	if (won && !keepPlaying)
	{
		return Won;
	}
	if (over)
	{
		return Over; // Game over
	}
	return Go;
}

static void GridCreateObjectList(uint16_t useReverseOrder)
{
	struct Tile *current;
	while (refreshFromBackbuffer); // Wait for isr ready!
	for (uint16_t index = 0; index < size*size; ++index)
	{
		current = useReverseOrder ? cellList[size*size - 1 - index] : cellList[index];
		current->tile->p0.link = baseOlAdr + 2 * index;
		memcpy(&backbuffer[index], current->tile, sizeof(op_bmp_object));
	}
	refreshFromBackbuffer = TRUE;
}

static uint16_t GridCellsAvailable()
{
	for (uint16_t index = 0; index < size*size; ++index)
		if (cellList[index]->value == 0)
			return TRUE;
	return FALSE;
}

static uint16_t GridTileMatchesAvailable()
{
	for (uint8_t x = 0; x < size; ++x)
	{
		for (uint8_t y = 0; y < size; ++y)
		{
			if (x + 1 < size && cells[x][y].value == cells[x + 1][y].value ||
				y + 1 < size && cells[x][y].value == cells[x][y + 1].value)
				return TRUE;
		}
	}
	return FALSE;
}

static uint16_t GridMovesAvailable()
{
	return GridCellsAvailable() || GridTileMatchesAvailable();
}

static uint8_t scale[19] = {
	2, 3, 5, 6, 8, 10, 11, 13, 14, 16, 18, 19, 21, 22, 24, 26, 27, 29, 30
};

static void GridAddRandomTile()
{
	uint16_t value = rand() % 10 == 0 ? 4 : 2;
	uint16_t pos;
	do
	{
		pos = rand() & 0x0f;
	} while (cellList[pos]->value != 0);
	cellList[pos]->value = value;
	scaledBmp->p2.hscale = 0;
	scaledBmp->p2.vscale = 0;
	scaledBmp->p0.data = (uint32_t)(value == 2 ? block2 : block4) >> 3;
	scaledBmp->p1.xpos = cellList[pos]->tile->p1.xpos + 20;
	scaledBmp->p0.ypos = cellList[pos]->tile->p0.ypos + 20;
	scaledBmp->p2.hscale = 0;
	scaledBmp->p2.vscale = 0;
	for (uint16_t inner = 0; inner < 19; ++inner)
	{
		scaledBmp->p2.hscale = scale[inner];
		scaledBmp->p2.vscale = scale[inner];
		if (inner%2)
			jag_wait_vbl();
		--scaledBmp->p1.xpos;
		--scaledBmp->p0.ypos;
	}
	scaledBmp->p1.xpos = 400;
	cellList[pos]->tile->p0.data = scaledBmp->p0.data;
	GridCreateObjectList(TRUE);
}

static void GridUpdateScore(uint32_t value)
{
	uint16_t decsize;
	char scorestr[32];
	score += value;
	sprintf(scorestr,"%u",score);
	decsize = strlen(scorestr)*4;
	jag_console_set_cursor(28 - decsize, 30);
	puts(scorestr);
	if (score > best)
	{
		best = score;
	}
	sprintf(scorestr,"%u",best);
	decsize = strlen(scorestr)*4;
	jag_console_set_cursor(28 - decsize, 78);
	puts(scorestr);
}

void GridCreate(op_bmp_object *root, uint32_t base)
{
	uint8_t x, y;
	struct Tile *current;
	baseOlAdr = base;
	for (y = 0; y < size; ++y)
	{
		for (x = 0; x < size; ++x)
		{
			current = &cells[x][y];
			cellList[x + y * 4] = current;
			op_bmp_object *b = current->tile = &bmpList[x + y * 4];
			b->p0.type = BITOBJ;
			current->orgy = b->p0.ypos = 2 * (8 + y * 48) + root->p0.ypos;
			b->p0.height = 40;
			b->p0.data = (uint32_t)block_nul >> 3;
			current->value = 0;
			current->orgx = b->p1.xpos = x * 48 + 68 + root->p1.xpos;
			b->p1.depth = O_DEPTH8 >> 12;
			b->p1.pitch = 1;
			b->p1.dwidth = 40 / 8;
			b->p1.iwidth = 40 / 8;
			b->p1.trans = 1;
		}
	}

	// Build initial display list
	GridCreateObjectList(TRUE);
}

void GridGetObjectList(uint64_t *dest)
{
	if (refreshFromBackbuffer)
	{
		memcpy(dest, backbuffer, size*size * sizeof(op_bmp_object));
		refreshFromBackbuffer = FALSE; // release lock
	}
}

static struct Tile* filterTop(int8_t x, int8_t y)
{
	return &cells[x][y];
}

static struct Tile* filterBottom(int8_t x, int8_t y)
{
	return &cells[x][size - 1 - y];
}

static struct Tile* filterLeft(int8_t x, int8_t y)
{
	return &cells[y][x];
}

static struct Tile* filterRight(int8_t x, int8_t y)
{
	return &cells[size - 1 - y][x];
}

static uint16_t GridAnimateTiles(uint8_t useReverseOrder)
{
	for (uint16_t inner = 0; inner < 16; ++inner)
	{
		struct Tile *current;
		for (uint16_t index = 0; index < size*size; ++index)
		{
			current = cellList[index];

			if (inner == 15)
			{
				current->dx = 0;
				current->dy = 0;
				current->tile->p0.data = (uint32_t)block[1 + log2(current->value)] >> 3;
				current->tile->p1.xpos = current->orgx;
				current->tile->p0.ypos = current->orgy;
			}
			else
			{
				if (current->dx || current->dy)
				{
					current->tile->p1.xpos += 3 * current->dx;
					current->tile->p0.ypos += 6 * current->dy;
				}
			}
		}
		GridCreateObjectList(useReverseOrder);
	}
	GridAddRandomTile();

	return GridMovesAvailable();
}

void GridMoveTiles(enum Direction direction)
{
	if (over || (won && !keepPlaying)) // Game over?
		return;
	int8_t x, y, inner, dx, dy;
	uint16_t tilesfound = FALSE, useReverseOrder;
	struct Tile* (*func)(int8_t x, int8_t y);
	switch (direction)
	{
	case fromTop:
		dx = 0;
		dy = +1;
		func = filterTop;
		useReverseOrder = TRUE;
		break;
	case fromBottom:
		dx = 0;
		dy = -1;
		func = filterBottom;
		useReverseOrder = FALSE;
		break;
	case fromLeft:
		dx = +1;
		dy = 0;
		func = filterLeft;
		useReverseOrder = TRUE;
		break;
	case fromRight:
		dx = -1;
		dy = 0;
		func = filterRight;
		useReverseOrder = FALSE;
		break;
	}
	for (x = 0; x < size; ++x)
	{
		for (y = size - 1; y >= 0; --y)
		{
			inner = y;

			// find first tile
			do {
				--inner;
			} while (inner >= 0 && func(x, inner)->value == 0);

			if (inner >= 0) // found?
			{
				if (func(x, y)->value == 0)
				{
					func(x, inner)->dy = dy*(y - inner);
					func(x, inner)->dx = dx*(y - inner);
					func(x, y)->value = func(x, inner)->value;
					func(x, inner)->value = 0;

					// find next tile
					do {
						--inner;
					} while (inner >= 0 && func(x, inner)->value == 0);

					if (inner >= 0 && func(x, y)->value == func(x, inner)->value) // found and same value?
					{
						func(x, inner)->dy = dy*(y - inner);
						func(x, inner)->dx = dx*(y - inner);
						if ((func(x, y)->value <<= 1) == TARGET)
						{
							won = TRUE;
						}
						func(x, inner)->value = 0;
						GridUpdateScore(func(x, y)->value);
					}
					tilesfound = TRUE;
				}
				else
				{
					if (func(x, y)->value == func(x, inner)->value)
					{
						func(x, inner)->dy = dy*(y - inner);
						func(x, inner)->dx = dx*(y - inner);
						if ((func(x, y)->value <<= 1) == TARGET)
						{
							won = TRUE;
						}
						func(x, inner)->value = 0;
						GridUpdateScore(func(x, y)->value);
						tilesfound = TRUE;
					}
				}
			}
		}
	}
	if (tilesfound)
	{
		if (!GridAnimateTiles(useReverseOrder))
		{
			over = TRUE;
		}
	}
}

void GridNewGame()
{
	score = 0;
	over = FALSE;
	won = FALSE;
	keepPlaying = FALSE;
	jag_console_set_cursor(0, 30);
	puts("       ");
	struct Tile *current;
	for (uint16_t index = 0; index < size*size; ++index)
	{
		current = cellList[index];
		current->value = 0;
		current->tile->p0.data = (uint32_t)block_nul >> 3;
	}
	// rebuild Object List
	GridCreateObjectList(TRUE);
	// Display Scores
	GridUpdateScore(0);

	// Initial 2 tiles
	GridAddRandomTile();
	GridAddRandomTile();
}