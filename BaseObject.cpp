#include "BaseObject.h"
#include "CommonFunc.h"
#include <SDL_image.h>
BaseObject::BaseObject(){
    pObject = NULL ;
    rect.x = 0;
    rect.y = 0 ;
    rect.w = 0 ;
    rect.h = 0 ;
}
BaseObject::~BaseObject(){
}

bool BaseObject::loadIMG(char* path,SDL_Renderer* screen){
    free();
    SDL_Texture* newTexture = NULL ;
    SDL_Surface* loadSurface = IMG_Load(path);
    if(loadSurface != NULL){
        SDL_SetColorKey(loadSurface,SDL_TRUE,SDL_MapRGB(loadSurface->format,COLOR_KEY_R,COLOR_KEY_G,COLOR_KEY_B));
        newTexture = SDL_CreateTextureFromSurface(screen,loadSurface);
        if(newTexture!=NULL){
            rect.w = loadSurface->w;
            rect.h = loadSurface->h;
        }
        SDL_FreeSurface(loadSurface);
    }
    pObject = newTexture ;
    return (pObject != NULL);
}
void BaseObject::render(SDL_Renderer* des,const SDL_Rect* clip){
    SDL_Rect renderquad = {rect.x , rect.y , rect.w, rect.h};
    SDL_RenderCopy(des,pObject,clip,&renderquad);
}
void BaseObject::free(){
    SDL_DestroyTexture(pObject);
    pObject = NULL ;
    rect.w = 0;
    rect.h = 0;
}
