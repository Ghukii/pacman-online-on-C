#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <stdbool.h>

// Размеры игрового поля
#define MAP_WIDTH 40
#define MAP_HEIGHT 30

// Определение клеток поля
#define WALL '#'
#define FOOD '.'
#define BORDER '|'
#define PLAYER 'P'

// Структура для хранения игрока
typedef struct {
    int x, y;
    char direction;
    int score;
} Player;

// Инициализация игры
void initialize_game();

// Генерация игровой карты
void generate_map();

// Отображение карты
void display_map();

// Функция для обработки пользовательского ввода и изменения направления игрока
void handle_input(Player *player);

// Функция для проверки возможности перемещения игрока
bool can_move(Player player, char map[MAP_HEIGHT][MAP_WIDTH]);

// Функция для перемещения игрока
void move_player(Player *player, char map[MAP_HEIGHT][MAP_WIDTH]);

// Функция для обновления состояния игры
void update_game(Player *players, int num_players, char map[MAP_HEIGHT][MAP_WIDTH]);

// Функция для проверки условий завершения игры
bool is_game_over(char map[MAP_HEIGHT][MAP_WIDTH]);

// Функция для определения победителя или объявления ничьи
void determine_winner(Player *players, int num_players);



#endif /* GAME_LOGIC_H */
