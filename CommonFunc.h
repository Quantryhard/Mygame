// CommonFunc.h
#ifndef COMMONFUNC_H
#define COMMONFUNC_H
#include <SDL.h>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>

//SCREEN
static const int SCREEN_WIDTH = 1280;
static const int SCREEN_HEIGHT = 640;
static const int COLOR_KEY_R = 167;
static const int COLOR_KEY_G = 175;
static const int COLOR_KEY_B = 180;
static const int FRAME_PER_SECOND = 60;
static SDL_Window* gWindow;
static SDL_Renderer* gRender;
static SDL_Event gEvent;

#define TILE_SIZE 64 // pixel
#define MAX_MAP_X 400 // 400 dong
#define MAX_MAP_Y 10 // 10 cot
#define BLANK_TILE 0 // o trong
#define STATE_MONEY 4 // dong tien
#define MAX_FALL_SPEED 10 // gioi han roi tu do
#define PLAYER_SPEED 8 // toc do nhan vat
#define JUMP -20 // luc nhay
#define THREAT_FRAME_NUM 8// so frame anh bot
#define THREAT_GRAVITY_SPEED 10 /// độ rơi tự do của threat
#define COMEBACK_PLAYER 60 // thoi gian hoi sinh
#define THREAT_SPEED 3
#define FLAG_TILE 10
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

    int maxX ;//vị trí mapx hiện tại
    int maxY ;// vị trí mapy hiện tại

    int tile[MAX_MAP_Y][MAX_MAP_X]; // mảng chứa map
    char* file_name ;// tên map load lại nếu cần
};
namespace SDLCommonFunc
{
    bool checkCollision(const SDL_Rect& object1 , const SDL_Rect& object2);
}
#endif
