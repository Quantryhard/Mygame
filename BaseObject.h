#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include "CommonFunc.h"

class BaseObject
{
public:
    BaseObject();
    ~BaseObject();
    void SetRect(const int &x , const int &y){
        rect.x = x;
        rect.y = y ;
    }
    SDL_Rect GetRect() const{return rect;}
    SDL_Texture* GetObject() const{return pObject;}

    bool loadIMG(char* path , SDL_Renderer* screen);
    void render(SDL_Renderer* des , const SDL_Rect* clip = NULL);
    void free();
protected:
    SDL_Texture* pObject ;
    SDL_Rect rect ;
};
#endif // BASEOBJECT_H
