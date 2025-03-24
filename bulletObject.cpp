#include "bulletObject.h"

bulletObject::bulletObject(){
    x_v = 0 ;
    y_v = 0 ;
    is_move = false ;
    bullet_type = SPHERE_BULLET ;
};
bulletObject::~bulletObject(){
}
bool bulletObject::loadIMGBullet(SDL_Renderer* des){
    bool ret = false ;
    if(bullet_type == LASER_BULLET){
        ret = loadIMG("image/laser_bullet.png",des);
    }
    else{
        ret = loadIMG("image/sphere_bullet.png",des);
    }
    return ret ;
}
void bulletObject::handleMove(const int&x_border , const int &y_border){
    if(bullet_dir == DIR_RIGHT){
        rect.x += x_v ;
        if(rect.x > x_border){
            is_move = false ;
        }
    }
    else if(bullet_dir == DIR_LEFT){
        rect.x -= x_v ;
        if(rect.x < 0){
            is_move = false;
        }
    }
//    else if(bullet_dir == DIR_UP){
//        rect.y -= y_v;
//        if(rect.y < 0){
//            is_move = false;
//        }
//    }
//    else if(bullet_dir == DIR_UP_LEFT){
//        rect.x -= x_v;
//        rect.y -= y_v;
//        if(rect.x < 0 || rect.y < 0){
//            is_move = false;
//        }
//    }
//    else if(bullet_dir == DIR_UP_RIGHT){
//        rect.x += x_v;
//        rect.y -= y_v;
//        if(rect.x > x_border || rect.y < 0){
//            is_move = false;
//        }
//    }
//    else if(bullet_dir == DIR_DOWN_LEFT){
//        rect.x -= x_v;
//        rect.y += y_v;
//        if(rect.x < 0 || rect.y > y_border){
//            is_move = false;
//        }
//    }
//    else if(bullet_dir == DIR_DOWN_RIGHT){
//        rect.x += x_v;
//        rect.y += y_v;
//        if(rect.x > x_border || rect.y > y_border){
//            is_move = false;
//        }
//    }
}
