#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "CommonFunc.h"
#include "BaseObject.h"

#define NUM_FRAME_EXP 8

class explosionObject : public BaseObject
{
public:
    explosionObject();
    ~explosionObject();
    void setClip();
    void setFrame(const int& fr) {frame_ = fr;}
    virtual bool loadIMG(char* path,SDL_Renderer* gScreen);
    void show(SDL_Renderer* gScreen);
    int get_frame_width() const {return frame_width;}
    int get_frame_height() const {return frame_height;}
private:
    int frame_width;
    int frame_height;
    unsigned int frame_ ;
    SDL_Rect frame_clip[NUM_FRAME_EXP];

};
#endif // EXPLOSION_H
