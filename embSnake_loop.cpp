#include "embSnake_loop.h"

uint8_t get_rand();
uint8_t get_rand_max(uint8_t max_val);
uint8_t get_rand_color();

//#include <iostream>
//using namespace std;

#define CONSTRAIN_X(X) (X < 1 ? LEVEL_WIDTH : (X > LEVEL_WIDTH ? 1 : X)) 
#define CONSTRAIN_Y(Y) (Y < 1 ? LEVEL_HEIGHT : (Y > LEVEL_HEIGHT ? 1 : Y)) 

uint8_t next_position(uint8_t* x, uint8_t* y, level_t* level, uint8_t* event)
{
	uint8_t head_type;
	uint8_t tx, ty;
	uint8_t rotation;

	head_type = level->tile[*y][*x].type;
	if(*event == EV_NONE){
		switch(head_type){
		case TYPE_HEAD_R:
			tx = *x + 1;
			ty = *y;
			rotation = 0;
			break;
		case TYPE_HEAD_U:
			tx = *x;
			ty = *y - 1;
			rotation = 1;
			break;
		case TYPE_HEAD_L:
			tx = *x - 1;
			ty = *y;
			rotation = 2;
			break;
		case TYPE_HEAD_D:
			tx = *x;
			ty = *y + 1;
			rotation = 3;
			break;
		default:
			rotation = 0;
			//Do nothing
		}
	}else if(*event & EV_LEFT){
		*event &= ~EV_LEFT;
		switch(head_type){
		case TYPE_HEAD_R:
			tx = *x;
			ty = *y - 1;
			rotation = 1;
			break;
		case TYPE_HEAD_U:
			tx = *x - 1;
			ty = *y;
			rotation = 2;
			break;
		case TYPE_HEAD_L:
			tx = *x;
			ty = *y + 1;
			rotation = 3;
			break;
		case TYPE_HEAD_D:
			tx = *x + 1;
			ty = *y;
			rotation = 0;
			break;
		default:
			rotation = 0;
			//Do nothing
		}
	}else if(*event & EV_RIGHT){
		*event &= ~EV_RIGHT;
		switch(head_type){
		case TYPE_HEAD_R:
			tx = *x;
			ty = *y + 1;
			rotation = 3;
			break;
		case TYPE_HEAD_U:
			tx = *x + 1;
			ty = *y;
			rotation = 0;
			break;
		case TYPE_HEAD_L:
			tx = *x;
			ty = *y - 1;
			rotation = 1;
			break;
		case TYPE_HEAD_D:
			tx = *x - 1;
			ty = *y;
			rotation = 2;
			break;
		default:
			rotation = 0;
			//Do nothing
		}
	}else{
		rotation = 0;
		//Do nothing
	}

	*x = CONSTRAIN_X(tx);
	*y = CONSTRAIN_Y(ty);

	return rotation;
}

uint8_t prev_position(uint8_t* x, uint8_t* y, level_t* level)
{
	uint8_t type;
	uint8_t tx, ty;
	uint8_t rotation;

	type = level->tile[*y][*x].type;

	switch(type){
	case TYPE_HEAD_R:
	case TYPE_TAIL_R:
	case TYPE_LINE_R:
	case TYPE_TURN_LU:
	case TYPE_TURN_LD:
		tx = *x - 1;
		ty = *y;
		rotation = 0;
		break;
	case TYPE_HEAD_U:
	case TYPE_TAIL_U:
	case TYPE_LINE_U:
	case TYPE_TURN_DR:
	case TYPE_TURN_DL:
		tx = *x;
		ty = *y + 1;
		rotation = 1;
		break;
	case TYPE_HEAD_L:
	case TYPE_TAIL_L:
	case TYPE_LINE_L:
	case TYPE_TURN_RU:
	case TYPE_TURN_RD:
		tx = *x + 1;
		ty = *y;
		rotation = 2;
		break;
	case TYPE_HEAD_D:
	case TYPE_TAIL_D:
	case TYPE_LINE_D:
	case TYPE_TURN_UR:
	case TYPE_TURN_UL:
		tx = *x;
		ty = *y - 1;
		rotation = 3;
		break;
	default:
		rotation = 0;
		//Do nothing
	}

	*x = CONSTRAIN_X(tx);
	*y = CONSTRAIN_Y(ty);

	return rotation;
}

uint8_t get_next_head(uint8_t next_rotation)
{
	switch(next_rotation){
	case 0:
		return TYPE_HEAD_R;
	case 1:
		return TYPE_HEAD_U;
	case 2:
		return TYPE_HEAD_L;
	case 3:
		return TYPE_HEAD_D;
	default:
		return TYPE_NONE;
	}
}

uint8_t get_prev_tail(uint8_t rotation)
{
	switch(rotation){
	case 0:
		return TYPE_TAIL_R;
	case 1:
		return TYPE_TAIL_U;
	case 2:
		return TYPE_TAIL_L;
	case 3:
		return TYPE_TAIL_D;
	default:
		return TYPE_NONE;
	}
}

uint8_t get_new_segment(uint8_t next_rotation, uint8_t rotation)
{
	uint8_t cr = (next_rotation << 2) | rotation;
	switch(cr){
	case (0 << 2) | (0 << 0):
	case (0 << 2) | (2 << 0):
		return TYPE_LINE_R;
	case (2 << 2) | (0 << 0):
	case (2 << 2) | (2 << 0):
		return TYPE_LINE_L;
	case (1 << 2) | (1 << 0):
	case (1 << 2) | (3 << 0):
		return TYPE_LINE_U;
	case (3 << 2) | (1 << 0):
	case (3 << 2) | (3 << 0):
		return TYPE_LINE_D;
	case (0 << 2) | (1 << 0):
		return TYPE_TURN_DR;
	case (0 << 2) | (3 << 0):
		return TYPE_TURN_UR;
	case (2 << 2) | (1 << 0):
		return TYPE_TURN_DL;
	case (2 << 2) | (3 << 0):
		return TYPE_TURN_UL;
	case (1 << 2) | (0 << 0):
		return TYPE_TURN_LU;
	case (1 << 2) | (2 << 0):
		return TYPE_TURN_RU;
	case (3 << 2) | (0 << 0):
		return TYPE_TURN_LD;
	case (3 << 2) | (2 << 0):
		return TYPE_TURN_RD;
	default:
		return TYPE_NONE;
	}
}



void game_reset(level_t* level)
{
	uint8_t i, j;
	uint8_t x, y, r;

	for(i = 1; i <= LEVEL_WIDTH; i++){
		level->tile[0][i].type = TYPE_LWALL_D;
		level->tile[LEVEL_HEIGHT+1][i].type = TYPE_LWALL_U;
	}

	for(i = 1; i <= LEVEL_HEIGHT; i++){
		level->tile[i][0].type = TYPE_LWALL_R;
		level->tile[i][LEVEL_WIDTH+1].type = TYPE_LWALL_L;
	}

	for(i = 1; i <= LEVEL_HEIGHT; i++){
		for(j = 1; j <= LEVEL_WIDTH; j++){
			level->tile[i][j].type = TYPE_NONE;
		}
	}

	level->tile[0][0].type = TYPE_CWALL_DR;
	level->tile[0][LEVEL_WIDTH+1].type = TYPE_CWALL_LD;
	level->tile[LEVEL_HEIGHT+1][0].type = TYPE_CWALL_RU;
	level->tile[LEVEL_HEIGHT+1][LEVEL_WIDTH+1].type = TYPE_CWALL_UL;

	//Test code:
	x = get_rand_max(LEVEL_WIDTH) + 1;
	y = get_rand_max(LEVEL_HEIGHT) + 1;
	level->tile[y][x].type = TYPE_HEAD_R;
	
	level->x = x;
	level->y = y;

	r = prev_position(&x, &y, level);
	level->tile[y][x].type = get_prev_tail(r);
	level->tile[y][x].color = get_rand_color();
	
	level->pause_frames = 10;
	level->place_apples = 2;
}

#define IS_TAIL(TYPE) ((TYPE&0xf0) == (TYPE_TAIL_R&0xf0))

void game_loop(level_t* level, uint8_t* event)
{
	uint8_t x, y, r;
	uint8_t nx, ny, nr;

	uint8_t insert_part = 0;
	uint8_t first_part = 0;
	uint8_t insert_part_color = 0;

	nx = level->x;
	ny = level->y;

	x = nx;
	y = ny;

	if(level->pause_frames){
		level->pause_frames--;
		return;
	}

	nr = next_position(&nx, &ny, level, event);

	if(level->tile[ny][nx].type == TYPE_APPLE){
		insert_part = 1;
		insert_part_color = level->tile[ny][nx].color;
		level->place_apples++;
	}else if(level->tile[ny][nx].type != TYPE_NONE){
		game_reset(level);
		return;
	}

	level->tile[ny][nx].type = get_next_head(nr);
	level->x = nx;
	level->y = ny;
	first_part = 1;
	
	//cout << "New loop:" << (int)nx << "," << (int)ny << "\r\n";
	//cout << "First:" << (int)x << "," << (int)y << "\r\n";

	nx = x;
	ny = y;
	while(1){ //Loop until tail is found
		

		r = prev_position(&x, &y, level);

		//cout << "Next:" << (int)x << "," << (int)y << "\r\n";

		if(IS_TAIL(level->tile[y][x].type)){
			if(insert_part){
				level->tile[ny][nx].color = level->tile[y][x].color;
				level->tile[y][x].color = insert_part_color;
			}else{
				level->tile[ny][nx].color = level->tile[y][x].color;
				level->tile[ny][nx].type = get_prev_tail(nr);
				level->tile[y][x].type = TYPE_NONE;
			}
			break;
		}else if(first_part){
			//cout << "Rots:" << (int)nr << "," << (int)r << "\r\n";
			level->tile[ny][nx].type = get_new_segment(nr, r);
			//cout << "Type:" << (int)level->tile[ny][nx].type << "\r\n";
			first_part = 0;
		}
		level->tile[ny][nx].color = level->tile[y][x].color;

		nx = x;
		ny = y;
		nr = r;
	}

	if(level->place_apples){
		x = get_rand_max(LEVEL_WIDTH) + 1;
		y = get_rand_max(LEVEL_HEIGHT) + 1;
		if(level->tile[y][x].type == TYPE_NONE){
			level->place_apples--;
			level->tile[y][x].type = TYPE_APPLE;
			level->tile[y][x].color = get_rand_color();
		}
	}
}