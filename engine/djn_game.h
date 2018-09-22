// Game symbols
#ifndef _DJN_GAME_H_
#define _DJN_GAME_H_

extern const size_t game_data_size;
extern struct str* game_data;

extern void init();
extern void step();
extern void draw();

#endif