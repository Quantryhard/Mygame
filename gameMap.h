#ifndef GAME_MAP_H
#define GAME_MAP_H
#include "CommonFunc.h"
#include "BaseObject.h"

#define MAX_TILE 20

class tileMat : public BaseObject // kế thừa
{
public:
    tileMat(){;}
    ~tileMat(){;}

};

class gameMap{
public:
    gameMap(){;}
    ~gameMap(){;}
    void loadMap(char* name);
    void loadTiles(SDL_Renderer* screen);
    void drawMap(SDL_Renderer* screen);
    Map getMap() const{return game_map;};
    void setMap(Map& map_data) {game_map = map_data;};
private:
    Map game_map;
    tileMat tile_mat[MAX_TILE];
};
#endif // GAME_MAP_H
