#ifndef _EMBSNAKE_DRAW_H_
#define _EMBSNAKE_DRAW_H_

#include "stdint.h"
#include "embSnake_loop.h"

#define SCREEN_WIDTH 		220
#define SCREEN_HEIGHT 		176
#define TILE_SIZE			11

typedef struct draw_location_t { 
	uint8_t x_level;
	uint8_t y_level;
	uint8_t y_offset;
} draw_location_t;

void reset_trow_location(draw_location_t* draw_location);
void get_next_trow(draw_location_t* draw_location, level_t* level, uint16_t* trow);

#endif