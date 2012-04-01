#ifndef INCLUDED_DIRECTION_H
#define INCLUDED_DIRECTION_H

#include "h-basic.h"

enum {
	DIR_UNKNOWN = 0,
	DIR_NW = 1,
	DIR_N = 2,
	DIR_NE = 3,
	DIR_W = 4,
	DIR_TARGET = 5,
	DIR_NONE = 5,
	DIR_E = 6,
	DIR_SW = 7,
	DIR_S = 8,
	DIR_SE = 9,
};

enum {
	MOVE_UNKNOWN = 0,
	MOVE_SW = 1,
	MOVE_S = 2,
	MOVE_SE = 3,
	MOVE_W = 4,
	MOVE_TARGET = 5,
	MOVE_NONE = 5,
	MOVE_E = 6,
	MOVE_NW = 7,
	MOVE_N = 8,
	MOVE_NE = 9,
};

extern const s16b ddd[9];
extern const s16b ddx[10];
extern const s16b ddy[10];
extern const s16b ddx_ddd[9];
extern const s16b ddy_ddd[9];

int motion_dir(int from_y, int from_x, int to_y, int to_x);

#endif
