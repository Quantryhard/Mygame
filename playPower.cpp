#include "playPower.h"

playPower::playPower(){
    number_ = 0 ;

}
playPower::~playPower(){

}
void playPower::addPos(const int& xPos){
    pos_list.push_back(xPos);
}
void playPower::init(SDL_Renderer* gScreen){
    loadIMG("image/player_pw.png",gScreen);
    number_ = 3 ;
    if(pos_list.size() > 0){
        pos_list.clear();
    }
    addPos(20);
    addPos(60);
    addPos(100);
}

void playPower::decrease(){
    number_--;
    pos_list.pop_back();
}
void playPower::show(SDL_Renderer* gScreen){
    for(int i = 0 ; i < pos_list.size() ; i++){
        rect.x = pos_list.at(i);
        rect.y = 0 ;
        render(gScreen);
    }
}
void playPower::initIncrease(){
    number_++;
    int last_pos = pos_list.back();
    last_pos +=40 ;
    pos_list.push_back(last_pos);
}

playMoney::playMoney(){
    x_pos = 0;
    y_pos = 0;
}
playMoney::~playMoney(){
}
void playMoney::init(SDL_Renderer* gScreen){
    loadIMG("image/money_img.png",gScreen);

}
void playMoney::show(SDL_Renderer* gScreen){
    rect.x = x_pos ;
    rect.y = y_pos ;
    render(gScreen);
}
