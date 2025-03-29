#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <windows.h>
#include <SDL.h>
#include "CommonFunc.h"
#include "BaseObject.h"
#include "gameMap.h"
#include "mainObject.h"
#include "impTimer.h"
#include "threatObject.h"
#include "explosionObject.h"
#include "textObject.h"
#include "playPower.h"
#include "geoMetric.h"
using namespace std;

BaseObject gBackground; // Đối tượng nền của game
TTF_Font* font_time = NULL; // Font cho thời gian và điểm số
TTF_Font* font_game_over = NULL; // Font lớn hơn cho Game Over, menu và pause

Mix_Chunk* sound_beep = NULL;
Mix_Chunk* sound_explosion = NULL;
Mix_Chunk* sound_laser = NULL;

Mix_Music* music_backsound = NULL;
Mix_Music* music_action = NULL;
// Định nghĩa các trạng thái game
enum GameState {
    MENU,      // Trạng thái hiển thị menu
    PLAYING,   // Trạng thái chơi game
    PAUSED,    // Trạng thái tạm dừng game
    GAME_OVER,  // Trạng thái khi game kết thúc
    WIN
};


bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) { // Khởi tạo SDL, nếu lỗi thì trả về false
        return false;
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"); // Cải thiện chất lượng render
    gWindow = SDL_CreateWindow("Strike game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN); // Tạo cửa sổ game
    if (gWindow == NULL) { // Nếu tạo cửa sổ thất bại
        cout << SDL_GetError() << endl; // In lỗi
        return false;
    }
    else {
        gRender = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED); // Tạo renderer với chế độ tăng tốc phần cứng
        if (gRender == NULL) { // Nếu tạo renderer thất bại
            cout << SDL_GetError() << endl; // In lỗi
            return false;
        }
        else {
            SDL_SetRenderDrawColor(gRender, 0xff, 0xff, 0xff, 0xff); // Đặt màu nền trắng cho renderer
            int imFlags = IMG_INIT_PNG; // Cờ để khởi tạo SDL_image với định dạng PNG
            if (!IMG_Init(imFlags) && imFlags) { // Khởi tạo SDL_image, nếu lỗi thì in thông báo
                cout << IMG_GetError() << endl;
                return false;
            }
            if (TTF_Init() == -1) { // Khởi tạo SDL_ttf cho văn bản, nếu lỗi thì trả về false
                return false;
            }
            font_time = TTF_OpenFont("font/dlxfont_.ttf", 15); // Load font nhỏ cho thời gian/điểm
            if (font_time == NULL) { // Nếu load font thất bại
                return false;
            }
            font_game_over = TTF_OpenFont("font/dlxfont_.ttf", 40); // Load font lớn cho Game Over/menu/pause
            if (font_game_over == NULL) { // Nếu load font thất bại
                return false;
            }
        }
    }
    return true; // Khởi tạo thành công
}

// Hàm load hình nền
bool loadBackground() {
    bool f = gBackground.loadIMG("image/background.png", gRender); // Load hình nền từ file
    if (!f) { // Nếu load thất bại
        return false;
    }
    return true; // Load thành công
}

// Hàm tạo danh sách kẻ thù
vector<threatObject*> makeThreatList() {
    vector<threatObject*> list_threats; // Vector chứa danh sách kẻ thù
    threatObject* dynamic_threat = new threatObject[30]; // Tạo mảng 20 kẻ thù động
    for (int i = 0; i < 30; i++) { // Vòng lặp để khởi tạo từng kẻ thù
        threatObject* p_threat = (dynamic_threat + i); // Lấy con trỏ đến kẻ thù thứ i
        if (p_threat != NULL) { // Kiểm tra con trỏ hợp lệ
            p_threat->loadIMG("image/threat_left.png", gRender); // Load hình ảnh kẻ thù
            p_threat->setClips(); // Cài đặt các frame animation
            p_threat->set_type_move(threatObject::MOVE_IN_SPACE_THREAT); // Đặt kiểu di chuyển qua lại
            p_threat->set_x_pos(900 + i * 900); // Đặt vị trí x ban đầu
            p_threat->set_y_pos(100); // Đặt vị trí y ban đầu
            int pos1 = p_threat->get_x_pos() - 100; // Giới hạn trái của di chuyển
            int pos2 = p_threat->get_x_pos() + 100; // Giới hạn phải của di chuyển
            p_threat->setAnimationpos(pos1, pos2); // Cài đặt phạm vi di chuyển
            p_threat->set_input_left(1); // Bắt đầu di chuyển sang trái
            list_threats.push_back(p_threat); // Thêm vào danh sách
        }
    }
    return list_threats; // Trả về danh sách kẻ thù
}

// Hàm giải phóng tài nguyên
void close() {
    // Giải phóng âm thanh
    Mix_FreeChunk(sound_beep);
    Mix_FreeChunk(sound_explosion);
    Mix_FreeChunk(sound_laser);
    Mix_FreeMusic(music_backsound);
    Mix_FreeMusic(music_action);

    // Đóng SDL_mixer
    Mix_CloseAudio();
    SDL_DestroyRenderer(gRender); // Hủy renderer
    SDL_DestroyWindow(gWindow); // Hủy cửa sổ
    gRender = NULL; // Đặt lại con trỏ renderer
    gWindow = NULL; // Đặt lại con trỏ cửa sổ
    TTF_CloseFont(font_time); // Giải phóng font thời gian
    TTF_CloseFont(font_game_over); // Giải phóng font Game Over
    TTF_Quit(); // Thoát SDL_ttf
    SDL_Quit(); // Thoát SDL
    IMG_Quit(); // Thoát SDL_image
}

// Hàm khởi động lại game
void restartGame(mainObject& p_player, playPower& player_power, playMoney& player_money, vector<threatObject*>& threats_list, textObject& time_game, textObject& mark_game, textObject& money_game, UINT& mark_value, int& heart) {
    p_player.SetRect(0, 200); // Đặt lại vị trí ban đầu của nhân vật
    p_player.setComebacktime(0); // Reset thời gian hồi sinh
    p_player.set_bullet_list({}); // Xóa danh sách đạn của nhân vật
    player_power.init(gRender); // Khởi tạo lại thanh mạng sống
    heart = 0; // Reset số lần mất mạng
    player_money.init(gRender); // Khởi tạo lại biểu tượng tiền
    mark_value = 0; // Reset điểm số
    p_player.increase_money(); // Reset số tiền
    for (int i = 0; i < threats_list.size(); i++) { // Giải phóng từng kẻ thù cũ
        threatObject* p_threat = threats_list.at(i);
        if (p_threat) {
            p_threat->free();
            p_threat = NULL;
        }
    }
    threats_list.clear(); // Xóa danh sách kẻ thù
    threats_list = makeThreatList(); // Tạo lại danh sách kẻ thù mới
    time_game.setText("Time: "); // Reset văn bản thời gian
    mark_game.setText("Mark: 0"); // Reset văn bản điểm số
    money_game.setText(": 0"); // Reset văn bản số tiền
}

int main(int argc, char* argv[]) {
    impTimer fps_time; // Bộ đếm thời gian để giới hạn FPS
    if (!init()) { // Gọi hàm khởi tạo, nếu thất bại thì thoát
        return -1;
    }
    if (!loadBackground()) { // Load nền, nếu thất bại thì thoát
        return -1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL_mixer init failed: " << Mix_GetError() << std::endl;
        return -1;
    }
    // Tải các hiệu ứng âm thanh (WAV)
    sound_beep = Mix_LoadWAV("sound/beep_.wav");
    sound_explosion = Mix_LoadWAV("sound/explosion.wav");
    sound_laser = Mix_LoadWAV("sound/laser.wav");

    // Tải nhạc nền (MIDI)
    music_backsound = Mix_LoadMUS("sound/backsound.mid");
    music_action = Mix_LoadMUS("sound/action.mid");

    // Kiểm tra xem các file có tải thành công không
    if (sound_beep == NULL || sound_explosion == NULL || sound_laser == NULL ||
        music_backsound == NULL || music_action == NULL) {
        std::cout << "Failed to load sound files: " << Mix_GetError() << std::endl;
        return -1;
    }
    // Trạng thái game ban đầu là menu
    GameState game_state = MENU;

    // Khởi tạo các văn bản cho menu
    textObject menu_title;
    menu_title.setColor(textObject::WHITE_TEXT); // Đặt màu trắng cho tiêu đề
    menu_title.setText("Strike Game"); // Đặt nội dung tiêu đề

    textObject menu_start;
    menu_start.setColor(textObject::WHITE_TEXT); // Đặt màu trắng cho mục "Start Game"
    menu_start.setText("Start Game"); // Nội dung mục "Start Game"

    textObject menu_exit;
    menu_exit.setColor(textObject::WHITE_TEXT); // Đặt màu trắng cho mục "Exit"
    menu_exit.setText("Exit"); // Nội dung mục "Exit"

    int selected_item = 0; // Biến theo dõi mục được chọn trong menu (0 = Start, 1 = Exit)

    // Văn bản cho trạng thái pause
    textObject pause_text;
    pause_text.setColor(textObject::WHITE_TEXT); // Đặt màu trắng cho văn bản "Paused"
    pause_text.setText("Paused - Press P to Resume"); // Nội dung văn bản khi tạm dừng

    textObject win_text;
    win_text.setColor(textObject::WHITE_TEXT); // Sử dụng màu xanh lá (nếu đã thêm từ gợi ý trước)
    win_text.setText("You Win!");

    textObject win_instruction_text;
    win_instruction_text.setColor(textObject::WHITE_TEXT);
    win_instruction_text.setText("Press R to Restart or ESC to Quit");

    // Khởi tạo các đối tượng game
    gameMap gMap; // Đối tượng bản đồ
    gMap.loadMap("map/map01.dat"); // Load dữ liệu bản đồ từ file
    gMap.loadTiles(gRender); // Load các tile của bản đồ

    mainObject p_player; // Đối tượng nhân vật chính
    p_player.loadIMG("image/player_right.png", gRender); // Load hình ảnh nhân vật
    p_player.set_clips(); // Cài đặt các frame animation cho nhân vật
    p_player.setSoundBeep(sound_beep); // ăn xu
    p_player.setSoundLaser(sound_laser);// bắn
    playPower player_power; // Thanh mạng sống
    player_power.init(gRender); // Khởi tạo thanh mạng sống

    playMoney player_money; // Biểu tượng tiền
    player_money.init(gRender); // Khởi tạo biểu tượng tiền

    vector<threatObject*> threats_list = makeThreatList(); // Tạo danh sách kẻ thù

    explosionObject exp_threat; // Đối tượng hiệu ứng nổ
    bool tRet = exp_threat.loadIMG("image/exp3.png", gRender); // Load hình ảnh hiệu ứng nổ
    exp_threat.setClip(); // Cài đặt các frame cho hiệu ứng nổ
    if (!tRet) return -1; // Nếu load thất bại thì thoát

    int heart = 0; // Số lần mất mạng

    textObject time_game; // Văn bản hiển thị thời gian
    time_game.setColor(textObject::WHITE_TEXT); // Đặt màu trắng

    textObject mark_game; // Văn bản hiển thị điểm số
    mark_game.setColor(textObject::WHITE_TEXT); // Đặt màu trắng
    UINT mark_value = 0; // Giá trị điểm số

    textObject money_game; // Văn bản hiển thị số tiền
    money_game.setColor(textObject::WHITE_TEXT); // Đặt màu trắng

    textObject game_over_text; // Văn bản "Game Over"
    game_over_text.setColor(textObject::RED_TEXT); // Đặt màu đỏ
    game_over_text.setText("Game Over"); // Nội dung "Game Over"

    textObject instruction_text; // Văn bản hướng dẫn khi Game Over
    instruction_text.setColor(textObject::WHITE_TEXT); // Đặt màu trắng
    instruction_text.setText("Press R to Restart or ESC to Quit"); // Nội dung hướng dẫn

    bool quit = false; // Biến kiểm soát vòng lặp chính

    while (!quit) { // Vòng lặp chính của game

        fps_time.start(); // Bắt đầu đếm thời gian cho frame
        while (SDL_PollEvent(&gEvent) != 0) { // Xử lý các sự kiện
            if (gEvent.type == SDL_QUIT) { // Nếu người dùng đóng cửa sổ
                quit = true;
            }

            if (game_state == MENU) { // Xử lý input trong trạng thái menu
                Mix_PlayMusic(music_backsound, -1);
                if (gEvent.type == SDL_KEYDOWN) { // Khi nhấn phím
                    switch (gEvent.key.keysym.sym) {
                    case SDLK_UP: // Phím mũi tên lên
                        selected_item = (selected_item - 1 + 2) % 2; // Chuyển lên mục trước (vòng quanh)
                        break;
                    case SDLK_DOWN: // Phím mũi tên xuống

                        selected_item = (selected_item + 1) % 2; // Chuyển xuống mục sau (vòng quanh)
                        break;
                    case SDLK_RETURN: // Phím Enter
                        if (selected_item == 0) { // Nếu chọn "Start Game"
                            Mix_HaltMusic();
                            game_state = PLAYING; // Chuyển sang trạng thái chơi game
                        } else if (selected_item == 1) { // Nếu chọn "Exit"
                            quit = true; // Thoát game
                        }
                        break;
                    }
                }
            } else if (game_state == PLAYING) { // Xử lý input trong trạng thái chơi game

                // Phát nhạc nền của game
                if (Mix_PlayingMusic() == 0) {
                Mix_PlayMusic(music_action, -1); // -1 để lặp vô hạn
    }
                if (gEvent.type == SDL_KEYDOWN && gEvent.key.keysym.sym == SDLK_p) { // Nhấn phím P để tạm dừng
                    game_state = PAUSED; // Chuyển sang trạng thái tạm dừng
                } else {
                    p_player.handelInputaction(gEvent, gRender); // Xử lý phím cho nhân vật
                }

            } else if (game_state == PAUSED) { // Xử lý input trong trạng thái tạm dừng
                if (gEvent.type == SDL_KEYDOWN && gEvent.key.keysym.sym == SDLK_p) { // Nhấn phím P để tiếp tục
                    game_state = PLAYING; // Quay lại trạng thái chơi
                }
            } else if (game_state == GAME_OVER) { // Xử lý input trong trạng thái Game Over
                Mix_HaltMusic();
                if (gEvent.type == SDL_KEYDOWN) { // Khi nhấn phím
                    switch (gEvent.key.keysym.sym) {
                    case SDLK_r: // Phím R
                        restartGame(p_player, player_power, player_money, threats_list, time_game, mark_game, money_game, mark_value, heart); // Khởi động lại game
                        game_state = PLAYING; // Quay lại trạng thái chơi
                        break;
                    case SDLK_ESCAPE: // Phím ESC
                        quit = true; // Thoát game
                        break;
                    }
                }
            }
            else if (game_state == WIN) {
                Mix_HaltMusic();
                if (gEvent.type == SDL_KEYDOWN) {
                    switch (gEvent.key.keysym.sym) {
                    case SDLK_r: // Phím R
                        restartGame(p_player, player_power, player_money, threats_list, time_game, mark_game, money_game, mark_value, heart);
                        game_state = PLAYING;
                        break;
                    case SDLK_ESCAPE: // Phím ESC
                        quit = true;
                        break;
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(gRender, 0xff, 0xff, 0xff, 0xff); // Đặt màu nền trắng cho renderer
        SDL_RenderClear(gRender); // Xóa màn hình trước khi vẽ mới

        if (game_state == MENU) { // Hiển thị menu

            gBackground.render(gRender, NULL); // Vẽ nền

            menu_title.loadFromRenderText(font_game_over, gRender); // Load văn bản tiêu đề
            menu_title.renderText(gRender, (SCREEN_WIDTH - menu_title.getWidth()) / 2, SCREEN_HEIGHT / 4); // Vẽ tiêu đề ở giữa trên

            menu_start.loadFromRenderText(font_time, gRender); // Load văn bản "Start Game"
            if (selected_item == 0) menu_start.setColor(textObject::RED_TEXT); // Đổi màu đỏ nếu được chọn
            else menu_start.setColor(textObject::WHITE_TEXT); // Màu trắng nếu không được chọn
            menu_start.renderText(gRender, (SCREEN_WIDTH - menu_start.getWidth()) / 2, SCREEN_HEIGHT / 2); // Vẽ "Start Game" ở giữa

            menu_exit.loadFromRenderText(font_time, gRender); // Load văn bản "Exit"
            if (selected_item == 1) menu_exit.setColor(textObject::RED_TEXT); // Đổi màu đỏ nếu được chọn
            else menu_exit.setColor(textObject::WHITE_TEXT); // Màu trắng nếu không được chọn
            menu_exit.renderText(gRender, (SCREEN_WIDTH - menu_exit.getWidth()) / 2, SCREEN_HEIGHT / 2 + 50); // Vẽ "Exit" dưới "Start Game"

        } else if (game_state == PLAYING) { // Chơi game bình thường
            gBackground.render(gRender, NULL); // Vẽ nền

            Map map_data = gMap.getMap(); // Lấy dữ liệu bản đồ

            p_player.handleBullet(gRender , map_data); // Xử lý và vẽ đạn của nhân vật
            p_player.setMapXY(map_data.startX, map_data.startY); // Cập nhật vị trí bản đồ cho nhân vật
            p_player.doPlayer(map_data); // Cập nhật trạng thái nhân vật (di chuyển, nhảy, v.v.)

            if (p_player.hasFallen()) { // Nếu nhân vật rơi xuống vực
                heart++; // Tăng số lần mất mạng
                if (heart < 3) { // Nếu chưa hết mạng
                    p_player.SetRect(0, 200); // Đặt lại vị trí nhân vật
                    p_player.setComebacktime(60); // Đặt thời gian hồi sinh
                    player_power.decrease(); // Giảm thanh mạng sống
                    player_power.render(gRender); // Vẽ lại thanh mạng sống
                } else {
                    game_state = GAME_OVER; // Chuyển sang trạng thái Game Over
                }
                p_player.resetFallen(); // Reset cờ rơi vực
            }
            else if (p_player.hasReachedFlag()) { // Kiểm tra nếu chạm vào cờ
                game_state = WIN;
                p_player.resetFlag();
            }

            p_player.show(gRender); // Hiển thị nhân vật

            gMap.setMap(map_data); // Cập nhật dữ liệu bản đồ
            gMap.drawMap(gRender); // Vẽ bản đồ

            // Vẽ thanh trạng thái trên cùng
            geoMetricFormat rectangle_size(0, 0, SCREEN_WIDTH, 40); // Kích thước thanh trạng thái
            colorData color_data(0, 0, 0); // Màu đen
            geoMetric::renderRectangle(rectangle_size, color_data, gRender); // Vẽ hình chữ nhật đen

            geoMetricFormat outline_size(1, 1, SCREEN_WIDTH - 1, 38); // Viền thanh trạng thái
            colorData color_data1(255, 255, 255); // Màu trắng
            geoMetric::renderOutline(outline_size, color_data1, gRender); // Vẽ viền trắng

            player_power.show(gRender); // Hiển thị thanh mạng sống
            player_money.show(gRender); // Hiển thị biểu tượng tiền
            player_money.setPos(SCREEN_WIDTH * 0.3 - 40, 8); // Đặt vị trí biểu tượng tiền

            // Xử lý và vẽ kẻ thù
            for (int i = 0; i < threats_list.size(); i++) {
                threatObject* p_threat = threats_list.at(i);
                if (p_threat != NULL) {
                    p_threat->setmapXY(map_data.startX, map_data.startY); // Cập nhật vị trí bản đồ cho kẻ thù
                    p_threat->imMovetype(gRender); // Cập nhật kiểu di chuyển
                    p_threat->doPlayer(map_data, gRender); // Cập nhật trạng thái kẻ thù
                    p_threat->makeBullet(gRender, SCREEN_WIDTH, SCREEN_HEIGHT,map_data); // Tạo và xử lý đạn của kẻ thù
                    p_threat->show(gRender); // Hiển thị kẻ thù

                    SDL_Rect rect_player = p_player.getRectframe(); // Lấy khung va chạm của nhân vật
                    bool bCol1 = false; // Cờ kiểm tra va chạm với đạn của kẻ thù
                    vector<bulletObject*> tBullet_list = p_threat->get_bullet_list(); // Lấy danh sách đạn của kẻ thù
                    for (int j = 0; j < tBullet_list.size(); j++) {
                        bulletObject* pt_bullet = tBullet_list.at(j);
                        if (pt_bullet) {
                            bCol1 = SDLCommonFunc::checkCollision(rect_player, pt_bullet->GetRect()); // Kiểm tra va chạm
                            if (bCol1) { // Nếu có va chạm
                                p_threat->removeBullet(j); // Xóa viên đạn
                                break;
                            }
                        }
                    }

                    SDL_Rect rect_threat = p_threat->getRectframe(); // Lấy khung va chạm của kẻ thù
                    bool bCol2 = SDLCommonFunc::checkCollision(rect_player, rect_threat); // Kiểm tra va chạm trực tiếp
                    if (bCol1 || bCol2) { // Nếu nhân vật va chạm với đạn hoặc kẻ thù
                        heart++; // Tăng số lần mất mạng
                        if (heart < 3) { // Nếu chưa hết mạng
                            p_player.SetRect(0, 200); // Đặt lại vị trí nhân vật
                            p_player.setComebacktime(60); // Đặt thời gian hồi sinh
                            player_power.decrease(); // Giảm thanh mạng sống
                            player_power.render(gRender); // Vẽ lại thanh mạng sống
                            continue;
                        } else {
                            game_state = GAME_OVER; // Chuyển sang trạng thái Game Over
                        }
                    }
                }
            }

            // Xử lý va chạm giữa đạn của nhân vật và kẻ thù
            int frame_exp_width = exp_threat.get_frame_width(); // Chiều rộng frame nổ
            int frame_exp_height = exp_threat.get_frame_height(); // Chiều cao frame nổ
            vector<bulletObject*> bullet_arr = p_player.get_bullet_list(); // Lấy danh sách đạn của nhân vật
            for (int j = 0; j < bullet_arr.size(); j++) {
                bulletObject* p_bullet = bullet_arr.at(j);
                if (p_bullet != NULL) {
                    if (p_bullet->get_is_move()) { // Nếu đạn còn di chuyển
                        if (p_bullet->checkCollisionWithMap(map_data)) { // Kiểm tra va chạm với bản đồ
                            p_bullet->set_is_move(false); // Dừng đạn nếu va chạm
                        }
                    }
                    for (int k = 0; k < threats_list.size(); k++) {
                        threatObject* obj_threat = threats_list.at(k);
                        if (obj_threat != NULL) {
                            SDL_Rect tRect; // Khung va chạm của kẻ thù
                            tRect.x = obj_threat->GetRect().x;
                            tRect.y = obj_threat->GetRect().y;
                            tRect.w = obj_threat->get_width_frame();
                            tRect.h = obj_threat->get_height_frame();

                            SDL_Rect bRect = p_bullet->GetRect(); // Khung va chạm của đạn
                            bool bCol = SDLCommonFunc::checkCollision(bRect, tRect); // Kiểm tra va chạm
                            if (bCol) { // Nếu đạn trúng kẻ thù
                                mark_value++; // Tăng điểm số
                                Mix_PlayChannel(-1, sound_explosion, 0);
                                for (int ex = 0; ex < NUM_FRAME_EXP; ex++) { // Hiển thị hiệu ứng nổ
                                    int x_pos = p_bullet->GetRect().x - frame_exp_width * 0.5;
                                    int y_pos = p_bullet->GetRect().y - frame_exp_height * 0.5;
                                    exp_threat.setFrame(ex);
                                    exp_threat.SetRect(x_pos, y_pos);
                                    exp_threat.show(gRender);
                                }
                                p_player.removeBullet(j); // Xóa đạn
                                obj_threat->free(); // Giải phóng kẻ thù
                                threats_list.erase(threats_list.begin() + k); // Xóa kẻ thù khỏi danh sách
                            }
                        }
                    }
                }
            }

            // Hiển thị thời gian
            string str_time = "Time: ";
            Uint32 time_val = SDL_GetTicks() / 1000; // Thời gian đã trôi qua (giây)
            Uint32 val_time = 300 - time_val; // Thời gian còn lại
            if (val_time <= 0) { // Nếu hết thời gian
                game_state = GAME_OVER; // Chuyển sang trạng thái Game Over
            } else {
                string str_val = to_string(val_time); // Chuyển thời gian thành chuỗi
                str_time += str_val;
                time_game.setText(str_time); // Cập nhật văn bản thời gian
                time_game.loadFromRenderText(font_time, gRender); // Load văn bản
                time_game.renderText(gRender, SCREEN_WIDTH - 200, 15); // Vẽ văn bản thời gian
            }

            // Hiển thị điểm số
            string val_str_mark = to_string(mark_value); // Chuyển điểm số thành chuỗi
            string strMark("Mark: ");
            strMark += val_str_mark;
            mark_game.setText(strMark); // Cập nhật văn bản điểm số
            mark_game.loadFromRenderText(font_time, gRender); // Load văn bản
            mark_game.renderText(gRender, SCREEN_WIDTH * 0.6 - 50, 15); // Vẽ văn bản điểm số

            // Hiển thị số tiền
            int moneyCount = p_player.getMoneycount(); // Lấy số tiền
            string money_str = to_string(moneyCount); // Chuyển số tiền thành chuỗi
            string moneyDisplay = ": ";
            moneyDisplay += money_str;
            money_game.setText(moneyDisplay); // Cập nhật văn bản số tiền
            money_game.loadFromRenderText(font_time, gRender); // Load văn bản
            money_game.renderText(gRender, SCREEN_WIDTH * 0.3, 15); // Vẽ văn bản số tiền

        } else if (game_state == PAUSED) { // Hiển thị trạng thái tạm dừng
            gBackground.render(gRender, NULL); // Vẽ nền
            gMap.drawMap(gRender); // Vẽ bản đồ (giữ nguyên trạng thái game)
            p_player.show(gRender); // Hiển thị nhân vật (đứng yên)
            for (int i = 0; i < threats_list.size(); i++) { // Hiển thị kẻ thù (đứng yên)
                threatObject* p_threat = threats_list.at(i);
                if (p_threat != NULL) {
                    p_threat->show(gRender);
                }
            }
            pause_text.loadFromRenderText(font_game_over, gRender); // Load văn bản "Paused"
            pause_text.renderText(gRender, (SCREEN_WIDTH - pause_text.getWidth()) / 2, (SCREEN_HEIGHT - pause_text.getHeight()) / 2); // Vẽ "Paused" ở giữa màn hình

        } else if (game_state == GAME_OVER) { // Hiển thị màn hình Game Over
            gBackground.render(gRender, NULL); // Vẽ nền
            game_over_text.loadFromRenderText(font_game_over, gRender); // Load văn bản "Game Over"
            int go_width = game_over_text.getWidth(); // Lấy chiều rộng văn bản
            int go_height = game_over_text.getHeight(); // Lấy chiều cao văn bản
            game_over_text.renderText(gRender, (SCREEN_WIDTH - go_width) / 2, (SCREEN_HEIGHT - go_height) / 2 - 50); // Vẽ "Game Over" ở giữa trên

            instruction_text.loadFromRenderText(font_game_over, gRender); // Load văn bản hướng dẫn
            int inst_width = instruction_text.getWidth(); // Lấy chiều rộng văn bản
            int inst_height = instruction_text.getHeight(); // Lấy chiều cao văn bản
            instruction_text.renderText(gRender, (SCREEN_WIDTH - inst_width) / 2, (SCREEN_HEIGHT - inst_height) / 2 + 50); // Vẽ hướng dẫn ở giữa dưới
        }
        else if (game_state == WIN) {
            gBackground.render(gRender, NULL);
            win_text.loadFromRenderText(font_game_over, gRender);
            int win_width = win_text.getWidth();
            int win_height = win_text.getHeight();
            win_text.renderText(gRender, (SCREEN_WIDTH - win_width) / 2, (SCREEN_HEIGHT - win_height) / 2 - 50);

            win_instruction_text.loadFromRenderText(font_game_over, gRender);
            int inst_width = win_instruction_text.getWidth();
            int inst_height = win_instruction_text.getHeight();
            win_instruction_text.renderText(gRender, (SCREEN_WIDTH - inst_width) / 2, (SCREEN_HEIGHT - inst_height) / 2 + 50);
        }

        SDL_RenderPresent(gRender); // Cập nhật màn hình

        // Giới hạn FPS
        int real_imp_time = fps_time.get_ticks(); // Thời gian thực hiện frame
        int time_one_frame = 1000 / FRAME_PER_SECOND; // Thời gian lý tưởng cho 1 frame
        if (real_imp_time < time_one_frame) { // Nếu frame chạy nhanh hơn FPS mong muốn
            int delay_time = time_one_frame - real_imp_time; // Tính thời gian cần delay
            if (delay_time >= 0) {
                SDL_Delay(delay_time); // Delay để giữ FPS ổn định
            }
        }
    }

    // Giải phóng tài nguyên trước khi thoát
    for (int i = 0; i < threats_list.size(); i++) {
        threatObject* p_threat = threats_list.at(i);
        if (p_threat) {
            p_threat->free(); // Giải phóng từng kẻ thù
            p_threat = NULL;
        }
    }
    threats_list.clear(); // Xóa danh sách kẻ thù
    close(); // Gọi hàm giải phóng tài nguyên
    return 0; // Thoát chương trình
}
