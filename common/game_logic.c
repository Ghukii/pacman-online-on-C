#include "game_logic.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <termios.h>
#include <ncurses.h>

// Массив для хранения игровой карты
char map[MAP_HEIGHT][MAP_WIDTH];

// Игроки
Player players[MAX_PLAYERS];

// Количество игроков
int num_players = 0;

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
    srand(time(NULL));

    // Инициализация части карты
    char** sub_map = (char**)malloc((MAP_HEIGHT / 2) * sizeof(char*));
    for (int i = 0; i < MAP_HEIGHT / 2; i++) {
        sub_map[i] = (char*)malloc((MAP_WIDTH / 2) * sizeof(char));
        for (int j = 0; j < MAP_WIDTH / 2; j++) {
            if (i == 0 || j == 0 || i == MAP_HEIGHT / 2 - 1 || j == MAP_WIDTH / 2 - 1) {
                sub_map[i][j] = WALL; // Установка стен на границах
            } else {
                sub_map[i][j] = (rand() % 3 >= 1) ? FOOD : WALL;
            }
        }
    }

    while(1){

        int start_x = rand() % 20;
        int start_y = rand() % 15;

        if (sub_map[start_y][start_x] == '.'){
            sub_map[start_y][start_x] = PLAYER;
            break;
        }
    }

    // Заполнение всей карты на основе sub_map
    for (int i = 0; i < MAP_HEIGHT / 2; i++) {
        for (int j = 0; j < MAP_WIDTH / 2; j++) {
            char value = sub_map[i][j];
            // Заполнение первой четверти
            map[i][j] = value;
            // Отражение по горизонтали во вторую четверть
            map[i][MAP_WIDTH - 1 - j] = value;
            // Отражение по вертикали в третью четверть
            map[MAP_HEIGHT - 1 - i][j] = value;
            // Отражение в четвертую четверть
            map[MAP_HEIGHT - 1 - i][MAP_WIDTH - 1 - j] = value;
        }
    }

    // Освобождение памяти sub_map
    for (int i = 0; i < MAP_HEIGHT / 2; i++) {
        free(sub_map[i]);
    }
    free(sub_map);
}

void print_map(char map[MAP_HEIGHT][MAP_WIDTH]){
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            putchar(map[i][j]); // Выводим символ на экран   
        }
        printf("\n");
    }
}

// Функция для отрисовки карты
void draw_map(char map[MAP_HEIGHT][MAP_WIDTH]) {
    system("clear"); // Очищаем экран

    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            char element = map[i][j];
            switch (element) {
                case WALL:
                    putchar(WALL); // Используем putchar для увеличения производительности
                    break;
                case FOOD:
                    putchar(FOOD);
                    break;
                case PLAYER:
                    putchar(PLAYER);
                    break;
                default:
                    putchar(' '); // Выводим пробел для пустых мест
                    break;
            }
        }
        putchar('\n'); // Переходим на новую строку после каждой строки карты
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
bool can_move(Player player, char map[MAP_HEIGHT][MAP_WIDTH]) {
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
            map[new_y][new_x] != WALL);
}

// Функция для перемещения игрока
void move_player(Player *player, char map[MAP_HEIGHT][MAP_WIDTH]) {
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
void update_game(Player *players, int num_players, char map[MAP_HEIGHT][MAP_WIDTH]) {
    // Очищаем терминал
    printf("\033[2J");

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

    // Отрисовываем обновленную карту
    draw_map(map);
}

// Функция для проверки условий завершения игры
bool is_game_over(char map[MAP_HEIGHT][MAP_WIDTH]) {
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
    // Устанавливаем количество игроков
    num_players = MAX_PLAYERS;

    // Генерируем и отображаем игровую карту
    generate_map();

    // Инициализация переменной для отслеживания числа найденных стартовых позиций
    int start_positions = 0;

    // Поиск стартовых позиций 'S' и размещение игроков
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (map[y][x] == "P" && start_positions < num_players) {
                players[start_positions].x = x;
                players[start_positions].y = y;
                players[start_positions].direction = "d"; // Начальное направление движения
                players[start_positions].score = 0; // Начальное количество очков
                start_positions++;
            }
        }
    }

    // Отрисовка карты после размещения игроков
    draw_map(map);
}


int main() {
    initialize_game();


    // Игровой цикл продолжается, пока игра не будет завершена
    while (!is_game_over(map)) {
        draw_map(map); // Отображаем карту
        update_game(players, num_players, map); // Обновляем игру: обработка ввода и перемещение игроков
        usleep(100000); // Задержка для контроля скорости игры (100 миллисекунд)
    }

    determine_winner(players, num_players); // Определяем победителя игры

    return 0; // Завершаем программу с кодом 0
}