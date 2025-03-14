#include "impTimer.h"
#include "CommonFunc.h"
impTimer::impTimer(){
    start_tick = 0;
    paused_tick = 0 ;
    is_paused_ = false ;
    is_started_ = false ;
}

impTimer::~impTimer(){
}
void impTimer::start(){
    is_started_ = true ;
    is_paused_ = true ;
}
void impTimer::stop(){
    is_started_ = false ;
    is_paused_ = false ;
}
void impTimer::paused(){
    if(is_started_ == true && is_paused_ == false){
        is_paused_= false ;
        paused_tick = SDL_GetTicks() - start_tick ;
    }
}
void impTimer::unpaused(){
    if(is_paused_ == true){
        is_paused_ = false ;
        start_tick = SDL_GetTicks() - paused_tick;
        paused_tick = 0 ;
    }
}
int impTimer::get_ticks(){
     if(is_started_==true){
        if(is_paused_ == true){
            return paused_tick ;
        }
        else{
            return SDL_GetTicks() - start_tick;
        }
     }
     return 0;
}
bool impTimer::is_started(){
    return is_started_;
}
bool impTimer::is_paused(){
    return is_paused_;
}
