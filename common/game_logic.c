#include "game_logic.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <termios.h>

// Массив для хранения игровой карты
char map[MAP_HEIGHT + 4][MAP_WIDTH + 4];

// Игроки
Player players[MAX_PLAYERS];

// Количество игроков
int num_players = 0;dawsd

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

// Функция для установки терминальных настроек для немедленного считывания клавиш
void enable_raw_mode() {
    struct termios term;
    if (tcgetattr(STDIN_FILENO, &term) == -1) {
        perror("tcgetattr");
        exit(1);
    }
    term.c_lflag &= ~(ICANON | ECHO); // Отключаем канонический режим и эхо
    if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1) {
        perror("tcsetattr");
        exit(1);
    }
}

// Функция для восстановления обычных терминальных настроек
void disable_raw_mode() {
    struct termios term;
    if (tcgetattr(STDIN_FILENO, &term) == -1) {
        perror("tcgetattr");
        exit(1);
    }
    term.c_lflag |= (ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1) {
        perror("tcsetattr");
        exit(1);
    }
}

// Функция для получения символа с клавиатуры
char get_key_press() {
    char ch;
    enable_raw_mode();
    if (read(STDIN_FILENO, &ch, 1) != 1) {
        perror("read");
        exit(1);
    }
    disable_raw_mode();
    return ch;
}

// Функция для обработки пользовательского ввода и изменения направления игрока
void handle_input(Player *player) {
    char input = get_key_press();

    // Проверяем валидность ввода и изменяем направление игрока
    switch (input) {
        case 'w':
        case 'a':
        case 's':
        case 'd':
            player->direction = input;
            break;
        default:
            printf("Неверный ввод. Попробуйте еще раз.\n");
            break;
    }
}

// Функция для проверки возможности перемещения игрока
bool can_move(Player player, char map[MAP_HEIGHT + 4][MAP_WIDTH + 4]) {
    int dx = 0, dy = 0;

    // Определяем изменение координат в соответствии с направлением
    switch (player.direction) {
        case 'w':
            dy = -1;
            break;
        case 'a':
            dx = -1;
            break;
        case 's':
            dy = 1;
            break;
        case 'd':
            dx = 1;
            break;
    }

    // Проверяем возможность перемещения
    int new_x = player.x + dx;
    int new_y = player.y + dy;
    return (new_x >= 0 && new_x < MAP_WIDTH && new_y >= 0 && new_y < MAP_HEIGHT &&
            map[new_y][new_x] != WALL && map[new_y][new_x] != BORDER);
}

// Функция для перемещения игрока
void move_player(Player *player, char map[MAP_HEIGHT + 4][MAP_WIDTH + 4]) {
    if (can_move(*player, map)) {
        // Если перемещение возможно, обновляем координаты игрока
        switch (player->direction) {
            case 'w':
                player->y -= 1;
                break;
            case 'a':
                player->x -= 1;
                break;
            case 's':
                player->y += 1;
                break;
            case 'd':
                player->x += 1;
                break;
        }

        
    } else {
        printf("Невозможно переместиться в указанном направлении. Препятствие!\n");
    }
}

// Функция для обновления состояния игры
void update_game(Player *players, int num_players, char map[MAP_HEIGHT + 4][MAP_WIDTH + 4]) {
    // Проверяем условия завершения игры
    if (is_game_over(map)) {
        // Определяем победителя или объявляем ничью
        determine_winner(players, num_players);
        exit(0);
    }

    // Обновляем состояние каждого игрока
    for (int i = 0; i < num_players; i++) {
        handle_input(&players[i]); // Обработка пользовательского ввода
        move_player(&players[i], map); // Перемещение игрока

        // Проверяем, попал ли игрок на клетку с едой
        if (map[players[i].y][players[i].x] == FOOD) {
            players[i].score++; // Увеличиваем счет игрока
            map[players[i].y][players[i].x] = ' '; // Убираем еду с карты
        }
    }
}

// Функция для проверки условий завершения игры
bool is_game_over(char map[MAP_HEIGHT + 4][MAP_WIDTH + 4]) {
    // Проверяем, остались ли клетки с едой на карте
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (map[y][x] == FOOD) {
                return false; // Игра продолжается
            }
        }
    }
    return true; // Все клетки с едой были съедены, игра завершена
}

// Функция для определения победителя или объявления ничьи
void determine_winner(Player *players, int num_players) {
    // Ищем игрока с максимальным количеством очков
    int max_score = players[0].score;
    int winner_index = 0;
    bool draw = false; // Флаг, указывающий на ничью

    for (int i = 1; i < num_players; i++) {
        if (players[i].score > max_score) {
            max_score = players[i].score;
            winner_index = i;
            draw = false; // Сбрасываем флаг ничьей, если найден игрок с более высоким счетом
        } else if (players[i].score == max_score) {
            draw = true; // Объявляем ничью, если есть игрок(и) с равным количеством очков
        }
    }

    // Выводим результат игры
    if (draw) {
        printf("Ничья! Все игроки имеют равное количество очков.\n");
    } else {
        printf("Победил игрок %d с количеством очков: %d\n", winner_index + 1, max_score);
    }
}

void initialize_game() {
    // Создаем игрока
    players[0].x = 0; // Установка начальной позиции игрока по X
    players[0].y = 0; // Установка начальной позиции игрока по Y
    players[0].direction = 'd'; // Устанавливаем начальное направление движения игрока
    players[0].score = 0; // Устанавливаем начальное количество очков игрока

    // Устанавливаем количество игроков в 1
    num_players = 1;

    // Генерируем и отображаем игровую карту
    generate_map();
    display_map();
}

int main(){
    initialize_game();

    while (!is_game_over(map)) {
        display_map();
        update_game(&players[0], num_players, map);
        usleep(100000); // Пауза на 100 мс, чтобы замедлить игру
    }

    determine_winner(&players[0], num_players);

    return 0;
}