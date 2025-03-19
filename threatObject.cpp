
#include"threatObject.h"

threatObject::threatObject(){
    width_frame = 0 ;
    height_frame = 0 ;
    x_v = 0;
    y_v = 0;
    x_pos = 0;
    y_pos = 0;
    onGround = 0;
    comeBack = 0;
    frame = 0 ;
}
threatObject::~threatObject(){
}
bool threatObject::loadIMG(char* path,SDL_Renderer* screen){
    bool ret = BaseObject::loadIMG(path,screen);
    if(ret){
        width_frame = rect.w/THREAT_FRAME_NUM;
        height_frame = rect.h;
    }
}
void threatObject::setClips(){
    if(width_frame > 0 && height_frame > 0){
        for(int i = 0 ; i < THREAT_FRAME_NUM ; i++){
            frame_clip[i].x = i*width_frame;
            frame_clip[i].y = 0;
            frame_clip[i].w = width_frame ;
            frame_clip[i].h = height_frame ;
        }
    }
}
void threatObject::show(SDL_Renderer* des){
    if(comeBack == 0){
        rect.x = x_pos - map_x ;
        rect.y = y_pos - map_y ;
        frame = (frame + 1)%THREAT_FRAME_NUM;
        SDL_Rect* currentClip = &frame_clip[frame];
        SDL_Rect renderQuad ={rect.x,rect.y,width_frame,height_frame};
        SDL_RenderCopy(des,pObject,currentClip,&renderQuad);
    }
}
void threatObject::doPlayer(Map& gMap){
    if(comeBack == 0){
        x_v = 0;
        y_v +=THREAT_SPEED ;
        if(y_v >= THREAT_SPEED){
            y_v = THREAT_SPEED ;
        }
        checkTomap(gMap);

    }
    else if(comeBack > 0){
        comeBack -- ;
        if(comeBack == 0){
            x_v = 0;
            y_v = 0;
            if(x_pos > 256){
                x_pos = 256;
            }
            else{
                x_pos = 0;
            }
            y_pos = 0 ;
            comeBack = 0;
        }
    }
}
void threatObject::checkTomap(Map& gMap){
    int x1 = 0 ;
    int x2 = 0 ;

    int y1 = 0;
    int y2 = 0;
    //check theo width
    int height_min = height_frame < TILE_SIZE ? height_frame : TILE_SIZE ;
    x1 = (x_pos + x_v)/TILE_SIZE;
    x2 = (x_pos + x_v + width_frame - 1)/TILE_SIZE;

    y1 = (y_pos)/TILE_SIZE ;
    y2 = (y_pos + height_min - 1)/TILE_SIZE;

    if(x1 >= 0 && x2 < MAX_MAP_X && y1>=0 && y2 <MAX_MAP_Y){
        if(x_v > 0){ // khi nhan vat chuyen sang phai
            if(gMap.tile[y1][x2] != BLANK_TILE || gMap.tile[y2][x2] != BLANK_TILE){
                x_pos = x2*TILE_SIZE-width_frame-1;
                x_v = 0;
                }
            }
        else if(x_v < 0){
            if(gMap.tile[y1][x1] != BLANK_TILE || gMap.tile[y2][x1] != BLANK_TILE){
                x_pos =(x1+1)*TILE_SIZE ;
                x_v = 0 ;
            }
        }
    }
    // check doc
    int width_min = width_frame < TILE_SIZE ? width_frame : TILE_SIZE ;
    x1 = (x_pos)/TILE_SIZE ;
    x2 = (x_pos + width_min) / TILE_SIZE ;

    y1 = (y_pos + y_v )/TILE_SIZE ;
    y2 = (y_pos + y_v + height_frame - 1)/TILE_SIZE;

    if(x1 >=0 && x2 <MAX_MAP_X && y1 >= 0 && y2<MAX_MAP_Y){
        if(y_v > 0){
            if(gMap.tile[y2][x1] != BLANK_TILE || gMap.tile[y2][x2] != BLANK_TILE){
                y_pos = y2*TILE_SIZE ;
                y_pos -= (height_frame+1);
                y_v = 0 ;
                onGround = true ;

            }
        }
        else if(y_v < 0){
            if(gMap.tile[y1][x1] != BLANK_TILE || gMap.tile[y1][x2] != BLANK_TILE){
                y_pos =(y1+1)*TILE_SIZE;
                y_v = 0 ;
            }
        }
    }
    x_pos += x_v;
    y_pos += y_v;
    if(x_pos < 0){
        x_pos = 0;
    }
    else if(x_pos +width_frame >gMap.maxX){
        x_pos = gMap.maxX - width_frame;
    }
    if(y_pos > gMap.maxY){
        comeBack = 60 ;
    }
}
