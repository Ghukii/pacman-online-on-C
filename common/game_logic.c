#include "game_logic.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Массив для хранения игровой карты
char map[MAP_HEIGHT + 4][MAP_WIDTH + 4];

// Игроки
//Player players[MAX_PLAYERS];

// Количество игроков
int num_players = 0;

// Инициализация игры
void initialize_game() {
    // Генерация игровой карты
    generate_map();
}

int random_number(){

    int randnum = rand() % 100;

    if (randnum <= 30){
        return 0;
    }
    else{
        return 1;
    }
}

// Генерация игровой карты
void generate_map() {
    // Здесь будет реализация алгоритма генерации карты

    srand(time(NULL));

    char** sub_map = (char**)malloc((MAP_HEIGHT / 2 + 2) * sizeof(char*));

    for(int i = 0; i < MAP_HEIGHT / 2 + 2; i++){

        sub_map[i] = (char*)malloc((MAP_WIDTH / 2 + 2) * sizeof(char));

        if (i == 0 || i == MAP_HEIGHT / 2 + 1){
            for(int j = 0; j < MAP_WIDTH / 2 + 2; j++){
                sub_map[i][j] = BORDER;
            }
        }
        else{
            sub_map[i][0] = BORDER;
            for(int j = 1; j < MAP_WIDTH / 2 + 1; j++){
                
                if (rand() % 3 >= 1){
                    sub_map[i][j] = FOOD;
                }
                else{
                    sub_map[i][j] = WALL;
                }
            }
            sub_map[i][MAP_WIDTH / 2 + 1] = BORDER;
        }
    }

    while(1){

        int start_x = rand() % 20;
        int start_y = rand() % 15;

        if (sub_map[start_y][start_x] == '.'){
            sub_map[start_y][start_x] = 'S';
            break;
        }
    }

    for (int i = 0; i < MAP_HEIGHT / 2 + 2; i++) {
        for (int j = 0; j < MAP_WIDTH / 2 + 2; j++) {
            // Отражение по вертикали (по горизонтали)
            map[i][j] = sub_map[i][j];
            map[i][MAP_WIDTH - j + 2] = sub_map[i][j];

            // Отражение по горизонтали (по вертикали)
            map[MAP_HEIGHT - i + 2][j] = sub_map[i][j];
            map[MAP_HEIGHT - i + 2][MAP_WIDTH - j + 2] = sub_map[i][j];
        }
    }
    // Нужно написать проверку доступности полей с едой!!!
}

// Отображение карты
void display_map() {
    // Здесь будет реализация вывода карты на экран
    for (int i = 0; i < MAP_HEIGHT + 4; i++) {
        for (int j = 0; j < MAP_WIDTH + 4; j++) {
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
}

// Проверка возможности движения в заданном направлении
bool can_move(Player *player, char direction) {
    // Здесь будет реализация проверки возможности движения
}

// Обработка движения игрока
void move_player(Player *player) {
    // Здесь будет реализация движения игрока
}

// Обновление состояния игры
void update_game() {
    // Здесь будет реализация обновления состояния игры
}

int main(){
    generate_map();
    display_map();
}