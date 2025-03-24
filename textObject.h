#ifndef TEXTOBJECT_H
#define TEXTOBJECT_H
#include "CommonFunc.h"

#include <SDL_ttf.h>
class textObject{
public:
    textObject();
    ~textObject();
    enum textColor{
        RED_TEXT = 0 ,
        WHITE_TEXT = 1 ,
        BLACK_TEXT = 2 ,
    };

    bool loadFromRenderText(TTF_Font* font ,SDL_Renderer* gScreen) ;
    void free();
    void setColor(Uint8 red ,Uint8 green ,Uint8 blue);
    void setColor(int type);
    void renderText(SDL_Renderer* gScreen ,
                    int xp , int yp ,
                    SDL_Rect* clip = NULL ,
                    double angle = 0.0 ,
                    SDL_Point* center = NULL ,
                    SDL_RendererFlip flip = SDL_FLIP_NONE);
    int getWidth() const{return width;}
    int getHeight() const {return height;}
    void setText(const std::string & text) {str_val = text;}
    std::string getText()const {return str_val;}
private:
    std::string str_val ;
    SDL_Color text_color ;
    SDL_Texture* loadTexture ;
    int width ;
    int height ;

};;
#endif // TEXTOBJECT_H
