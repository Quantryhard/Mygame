
#include "explosionObject.h"

explosionObject::explosionObject(){
    frame_height = 0 ;
    frame_width = 0;
    frame_ = 0;
}
explosionObject::~explosionObject(){

}
bool explosionObject::loadIMG(char* path , SDL_Renderer* gScreen){
    bool ret = BaseObject::loadIMG(path ,gScreen);
    if(ret){
        frame_width = rect.w /NUM_FRAME_EXP;
        frame_height = rect.h;
    }
    return ret ;
}
void explosionObject::setClip(){
    if(frame_width > 0 && frame_height > 0){
        for(int i = 0 ; i < NUM_FRAME_EXP ; i++){
            frame_clip[i].x = frame_width*i;
            frame_clip[i].y = 0;
            frame_clip[i].w = frame_width;
            frame_clip[i].h = frame_height;
        }
    }
}
void explosionObject::show(SDL_Renderer* gScreen){
    SDL_Rect* current_clip = &frame_clip[frame_];
    SDL_Rect renderQuad = {rect.x,rect.y,frame_width,frame_height};
    if(current_clip!= NULL){
        renderQuad.w = current_clip->w;
        renderQuad.h = current_clip->h;
    }
    SDL_RenderCopy(gScreen,pObject,current_clip,&renderQuad);
}
