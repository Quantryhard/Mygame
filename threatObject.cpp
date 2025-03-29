
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

    animation_l = 0 ;
    animation_r = 0 ;
    input_type.left = 0 ;
    type_move = STATIC_THREAT;
}
threatObject::~threatObject(){
}
bool threatObject::loadIMG(char* path,SDL_Renderer* screen){
    bool ret = BaseObject::loadIMG(path,screen);
    if(ret){
        width_frame = rect.w/THREAT_FRAME_NUM;
        height_frame = rect.h;
    }
    return ret;
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
SDL_Rect threatObject::getRectframe(){
    SDL_Rect rect_frame;
    rect_frame.x = rect.x;
    rect_frame.y = rect.y;
    rect_frame.w = width_frame ;
    rect_frame.h = height_frame ;
    return rect_frame ;
}
void threatObject::removeBullet(const int& indx){
    int size = bullet_list.size();
    if(size>0 && indx <size){
        bulletObject* p_bullet = bullet_list.at(indx);
        bullet_list.erase(bullet_list.begin()+indx);
        if(p_bullet){
            delete p_bullet;
            p_bullet = NULL;
        }
    }
}
void threatObject::doPlayer(Map& gMap , SDL_Renderer* gScreen){
    if(comeBack == 0){
        x_v = 0;
        y_v +=THREAT_GRAVITY_SPEED ;
        if(y_v >= THREAT_GRAVITY_SPEED){
            y_v = THREAT_GRAVITY_SPEED ;
        }
        if(input_type.left == 1){
            x_v -= THREAT_SPEED ;
        }
        else if(input_type.right == 1){
            x_v += THREAT_SPEED ;
        }
        checkTomap(gMap,gScreen);

    }
    else if(comeBack > 0){
        comeBack -- ;
        if(comeBack == 0){
            initThreat();
        }
    }
}
void threatObject::initThreat(){
    x_v = 0;
    y_v = 0;
    if(x_pos > 256){
        x_pos -= 256;
        animation_l -= 256;//
        animation_r -= 256;//
    }
    else{
        x_pos = 0;
    }
    y_pos = 0 ;
    comeBack = 0;
    input_type.left = 1 ;
}
void threatObject::checkTomap(Map& gMap , SDL_Renderer* gScreen) {
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;

    // Check theo chiều ngang
    int height_min = height_frame < TILE_SIZE ? height_frame : TILE_SIZE;
    x1 = (x_pos + x_v) / TILE_SIZE;
    x2 = (x_pos + x_v + width_frame - 1) / TILE_SIZE;
    y1 = (y_pos) / TILE_SIZE;
    y2 = (y_pos + height_min - 1) / TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
        if (x_v > 0) { // Khi quái vật di chuyển sang phải
            int val1 = gMap.tile[y1][x2];
            int val2 = gMap.tile[y2][x2];
            if (((val1 != BLANK_TILE) && (val1 != STATE_MONEY)) || ((val2 != BLANK_TILE) && (val2 != STATE_MONEY))) {
                x_pos = x2 * TILE_SIZE - width_frame - 1;
                x_v = 0;
                // Quay đầu: từ phải sang trái
                input_type.left = 1;
                input_type.right = 0;
                loadIMG("image/threat_left.png", gScreen); // Cập nhật hình ảnh
            }
        }
        else if (x_v < 0) { // Khi quái vật di chuyển sang trái
            int val1 = gMap.tile[y1][x1];
            int val2 = gMap.tile[y2][x1];
            if (((val1 != BLANK_TILE) && (val1 != STATE_MONEY)) || ((val2 != BLANK_TILE) && (val2 != STATE_MONEY))) {
                x_pos = (x1 + 1) * TILE_SIZE;
                x_v = 0;
                // Quay đầu: từ trái sang phải
                input_type.left = 0;
                input_type.right = 1;
                loadIMG("image/threat_right.png", gScreen); // Cập nhật hình ảnh
            }
        }
    }

    // Check theo chiều dọc
    int width_min = width_frame < TILE_SIZE ? width_frame : TILE_SIZE;
    x1 = (x_pos) / TILE_SIZE;
    x2 = (x_pos + width_min) / TILE_SIZE;
    y1 = (y_pos + y_v) / TILE_SIZE;
    y2 = (y_pos + y_v + height_frame - 1) / TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
        if (y_v > 0) {
            int val1 = gMap.tile[y2][x1];
            int val2 = gMap.tile[y2][x2];
            if (((val1 != BLANK_TILE) && (val1 != STATE_MONEY)) || ((val2 != BLANK_TILE) && (val2 != STATE_MONEY))) {
                y_pos = y2 * TILE_SIZE;
                y_pos -= (height_frame + 1);
                y_v = 0;
                onGround = true;
            }
        }
        else if (y_v < 0) {
            int val1 = gMap.tile[y1][x1];
            int val2 = gMap.tile[y1][x2];
            if (((val1 != BLANK_TILE) && (val1 != STATE_MONEY)) || ((val2 != BLANK_TILE) && (val2 != STATE_MONEY))) {
                y_pos = (y1 + 1) * TILE_SIZE;
                y_v = 0;
            }
        }
    }

    x_pos += x_v;
    y_pos += y_v;

    if (x_pos < 0) {
        x_pos = 0;
    }
    else if (x_pos + width_frame > gMap.maxX) {
        x_pos = gMap.maxX - width_frame;
    }

    if (y_pos > gMap.maxY) {
        comeBack = COMEBACK_PLAYER;
    }
}
void threatObject::imMovetype(SDL_Renderer* gScreen) {
    if (type_move == STATIC_THREAT) {
        return; // Không làm gì cả cho đối tượng tĩnh
    }

    if (onGround == true) {
        if(x_pos > animation_r){
            input_type.left = 1 ;
            input_type.right = 0 ;
            loadIMG("image/threat_left.png",gScreen);
        }
        else if(x_pos < animation_l){
            input_type.left = 0;
            input_type.right = 1;
            loadIMG("image/threat_right.png",gScreen);
        }
    }
    else{
        if(input_type.left == 1){
            loadIMG("image/threat_left.png",gScreen);
        }
    }
}
void threatObject::initBullet(bulletObject* p_bullet, SDL_Renderer* gScreen) {
    if (p_bullet != NULL) {
        p_bullet->set_bullet_type(bulletObject::LASER_BULLET);
        bool ret = p_bullet->loadIMGBullet(gScreen);
        if (ret) {
            p_bullet->set_is_move(true);

            // Đặt hướng đạn dựa trên hướng di chuyển của bot
            if (input_type.left == 1) {
                p_bullet->set_bullet_dir(bulletObject::DIR_LEFT);
                p_bullet->SetRect(rect.x - 10, rect.y + height_frame * 0.5); // Bắn từ bên trái bot
            } else if (input_type.right == 1) {
                p_bullet->set_bullet_dir(bulletObject::DIR_RIGHT);
                p_bullet->SetRect(rect.x + width_frame + 10, rect.y + height_frame * 0.5); // Bắn từ bên phải bot
            }

            p_bullet->set_x_val(15); // Tốc độ đạn
            p_bullet->set_initial_pos(p_bullet->GetRect().x, p_bullet->GetRect().y); // Lưu vị trí ban đầu
            bullet_list.push_back(p_bullet);
        }
    }
}
void threatObject::makeBullet(SDL_Renderer* gScreen, const int& x_limit, const int& y_limit, const Map& map_data) {
    static int shoot_timer = 0;
    shoot_timer++;
    if (shoot_timer >= 60 && comeBack == 0) {
        bulletObject* p_bullet = new bulletObject();
        initBullet(p_bullet, gScreen);
        shoot_timer = 0;
    }

    for (int i = 0; i < bullet_list.size(); i++) {
        bulletObject* p_bullet = bullet_list.at(i);
        if (p_bullet != NULL) {
            if (p_bullet->get_is_move()) {
                p_bullet->handleMove(x_limit, y_limit, map_data);
                p_bullet->render(gScreen);
            } else {
                removeBullet(i);
                i--;
            }
        }
    }
}
