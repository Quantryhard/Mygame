#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "CommonFunc.h"
#include "BaseObject.h"
#include "gameMap.h"
#include "mainObject.h"
#include "impTimer.h"
using namespace std;
BaseObject gBackground ;
bool init(){
    if(SDL_Init(SDL_INIT_VIDEO)!=0){
        return false ;
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1");
    gWindow = SDL_CreateWindow("My game" , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , SCREEN_WIDTH , SCREEN_HEIGHT , SDL_WINDOW_SHOWN);
    if(gWindow == NULL){
        cout<<SDL_GetError()<<endl;
        return false;
    }
    else{
        gRender = SDL_CreateRenderer(gWindow,-1,SDL_RENDERER_ACCELERATED);
        if(gRender == NULL){
            cout<<SDL_GetError()<<endl;
            return false;
        }
        else{
            SDL_SetRenderDrawColor(gRender,0xff,0xff,0xff,0xff);
            int imFlags = IMG_INIT_PNG ;
            if(!IMG_Init(imFlags)&&imFlags){
                cout<<IMG_GetError()<<endl;
                return false;
            }
        }
    }
    return true;

}
bool loadBackground(){
    bool f = gBackground.loadIMG("image/background.png",gRender);
    if(!f){
        return false;
    }
    return true ;
}
int main(int argc , char* argv[])
{
    impTimer fps_time ;
    if(!init()){
        return -1;
    }
    if(!loadBackground()){
        return -1;
    }
    gameMap gMap;
    gMap.loadMap("map/map01.dat");
    gMap.loadTiles(gRender);

    mainObject p_player ;
    p_player.loadIMG("image/player_right.png",gRender);
    p_player.set_clips();
    bool quit = false ;
    while(!quit){
        fps_time.start();
        while(SDL_PollEvent(&gEvent) !=0){
            if(gEvent.type == SDL_QUIT){
                quit = true ;
            }
            p_player.handelInputaction(gEvent,gRender);
        }

        SDL_SetRenderDrawColor(gRender , 0xff,0xff,0xff,0xff);
        SDL_RenderClear(gRender);

        gBackground.render(gRender , NULL);

        Map map_data = gMap.getMap();

        p_player.setMapXY(map_data.startX,map_data.startY);
        p_player.doPlayer(map_data);
        p_player.show(gRender);


        gMap.setMap(map_data);
        gMap.drawMap(gRender);

        SDL_RenderPresent(gRender);
        //SDL_Delay(20);
        int real_imp_time = fps_time.get_ticks();
        int time_one_frame = 1000/FRAME_PER_SECOND;//MS
        if(real_imp_time < time_one_frame){
            int delay_time = time_one_frame - real_imp_time ;
            if(delay_time >=0){
                SDL_Delay(delay_time);
            }
        }
    }
    return 0;
}
