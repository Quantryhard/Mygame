#include"textObject.h"
textObject::textObject(){
    text_color.r = 255 ;
    text_color.g = 255 ;
    text_color.b = 255 ;

}
textObject::~textObject(){
}
bool textObject::loadFromRenderText(TTF_Font* font ,SDL_Renderer* gScreen){
    SDL_Surface* text_surface = TTF_RenderText_Solid(font,str_val.c_str(),text_color);
    if(text_surface){
        loadTexture = SDL_CreateTextureFromSurface(gScreen,text_surface);
        width = text_surface->w;
        height = text_surface->h;
        SDL_FreeSurface(text_surface);
    }
    return (loadTexture != NULL);
}
void textObject::free(){
    if(loadTexture != NULL){
        SDL_DestroyTexture(loadTexture);
        loadTexture = NULL ;
    }
}

void textObject::setColor(Uint8 red , Uint8 green , Uint8 blue){
    text_color.r = red;
    text_color.g = green ;
    text_color.b = blue ;
}
void textObject::setColor(int type){
    if(type == RED_TEXT){
        SDL_Color color = {255,0,0};
        text_color = color ;
    }
    else if(type == WHITE_TEXT){
        SDL_Color color = {255,255,255};
        text_color = color ;
    }
    else{
        SDL_Color color = {0,0,0};
        text_color = color ;
    }
}
void textObject::renderText(SDL_Renderer* gScreen ,
                    int xp , int yp ,
                    SDL_Rect* clip  ,
                    double angle  ,
                    SDL_Point* center  ,
                    SDL_RendererFlip flip )
{
    SDL_Rect renderQuad ={ xp ,yp ,width , height };
    if(clip != NULL){
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_RenderCopyEx(gScreen,loadTexture,clip,&renderQuad,angle,center,flip);














}
