#ifndef _GRID_H
#define _GRID_H

#include <stdint.h>
#include <jaglib.h>

enum Direction
{
	fromTop,
	fromBottom,
	fromLeft,
	fromRight
};

enum Game
{
	Over,
	Go,
	Won
};

void GridCreate(op_bmp_object *root, uint32_t base);
void GridMoveTiles(enum Direction direction);
void GridGetObjectList(uint64_t *dest);
void GridNewGame();
void GridKeepPlaying();
enum Game GridGameState();

#endif