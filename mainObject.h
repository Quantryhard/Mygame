#ifndef MAINOBJECT_H
#define MAINOBJECT_H

#include "CommonFunc.h"
#include "BaseObject.h"
class mainObject : public BaseObject
{
public:
    mainObject();
    ~mainObject();
    enum walkType{
        WALK_RIGHT = 0,
        WALK_LEFT = 1,
        WALK_UP = 2,
        WALK_DOWN = 3,
    };
    virtual bool loadIMG(char* path , SDL_Renderer* screen);
    void show(SDL_Renderer* des);
    void handelInputaction(SDL_Event e,SDL_Renderer* screen);
    void set_clips();// xu li animetion
    void doPlayer(Map &map_data);
    void checkTomap(Map& map_data);
    void setMapXY(const int map_x , const int map_y){map_x_ = map_x; map_y_ = map_y;};
    void centerEntityonmap(Map& map_data);
private:
    float x_v ; // speed x
    float y_v ; // speed y
    float x_pos ; // vị trí x
    float y_pos ; // vị trí y

    int width_frame ; // chiều rộng ảnh
    int height_frame ; // chiều dài ảnh

    SDL_Rect frame_clip[8];
    input input_type ; // kiểu đầu vào
    int frame ; //
    int status ; // trạng thái trái phải lên xuống
    bool onGround ;
    int map_x_;
    int map_y_;
    int comeBack ;
};
#endif // MAINOBJECT_H
