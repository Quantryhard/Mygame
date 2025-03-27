#ifndef BULLETOBJECT_H
#define BULLETOBJECT_H

#include "BaseObject.h"
#include "CommonFunc.h"

class bulletObject : public BaseObject {
public:
    bulletObject();
    ~bulletObject();
    enum bulletDir {
        DIR_RIGHT = 20,
        DIR_LEFT = 21,
        DIR_UP = 22,
        DIR_UP_LEFT = 23,
        DIR_UP_RIGHT = 24,
        DIR_DOWN_LEFT = 25,
        DIR_DOWN_RIGHT = 26,
    };
    enum bulletType {
        SPHERE_BULLET = 50,
        LASER_BULLET = 51,
    };
    void set_x_val(const int& x_val) { x_v = x_val; }
    void set_y_val(const int& y_val) { y_v = y_val; }
    int get_x_val() const { return x_v; }
    int get_y_val() const { return y_v; }

    void set_is_move(const bool& isMove) { is_move = isMove; }
    bool get_is_move() const { return is_move; }

    void set_bullet_dir(const unsigned int bullet_dir_) { bullet_dir = bullet_dir_; }

    void set_bullet_type(const unsigned int& bulletType) { bullet_type = bulletType; }
    unsigned int get_bullet_type() const { return bullet_type; }
    void handleMove(const int& x_border, const int& y_border);
    int get_bullet_dir() const { return bullet_dir; }
    bool loadIMGBullet(SDL_Renderer* des);

    // Thêm hàm kiểm tra va chạm với bản đồ
    bool checkCollisionWithMap(const Map& map_data);
    void set_initial_pos(const int& x, const int& y) { initial_x = x; initial_y = y; }
    int get_initial_x() const { return initial_x; }
    int get_initial_y() const { return initial_y; }

private:
    int x_v;
    int y_v;
    bool is_move;
    unsigned int bullet_dir;
    unsigned int bullet_type;
    int initial_x; // Vị trí ban đầu của viên đạn (x)
    int initial_y; // Vị trí ban đầu của viên đạn (y)
};
#endif // BULLETOBJECT_H
