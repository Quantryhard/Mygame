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
#include "threatObject.h"

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
vector<threatObject*> makeThreatList(){
    vector<threatObject*> list_threats;


    threatObject* threat_objs = new threatObject[20];// tĩnh

    for(int i = 0 ; i < 20 ; i++){
        threatObject* p_threat = (threat_objs+i);
        if(p_threat != NULL){
            p_threat->loadIMG("image/threat_level.png",gRender);
            p_threat->setClips();
            p_threat->set_x_pos(650+i*1200); // rải rác
            p_threat->set_y_pos(200); // độ cao rơi
            p_threat->set_type_move(threatObject::STATIC_THREAT);//gắn tĩnh
            p_threat->set_input_left(0);

            bulletObject* p_bullet = new bulletObject();
            p_threat->initBullet(p_bullet,gRender);
            list_threats.push_back(p_threat);
        }
    }
    threatObject* dynamic_threat = new threatObject[20];//động
    for(int i = 0 ; i < 20 ; i++){
        threatObject* p_threat = (dynamic_threat + i);
        if(p_threat != NULL){
            p_threat->loadIMG("image/threat_left.png",gRender);
            p_threat->setClips();
            p_threat->set_type_move(threatObject::MOVE_IN_SPACE_THREAT); // set ở dạng tĩnh
            p_threat->set_x_pos(900+i*700);
            p_threat->set_y_pos(100);

            int pos1 = p_threat->get_x_pos() - 100;
            int pos2 = p_threat->get_x_pos() + 100;
            p_threat->setAnimationpos(pos1,pos2);
            p_threat->set_input_left(1);
            list_threats.push_back(p_threat);
        }
    }
    return list_threats ;
}
void close(){
    SDL_DestroyRenderer(gRender);
    SDL_DestroyWindow(gWindow);

    gRender = NULL;
    gWindow = NULL;

    SDL_Quit();
    IMG_Quit();

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
    vector<threatObject*> threats_list = makeThreatList();
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

        p_player.handleBullet(gRender);
        p_player.setMapXY(map_data.startX,map_data.startY);
        p_player.doPlayer(map_data);
        p_player.show(gRender);

        gMap.setMap(map_data);
        gMap.drawMap(gRender);

        for(int i = 0 ; i < threats_list.size() ; i++){
            threatObject* p = threats_list.at(i);
            if(p != NULL){
                p->setmapXY(map_data.startX,map_data.startY);
                p->imMovetype(gRender);
                p->doPlayer(map_data);
                p->makeBullet(gRender,SCREEN_WIDTH,SCREEN_HEIGHT);
                p->show(gRender);
            }

        }

        vector<bulletObject*> bullet_arr = p_player.get_bullet_list();
        for(int j = 0 ; j < bullet_arr.size() ; j++){
            bulletObject* p_bullet = bullet_arr.at(j);
            if(p_bullet!=NULL){
                for(int k = 0 ; k < threats_list.size() ; k++){
                    threatObject* obj_threat = threats_list.at(k);
                    if(obj_threat != NULL){
                        SDL_Rect tRect ;
                        tRect.x = obj_threat->GetRect().x;
                        tRect.y = obj_threat->GetRect().y;
                        tRect.w = obj_threat->get_width_frame();
                        tRect.h = obj_threat->get_height_frame();

                        SDL_Rect bRect = p_bullet->GetRect();
                        bool bCol = SDLCommonFunc::checkCollision(bRect,tRect);
                        if(bCol){
                            p_player.removeBullet(j);
                            obj_threat->free();
                            threats_list.erase(threats_list.begin()+k);
                        }
                    }
                }
            }
        }
        SDL_RenderPresent(gRender);
        // SDL_Delay(100);
        int real_imp_time = fps_time.get_ticks();
        int time_one_frame = 1000/FRAME_PER_SECOND;//MS
        if(real_imp_time < time_one_frame){
            int delay_time = time_one_frame - real_imp_time ;
            if(delay_time >=0){
                SDL_Delay(delay_time);
            }
        }
    }
    for(int i = 0 ; i <threats_list.size() ; i++){
        threatObject* p_threat = threats_list.at(i);
        if(p_threat){
            p_threat->free();
            p_threat = NULL;
        }
    }
    threats_list.clear();
    close();
    return 0;
}
