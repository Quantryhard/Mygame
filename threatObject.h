#ifndef THREATOBJECT_H
#define THREATOBJECT_H
#include "BaseObject.h"
#include "CommonFunc.h"
#include "bulletObject.h"

class threatObject : public BaseObject
{
public:
    enum typeMove{
        STATIC_THREAT = 0 ,
        MOVE_IN_SPACE_THREAT = 1 ,
    };
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
    void doPlayer(Map& gMap , SDL_Renderer* gScreen);
    void initThreat();
    void checkTomap(Map& gMap , SDL_Renderer* gScreen);

    void set_type_move(const int& typeMove_){type_move = typeMove_;}
    void setAnimationpos(const int &pos_l , const int& pos_r){animation_l = pos_l ; animation_r = pos_r; }
    void set_input_left(const int& tLeft){input_type.left = tLeft;}
    void imMovetype(SDL_Renderer* gScreen);
    std::vector<bulletObject*> get_bullet_list()const{return bullet_list;}
    void set_bullet_list(const std::vector<bulletObject*> &bl_list){bullet_list = bl_list;}
    void initBullet(bulletObject* p_bullet , SDL_Renderer* gScreen);
    void makeBullet(SDL_Renderer* gScreen , const int& x_limit , const int& y_limit);
    void removeBullet(const int& indx);
    SDL_Rect getRectframe();
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
    int type_move ;
    int animation_l ;
    int animation_r ;
    input input_type ;
    std::vector<bulletObject*> bullet_list ;
};
#endif // THREATOBJECT_H
