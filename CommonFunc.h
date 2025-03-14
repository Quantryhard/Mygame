// CommonFunc.h
#ifndef COMMONFUNC_H
#define COMMONFUNC_H

#include <SDL.h>

//SCREEN
static const int SCREEN_WIDTH = 1280;
static const int SCREEN_HEIGHT = 640;
static const int SCREEN_PIXEL = 32;

static const int COLOR_KEY_R = 167;
static const int COLOR_KEY_G = 175;
static const int COLOR_KEY_B = 180;

static SDL_Window* gWindow;
static SDL_Renderer* gRender;
static SDL_Event gEvent;

#define TILE_SIZE 64
#define MAX_MAP_X 400
#define MAX_MAP_Y 10
#define BLANK_TILE 0
typedef struct input{
    int left ;
    int right ;
    int up ;
    int down ;
    int isJumping ;
};
typedef struct Map{
    int startX ;//vi tri x bat dau
    int startY ;// vi tri y bat dau

    int maxX ;
    int maxY ;

    int tile[MAX_MAP_Y][MAX_MAP_X];
    char* file_name ;
};
#endif
