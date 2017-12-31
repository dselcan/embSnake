#include "embSnake_draw.h"

#include "embSnake_graphics.h"

void reset_trow_location(draw_location_t* draw_location)
{
	draw_location->x_level = 0;
	draw_location->y_level = 0;
	draw_location->y_offset = 0;
}

void get_next_trow(draw_location_t* draw_location, level_t* level, uint16_t* trow)
{
	uint16_t pix;
	uint8_t i;
	uint8_t ax, ay;
	uint8_t color, color_value;

	uint8_t image_num = 0;
	uint8_t rotate = 0; //By 90 degrees, counter-clockwise, initial position = ->
	
	image_num = level->tile[draw_location->y_level][draw_location->x_level].type;
	color = level->tile[draw_location->y_level][draw_location->x_level].color;
	rotate = image_num&0x03;
	image_num = image_num >> 4;

	if(rotate == 0){
		ay = draw_location->y_offset;
	}else if(rotate == 1){
		ax = TILE_SIZE - draw_location->y_offset - 1;
	}else if(rotate == 2){
		ay = TILE_SIZE - draw_location->y_offset - 1;
	}else if(rotate == 3){
		ax = draw_location->y_offset;
	}

	for(i = 0; i < TILE_SIZE; i++){
		if(rotate == 0){
			ax = i;
		}else if(rotate == 1){
			ay = i;
		}else if(rotate == 2){
			ax = TILE_SIZE - i - 1;
		}else if(rotate == 3){
			ay = TILE_SIZE - i - 1;
		}

		switch(image_num){
		case IMG_SNAKE_HEAD_NUM:
			pix = IMG_SNAKE_HEAD[ay][ax];
			break;
		case IMG_SNAKE_TAIL_NUM:
			pix = IMG_SNAKE_TAIL[ay][ax];
			break;
		case IMG_SNAKE_LINE_NUM:
			color_value = IMG_SNAKE_LINE_CM[ay][ax];
			if(color_value){
				pix = COLORS[color][color_value-1];
			}else{
				pix = IMG_SNAKE_LINE[ay][ax];
			}
			break;
		case IMG_SNAKE_TURN_NUM:
			color_value = IMG_SNAKE_TURN_CM[ay][ax];
			if(color_value){
				pix = COLORS[color][color_value-1];
			}else{
				pix = IMG_SNAKE_TURN[ay][ax];
			}
			break;
		case IMG_APPLE_NUM:
			color_value = IMG_APPLE_CM[ay][ax];
			if(color_value){
				pix = COLORS[color][color_value-1];
			}else{
				pix = IMG_APPLE[ay][ax];
			}
			break;	
		case IMG_WALL_LINE_NUM:
			pix = IMG_WALL_LINE[ay][ax];
			break;
		case IMG_WALL_CORNER_NUM:
			pix = IMG_WALL_CORNER[ay][ax];
			break;
		case IMG_TILE_NUM:
		default:
			pix = IMG_TILE[ay][ax];
		}

		trow[i] = pix;
	}

	draw_location->x_level++;
	if(draw_location->x_level >= LEVEL_WIDTH + 2){
		draw_location->x_level = 0;
		draw_location->y_offset++;
		if(draw_location->y_offset >= TILE_SIZE){
			draw_location->y_offset = 0;
			draw_location->y_level++;
		}
	}
}

