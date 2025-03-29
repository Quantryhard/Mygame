#ifndef MAINOBJECT_H
#define MAINOBJECT_H

#include "CommonFunc.h"
#include "BaseObject.h"
#include "bulletObject.h"
#include <SDL_mixer.h>
class mainObject : public BaseObject
{
public:
    mainObject();
    ~mainObject();
    enum walkType{
        WALK_NONE = 0,
        WALK_RIGHT = 1,
        WALK_LEFT = 2,
//        WALK_UP = 3,
//        WALK_DOWN = 4,
    };
    virtual bool loadIMG(char* path , SDL_Renderer* screen);
    void show(SDL_Renderer* des);
    void handelInputaction(SDL_Event e,SDL_Renderer* screen);
    void set_clips();// xu li animetion
    void doPlayer(Map &map_data);
    void checkTomap(Map& map_data);
    void setMapXY(const int map_x , const int map_y){map_x_ = map_x; map_y_ = map_y;};
    void centerEntityonmap(Map& map_data);
    void updateImageplayer(SDL_Renderer* des );
    void set_bullet_list(std::vector<bulletObject*> bullet_list){
        p_bullet_list = bullet_list ;
    }
    std::vector<bulletObject*> get_bullet_list() const{return p_bullet_list;}
    void removeBullet(const int &indx);
    void increase_money();
    void handleBullet(SDL_Renderer* des , const Map& map_data);
    void setComebacktime(const int cb){comeBack = cb;}
    SDL_Rect getRectframe();
    int getMoneycount() const{return money_count;}
    bool hasFallen() const { return has_fallen; }
    void resetFallen() { has_fallen = false; }
    bool hasReachedFlag() const { return has_reached_flag; }
    void resetFlag() { has_reached_flag = false; }

    void setSoundBeep(Mix_Chunk* beep) { sound_beep = beep; }
    void setSoundLaser(Mix_Chunk* laser) { sound_laser = laser; }
private:
    std::vector<bulletObject*> p_bullet_list ; // danh sách đạn
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
    int money_count ;
    bool has_fallen; // ktra rơi vực
    bool has_reached_flag; // chạm cờ
    Mix_Chunk* sound_beep;
    Mix_Chunk* sound_laser;

};
#endif // MAINOBJECT_H
