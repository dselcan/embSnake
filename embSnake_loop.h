#ifndef _EMBSNAKE_LOOP_H_
#define _EMBSNAKE_LOOP_H_

#include "stdint.h"

#define LEVEL_WIDTH		18
#define LEVEL_HEIGHT	14

#define TYPE_NONE 		0x00

#define TYPE_HEAD_R 	0x10
#define TYPE_HEAD_U 	0x11
#define TYPE_HEAD_L		0x12
#define TYPE_HEAD_D		0x13

#define TYPE_TAIL_R 	0x20
#define TYPE_TAIL_U 	0x21
#define TYPE_TAIL_L		0x22
#define TYPE_TAIL_D		0x23

#define TYPE_LINE_R 	0x30
#define TYPE_LINE_U 	0x31
#define TYPE_LINE_L 	0x32
#define TYPE_LINE_D 	0x33

#define TYPE_TURN_RU 	0x40
#define TYPE_TURN_UL 	0x41
#define TYPE_TURN_LD	0x42
#define TYPE_TURN_DR	0x43
#define TYPE_TURN_UR 	0x44
#define TYPE_TURN_LU 	0x45
#define TYPE_TURN_DL	0x46
#define TYPE_TURN_RD	0x47

#define TYPE_APPLE		0x50

#define TYPE_LWALL_R 	0x60
#define TYPE_LWALL_U 	0x61
#define TYPE_LWALL_L	0x62
#define TYPE_LWALL_D	0x63

#define TYPE_CWALL_RU 	0x70
#define TYPE_CWALL_UL 	0x71
#define TYPE_CWALL_LD	0x72
#define TYPE_CWALL_DR	0x73

#define EV_NONE			0
#define EV_LEFT			1
#define EV_RIGHT		2

typedef struct tile_t { 
    uint8_t type;
	uint8_t color;
} tile_t;

typedef struct level_t {
	tile_t tile[LEVEL_HEIGHT+2][LEVEL_WIDTH+2];
	uint8_t x, y;
	uint8_t pause_frames;
	uint8_t place_apples;
} level_t;

void game_reset(level_t* level);
void game_loop(level_t* level, uint8_t* event);

#endif