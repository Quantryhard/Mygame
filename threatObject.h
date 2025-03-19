#ifndef THREATOBJECT_H
#define THREATOBJECT_H
#include "BaseObject.h"
#include "CommonFunc.h"


class threatObject : public BaseObject
{
    threatObject();
    ~threatObject();
    void set_x_v(const float & xV) {x_v = xV;}
    void set_y_v(const float & yV) {y_v = yV;}

    void set_x_pos(const float &xPos) {x_pos = xPos;}
    void set_y_pos(const float &yPos) {y_pos = yPos;}

    int get_x_pos() const { return x_pos; }
    int get_y_pos() const { return y_pos; }

    void setmapXY(const int &mp_x , const int& mp_y) { map_x = mp_x ; map_y = mp_y;}

    void setClips();
    bool loadIMG(char* path , SDL_Renderer* screen) ;
    void show(SDL_Renderer* des);
    int get_width_frame() const{return width_frame;}
    int get_height_frame() const{return height_frame;}
    void doPlayer(Map& gMap);
    void checkTomap(Map& gMap);
private:
    int map_x ;
    int map_y ;
    float x_v ;
    float y_v ;
    float x_pos ;
    float y_pos ;
    bool onGround ;
    int comeBack ;
    SDL_Rect frame_clip[THREAT_FRAME_NUM] ;
    int width_frame ;
    int height_frame ;
    int frame ;

};
#endif // THREATOBJECT_H
