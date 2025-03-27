#include "gameMap.h"
#include <stdio.h>
void gameMap::loadMap(char* path){
    std::ifstream file(path); // Mở tệp bằng ifstream
    if (!file.is_open()) {    // Kiểm tra xem tệp có mở thành công không
        //std::cout << "Failed to open map file: " << path << std::endl;

    }

    game_map.maxX = 0;
    game_map.maxY = 0;

    // Đọc dữ liệu từ tệp
    for (int i = 0; i < MAX_MAP_Y; i++) {
        for (int j = 0; j < MAX_MAP_X; j++) {
            if (file >> game_map.tile[i][j]) { // Đọc từng số nguyên từ tệp
                int val = game_map.tile[i][j];
                if (val > 0) {
                    if (j > game_map.maxX) {
                        game_map.maxX = j;
                    }
                    if (i > game_map.maxY) {
                        game_map.maxY = i;
                    }
                }
            }
            else {
                //std::cout << "Error reading map data at position (" << i << ", " << j << ")" << std::endl;
                file.close();

            }
        }
    }

    // Tính toán kích thước bản đồ (theo pixel)
    game_map.maxX = (game_map.maxX + 1) * TILE_SIZE;
    game_map.maxY = (game_map.maxY + 1) * TILE_SIZE;

    game_map.startX = 0;
    game_map.startY = 0;

    game_map.file_name = path;
    file.close(); // Đóng tệp
}
void gameMap::loadTiles(SDL_Renderer* screen){
    char file_img[30];
    FILE* fp = NULL ;
    for(int i = 0 ; i < MAX_TILE ; i++){
        sprintf_s(file_img,"map/%d.png",i);
        fopen_s(&fp,file_img,"rb");
        if(fp == NULL){
            continue;
        }
        fclose(fp);
        tile_mat[i].loadIMG(file_img,screen);
    }
}
void gameMap::drawMap(SDL_Renderer* screen){
    int x1 = 0;
    int x2 = 0 ;
    int y1 = 0 ;
    int y2 = 0 ;
    int map_x = game_map.startX/TILE_SIZE;
    int map_y = game_map.startY/TILE_SIZE;

    x1 = (game_map.startX%TILE_SIZE)*-1;
    x2 = x1 + SCREEN_WIDTH + (x1 == 0?0: TILE_SIZE);

    y1 = (game_map.startY%TILE_SIZE)*-1;
    y2 = y1 +SCREEN_HEIGHT + (y1 == 0?0:TILE_SIZE);

    for(int i = y1 ; i < y2 ;i+=TILE_SIZE){
        map_x = game_map.startX/TILE_SIZE;
        for(int j = x1 ; j < x2 ; j+= TILE_SIZE){
            int val = game_map.tile[map_y][map_x];
            if(val>0){
                tile_mat[val].SetRect(j,i);
                tile_mat[val].render(screen);
            }
            map_x++;
        }
        map_y++;
    }
}
