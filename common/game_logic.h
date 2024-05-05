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

// Структура для представления позиции на поле
typedef struct {
    int x;
    int y;
} Position;

// Структура для представления игрока
typedef struct {
    int id;
    Position position;
    char direction;
    int score;
} Player;

// Инициализация игры
void initialize_game();

// Генерация игровой карты
void generate_map();

// Отображение карты
void display_map();

// Проверка возможности движения в заданном направлении
bool can_move(Player *player, char direction);

// Обработка движения игрока
void move_player(Player *player);

// Обновление состояния игры
void update_game();

#endif /* GAME_LOGIC_H */
