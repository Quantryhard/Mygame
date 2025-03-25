#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <windows.h>
#include "CommonFunc.h"
#include "BaseObject.h"
#include "gameMap.h"
#include "mainObject.h"
#include "impTimer.h"
#include "threatObject.h"
#include "explosionObject.h"
#include "textObject.h"
using namespace std;


BaseObject gBackground ;
TTF_Font* font_time = NULL;
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
            if(TTF_Init()==-1){
                return false ;
            }
            font_time = TTF_OpenFont("font/dlxfont_.ttf",15);
            if(font_time == NULL){
                return false ;
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
            p_threat->set_x_pos(850+i*1200); // rải rác
            p_threat->set_y_pos(200); // độ cao rơi
            p_threat->set_type_move(threatObject::STATIC_THREAT);//gắn tĩnh
            p_threat->set_input_left(200);

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

    vector<threatObject*> threats_list = makeThreatList();

    explosionObject exp_threat ;
    bool tRet = exp_threat.loadIMG("image/exp3.png",gRender);
    exp_threat.setClip();
    if(!tRet) return -1;
    int heart = 0 ;

    textObject time_game ;
    time_game.setColor(textObject::WHITE_TEXT);

    textObject mark_game ;
    mark_game.setColor(textObject::WHITE_TEXT);
    UINT mark_value = 0 ;
    textObject money_game ;
    money_game.setColor(textObject::WHITE_TEXT);

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

        p_player.handleBullet(gRender);
        p_player.setMapXY(map_data.startX,map_data.startY);
        p_player.doPlayer(map_data);
        p_player.show(gRender);

        gMap.setMap(map_data);
        gMap.drawMap(gRender);

        for(int i = 0 ; i < threats_list.size() ; i++){
            threatObject* p_threat = threats_list.at(i);
            if(p_threat != NULL){
                p_threat->setmapXY(map_data.startX,map_data.startY);
                p_threat->imMovetype(gRender);
                p_threat->doPlayer(map_data);
                p_threat->makeBullet(gRender,SCREEN_WIDTH,SCREEN_HEIGHT);
                p_threat->show(gRender);
                SDL_Rect rect_player = p_player.getRectframe();
                bool bCol1 = false ;
                vector<bulletObject*> tBullet_list = p_threat->get_bullet_list();
                for(int j = 0 ; j < tBullet_list.size() ;j++){
                    bulletObject* pt_bullet = tBullet_list.at(j);
                    if(pt_bullet){
                        bCol1 = SDLCommonFunc::checkCollision(rect_player,pt_bullet->GetRect());
                        if(bCol1){

                            p_threat->removeBullet(j);
                            break;
                        }
                    }
                }


                SDL_Rect rect_threat = p_threat->getRectframe();
                bool bCol2 = false ;
                bCol2 = SDLCommonFunc::checkCollision(rect_player,rect_threat);
                if(bCol1 || bCol2){
                    heart++;
                    if(heart < 3){
                        p_player.SetRect(0,200);
                        p_player.setComebacktime(60);
                        continue;

                    }
                    else if(MessageBox(NULL,"GAME OVER", "Info" , MB_OK | MB_ICONSTOP) == IDOK){
                        p_threat->free();
                        close();
                        return 0 ;
                    }
                }



            }

        }
        int frame_exp_width = exp_threat.get_frame_width();
        int frame_exp_height = exp_threat.get_frame_height();
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
                            mark_value++ ;
                            for(int ex = 0 ; ex < NUM_FRAME_EXP ; ex++){
                                int x_pos = p_bullet->GetRect().x - frame_exp_width*0.5;
                                int y_pos = p_bullet->GetRect().y - frame_exp_height*0.5;

                                exp_threat.setFrame(ex);
                                exp_threat.SetRect(x_pos,y_pos);
                                exp_threat.show(gRender);
                            }
                            p_player.removeBullet(j);
                            obj_threat->free();
                            threats_list.erase(threats_list.begin()+k);
                        }
                    }
                }
            }
        }

        // show time
        string str_time = "Time: ";
        Uint32 time_val = SDL_GetTicks()  / 1000 ;
        Uint32 val_time = 300 - time_val ;
        if(val_time <= 0){
            if(MessageBox(NULL,"GAME OVER", "Info" , MB_OK | MB_ICONSTOP) == IDOK){
                quit = true ;
                break ;

            }
        }
        else{
            string str_val = to_string(val_time);
            str_time +=str_val ;

            time_game.setText(str_time);
            time_game.loadFromRenderText(font_time,gRender);
            time_game.renderText(gRender,SCREEN_WIDTH - 200 , 15);
        }

        string val_str_mark = to_string(mark_value) ;
        string strMark("Mark: ") ;
        strMark += val_str_mark ;
        int moneyCount = p_player.getMoneycount();
        string money_str = to_string(moneyCount);
        string moneyDisplay = "Money: ";
        moneyDisplay+=money_str;
        money_game.setText(moneyDisplay);
        money_game.loadFromRenderText(font_time,gRender);
        money_game.renderText(gRender,SCREEN_WIDTH*0.1-50,15);
        mark_game.setText(strMark);
        mark_game.loadFromRenderText(font_time,gRender);
        mark_game.renderText(gRender,SCREEN_WIDTH*0.5-50,15);
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
