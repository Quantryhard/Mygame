#include "mainObject.h"

const float GRAVITY = 0.8;
#define MAX_FALL_SPEED 10
#define PLAYER_SPEED 8
#define JUMP -17
mainObject::mainObject(){
    frame = 0 ;
    x_pos = 0 ;
    y_pos = 0 ;
    x_v = 0 ;
    y_v = 0 ;
    width_frame = 0;
    height_frame = 0 ;
    status = -1 ;
    input_type.left = 0;
    input_type.right = 0 ;
    input_type.up = 0 ;
    input_type.down = 0;
    input_type.isJumping = 0 ;
    onGround = false ;
    map_x_ = 0 ;
    map_y_ = 0 ;
    comeBack = 0 ;
}
mainObject::~mainObject(){
}
bool mainObject::loadIMG(char *path , SDL_Renderer* screen){
    bool ret = BaseObject::loadIMG(path,screen);
    if(ret == true){
        width_frame = rect.w/8 ;
        height_frame = rect.h ;
    }
    return ret ;
}
void mainObject::set_clips(){ // hàm cắt ảnh từ ảnh chính
    if(width_frame > 0 && height_frame > 0){
        for(int i = 0 ; i < 8 ; i++){
            frame_clip[i].x = i*width_frame;
            frame_clip[i].y = 0;
            frame_clip[i].w = width_frame;
            frame_clip[i].h = height_frame;
        }
    }
}// co the sua
void mainObject::show(SDL_Renderer* des){
    if(status == WALK_LEFT){
        loadIMG("image/player_left.png",des);
    }
    else{
        if(status == WALK_RIGHT){
            loadIMG("image/player_right.png",des);
        }
    }
    if(input_type.left == 1 || input_type.right ==1 ){
        frame = (frame+1)%8;
    }
    else{
        frame = 0 ;
    }
    rect.x = x_pos - map_x_;
    rect.y = y_pos - map_y_;
    SDL_Rect* current_clip = &frame_clip[frame] ;

    SDL_Rect renderQuad ={rect.x , rect.y , width_frame , height_frame};
    SDL_RenderCopy(des,pObject ,current_clip,&renderQuad);

}
void mainObject::handelInputaction(SDL_Event e,SDL_Renderer* screen){
    if(e.type == SDL_KEYDOWN){
        switch(e.key.keysym.sym){
        case SDLK_UP:
            status = WALK_UP ;
            input_type.up = 1 ;
            break;
        case SDLK_RIGHT:
            status = WALK_RIGHT ;
            input_type.right = 1;
            break ;
        case SDLK_LEFT:
            status = WALK_LEFT;
            input_type.left = 1 ;
            break;
        default:
            break;

        }
    }
    else if(e.type = SDL_KEYUP){
        switch(e.key.keysym.sym){
        case SDLK_RIGHT:
            input_type.right = -1;
            break;
        case SDLK_LEFT:
            input_type.left = -1;
            break;
        case SDLK_UP:
            input_type.up = -1 ;
            break;
        default:
            break;
        }
    }
}
void mainObject::doPlayer(Map& map_data){
    if(comeBack == 0){
        x_v = 0 ;
        y_v += GRAVITY ;
        if(y_v >= MAX_FALL_SPEED){ // gioi han roi tu do
            y_v = MAX_FALL_SPEED ;
        }
        if(input_type.left == 1){
            x_v -= PLAYER_SPEED;
        }
        if(input_type.right == 1){
                x_v +=PLAYER_SPEED;
            }
        if(input_type.up == 1){
            if(onGround){
                y_v += JUMP ;

            }
            onGround = false ;
            input_type.up = -1 ;
        }
        checkTomap(map_data);//ktra va cham
        centerEntityonmap(map_data); // tính toán di chuyển bản đồ
    }
    if(comeBack > 0) {
        comeBack--;
        if(comeBack == 0){
            y_v = 0;
            x_v = 0;
            y_pos = 0 ;
        }
    }
}
void mainObject::centerEntityonmap(Map& map_data){
    map_data.startX = x_pos - (SCREEN_WIDTH/2);
    if(map_data.startX < 0){
        map_data.startX = 0 ;
    }
    else if(map_data.startX + SCREEN_WIDTH >= map_data.maxX){
        map_data.startX = map_data.maxX - SCREEN_WIDTH ;
    }
    map_data.startY = y_pos - (SCREEN_HEIGHT/2);
    if(map_data.startY < 0){
        map_data.startY = 0;
    }
    else if(map_data.startY + SCREEN_HEIGHT >= map_data.maxY){
        map_data.startY = map_data.maxY - SCREEN_HEIGHT ;
    }
}
void mainObject::checkTomap(Map& map_data){
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
            if(map_data.tile[y1][x2] != BLANK_TILE || map_data.tile[y2][x2] != BLANK_TILE){
                x_pos = x2*TILE_SIZE-width_frame-1;
                x_v = 0;
                }
            }
        else if(x_v < 0){
            if(map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y2][x1] != BLANK_TILE){
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
            if(map_data.tile[y2][x1] != BLANK_TILE || map_data.tile[y2][x2] != BLANK_TILE){
                y_pos = y2*TILE_SIZE ;
                y_pos -= (height_frame+1);
                y_v = 0 ;
                onGround = true ;
            }
        }
        else if(y_v < 0){
            if(map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y1][x2] != BLANK_TILE){
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
    else if(x_pos +width_frame >map_data.maxX){
        x_pos = map_data.maxX - width_frame;
    }
    if(y_pos > map_data.maxY){
        comeBack = 60 ;
    }
}
