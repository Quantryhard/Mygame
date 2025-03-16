#ifndef BULLETOBJECT_H
#define BULLETOBJECT_H

#include "BaseObject.h"
#include "CommonFunc.h"
class bulletObject : public BaseObject
{
public:
    bulletObject();
    ~bulletObject();
    enum bulletDir{
        DIR_RIGHT = 20,
        DIR_LEFT = 21,
        DIR_UP = 22,
        DIR_UP_LEFT = 23,
        DIR_UP_RIGHT = 24,
        DIR_DOWN_LEFT = 25 ,
        DIR_DOWN_RIGHT = 26,
    };
    void set_x_val(const int&x_val) {x_v = x_val;};
    void set_y_val(const int&y_val) {y_v = y_val;};
    int get_x_val()const {return x_v;}
    int get_y_val()const {return y_v;}

    void set_is_move(const bool& isMove){is_move = isMove;}
    bool get_is_move() const {return is_move;}

    void set_bullet_dir(const unsigned int bullet_dir_) {bullet_dir = bullet_dir_;}
    void handleMove(const int &x_border , const int &y_border);
    int get_bullet_dir() const{return bullet_dir;}
private:
    int x_v ;
    int y_v ;
    bool is_move ;
    unsigned int bullet_dir ;
};
#endif // BULLETOBJECT_H
