#include "bulletObject.h"
#include "gameMap.h"
bulletObject::bulletObject(){
    x_v = 0 ;
    y_v = 0 ;
    is_move = false ;
    bullet_type = SPHERE_BULLET ;
    initial_x = 0;
    initial_y = 0;
};
bulletObject::~bulletObject(){
}
bool bulletObject::loadIMGBullet(SDL_Renderer* des){
    bool ret = false ;
    if(bullet_type == LASER_BULLET){
        ret = loadIMG("image/laser_bullet.png",des);
    }
    else{
        ret = loadIMG("image/boss_bullet.png",des);
    }
    return ret ;
}
void bulletObject::handleMove(const int& x_border, const int& y_border, const Map& map_data) {
    const int MAX_BULLET_DISTANCE = 300;

    if (bullet_dir == DIR_RIGHT) {
        rect.x += x_v;
        if (rect.x > x_border) {
            is_move = false;
        }
    } else if (bullet_dir == DIR_LEFT) {
        rect.x -= x_v;
        if (rect.x < 0) {
            is_move = false;
        }
    }

    int distance_x = abs(rect.x - initial_x);
    int distance_y = abs(rect.y - initial_y);
    int total_distance = sqrt(distance_x * distance_x + distance_y * distance_y);

    if (total_distance > MAX_BULLET_DISTANCE) {
        is_move = false;
    }

    if (checkCollisionWithMap(map_data)) {
        is_move = false;
    }
}
bool bulletObject::checkCollisionWithMap(const Map& map_data) {
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;

    // Tính toán vị trí của viên đạn trên lưới bản đồ (tương tự như mainObject::checkTomap)
    x1 = (rect.x) / TILE_SIZE;
    x2 = (rect.x + rect.w - 1) / TILE_SIZE;
    y1 = (rect.y) / TILE_SIZE;
    y2 = (rect.y + rect.h - 1) / TILE_SIZE;

    // Kiểm tra xem viên đạn có nằm trong giới hạn bản đồ không
    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
        // Kiểm tra các ô mà viên đạn đang chiếm
        for (int i = y1; i <= y2; i++) {
            for (int j = x1; j <= x2; j++) {
                int tile_value = map_data.tile[i][j];
                if (tile_value != BLANK_TILE && tile_value != STATE_MONEY) { // Nếu ô không phải là ô trống hoặc tiền
                    return true; // Có va chạm
                }
            }
        }
    }

    return false; // Không có va chạm
}
