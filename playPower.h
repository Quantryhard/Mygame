#ifndef PLAYPOWER_H
#define PLAYPOWER_H
#include "CommonFunc.h"
#include "BaseObject.h"

class playPower : public BaseObject
{
public:
    playPower();
    ~playPower();
    void setNum(const int& num){number_ = num;}
    void addPos(const int& xPos);
    void show(SDL_Renderer* gScreen);
    void init(SDL_Renderer* gScreen);

    void initIncrease();
    void decrease();
private:
    int number_ ;
    std::vector<int> pos_list ;


};

class playMoney : public BaseObject
{

public:
    playMoney();
    ~playMoney();
    void init(SDL_Renderer* gScreen);
    void show(SDL_Renderer* gScreen);
    void setPos(const int &xp , const int &yp){x_pos = xp ;  y_pos = yp;}
private:
    int x_pos ;
    int y_pos ;

};
#endif // PLAYPOWER_H
