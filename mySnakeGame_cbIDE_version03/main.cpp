#include <iostream>
#include <conio.h>
#include <cstdint>
#include "myLib.h"

// Global variable
int gdot_number = 0;

// Declare of drawing wall
#define WALL_MIN_Y 1
#define WALL_MAX_Y 26
#define WALL_MIN_X 1
#define WALL_MAX_X 100
// ***
void drawWallAbove();
void drawWallBelow();
void drawWallLeftHand();
void drawWallRightHand();
void drawWall();

// Declare of snake
#define SNAKE_INIT_LENGTH 5
#define SNAKE_X0 50
#define SNAKE_Y0 13
//***
void initSnake(int array_x[], int array_y[]);
void drawSnake(int array_x[], int array_y[], uint8_t snake_color);
void backSpaceSnake(int array_x[], int array_y[]);
void processMovingAndEatingSnake(int array_x[], int array_y[], int x, int y, int xF, int yF);
bool isGameOver(int array_x[], int array_y[]);

// Declare fruit
void drawFruit(int x, int y, uint8_t fruit_color);
void createFruit(int &x, int &y, int array_x[], int array_y[]);
bool isCaseFruitCreateInsideSnake(int xF, int yF, int array_x[], int array_y[]);

// Declare color
#define COLOR_DEFAULT_WHITE 7

using namespace std;

int main()
{
    srand(time(NULL));
    int array_x[100], array_y[100];
    int x, y;
    bool is_game_over = false;
    uint8_t direc_moving = 3;
    // 0: go to the above
    // 1: go to the below
    // 2: go to the left
    // 3: go to the right
    int x_fruit = 0, y_fruit = 0;
    int last_gdot_number = 0;
    uint8_t color = 1;
    uint8_t last_color = COLOR_DEFAULT_WHITE;

    // Drawing the wall
    drawWall();

    // Init snake
    initSnake(array_x, array_y);
    drawSnake(array_x, array_y, last_color);

    // Create fruit
    createFruit(x_fruit, y_fruit, array_x, array_y);
    drawFruit(x_fruit, y_fruit, 1);

    // Process moving snake
    x = array_x[0];
    y = array_y[0];
    while(!is_game_over) {
        switch(direc_moving) {
        case 0:
            y--;
            break;
        case 1:
            y++;
            break;
        case 2:
            x--;
            break;
        case 3:
            x++;
            break;
        }
        Sleep(200);
        backSpaceSnake(array_x, array_y);
        last_gdot_number = gdot_number;
        processMovingAndEatingSnake(array_x, array_y, x, y, x_fruit, y_fruit);
        drawSnake(array_x, array_y, last_color);

        // Check if snake eats the fruit => create new fruit
        if(last_gdot_number + 1 == gdot_number) {
            last_color = color;
            ++color;
            if(color > 15) {
                color = 1;
            }
            createFruit(x_fruit, y_fruit, array_x, array_y);
            drawFruit(x_fruit, y_fruit, color);
        }

        // Check game over status
        is_game_over = isGameOver(array_x, array_y);

        if(kbhit() == 0) {
            continue;
        }
        char input_1st_char = _getch();
        if(input_1st_char != -32) {
            continue;
        }
        char input_2nd_char = _getch();
        switch(input_2nd_char) {
        case 72:
                // command go to the above
                if(direc_moving != 1) {
                    direc_moving = 0;
                }
                break;
        case 80:
                // command go to the below
                if(direc_moving != 0) {
                    direc_moving = 1;
                }
                break;
        case 75:
                // command go to the left
                if(direc_moving != 3) {
                    direc_moving = 2;
                }
                break;
        case 77:
                // command go to the right
                if(direc_moving != 2) {
                    direc_moving = 3;
                }
                break;
        }
    }

    getch();
    return 0;
}

void drawWallAbove() {
    int coord_y = WALL_MIN_Y;
    for(int coord_x = WALL_MIN_X; coord_x <= WALL_MAX_X; coord_x++) {
        gotoXY(coord_x, coord_y);
        std::cout << "+";
    }
}

void drawWallBelow() {
    int coord_y = WALL_MAX_Y;
    for(int coord_x = WALL_MIN_X; coord_x <= WALL_MAX_X; coord_x++) {
        gotoXY(coord_x, coord_y);
        std::cout << "+";
    }
}

void drawWallLeftHand() {
    int coord_x = WALL_MIN_X;
    for(int coord_y = WALL_MIN_Y; coord_y <= WALL_MAX_Y; coord_y++) {
        gotoXY(coord_x, coord_y);
        cout << "+";
    }
}

void drawWallRightHand() {
    int coord_x = WALL_MAX_X;
    for(int coord_y = WALL_MIN_Y; coord_y <= WALL_MAX_Y; coord_y++) {
        gotoXY(coord_x, coord_y);
        cout << "+";
    }
}

void drawWall() {
    drawWallAbove();
    drawWallBelow();
    drawWallLeftHand();
    drawWallRightHand();
}

// Init snake
void initSnake(int array_x[], int array_y[]) {
    gdot_number = SNAKE_INIT_LENGTH;
    int co_x = SNAKE_X0, co_y = SNAKE_Y0;
    for(int i = 0; i < gdot_number; i++) {
        array_x[i] = co_x;
        co_x--;
        array_y[i] = co_y;
    }
}

// Draw the snake
void drawSnake(int array_x[], int array_y[], uint8_t snake_color) {
    SetColor(snake_color);
    for(int i = 0; i < gdot_number; i++) {
        gotoXY(array_x[i], array_y[i]);
        if(i == 0) {
            cout << "0";
        }else {
            cout << "o";
        }
    }
    // Reset color
    SetColor(7);
}

// Back space engineering for draw snake
void backSpaceSnake(int array_x[], int array_y[]) {
    for(int i = 0; i < gdot_number; i++) {
        gotoXY(array_x[i], array_y[i]);
        cout << " ";
    }
}

// Process snake's moving
void processMovingAndEatingSnake(int array_x[], int array_y[], int x, int y, int xF, int yF) {
    // add a new element to the array
    for(int i = gdot_number - 1; i >= 0; i--) {
        array_x[i + 1] = array_x[i];
        array_y[i + 1] = array_y[i];
    }
    array_x[0] = x;
    array_y[0] = y;
    // clear the last element => done due to gdot_number do not change
    array_x[gdot_number] = 0;
    array_y[gdot_number] = 0;
    // Snake eats fruit
    if(xF == array_x[0] && yF == array_y[0]) {
        gdot_number++;
    }
}

// Check game over status
bool isGameOver(int array_x[], int array_y[]) {
    int x0 = array_x[0];
    int y0 = array_y[0];
    bool isHitTheWall = false;
    bool isHitTheTail = false;
    // the snake head hit the limit
    if((y0 == WALL_MIN_Y && (x0 >= WALL_MIN_X && x0 <= WALL_MAX_X)) ||
        (y0 == WALL_MAX_Y && (x0 >= WALL_MIN_X && x0 <= WALL_MAX_X)) ||
        (x0 == WALL_MIN_X && (y0 >= WALL_MIN_Y && y0 <= WALL_MAX_Y)) ||
        (x0 == WALL_MAX_X && (y0 >= WALL_MIN_Y && y0 <= WALL_MAX_Y))
        )
    {
        isHitTheWall = true;
    }

    // The snake eats the tail
    for(int i = 1; i < gdot_number; i++) {
        if(x0 == array_x[i] && y0 == array_y[i]) {
            isHitTheTail = true;
        }
    }
    return (isHitTheWall || isHitTheTail);
}

 // Draw the fruit
 void drawFruit(int x, int y, uint8_t fruit_color) {
    SetColor(fruit_color);
    gotoXY(x, y);
    cout << "$";
    // Reset color
    SetColor(7);
 }

 bool isCaseFruitCreateInsideSnake(int xF, int yF, int array_x[], int array_y[]) {
    for(int i = 0; i < gdot_number; i++) {
        if(xF == array_x[i] && yF == array_y[i]) {
            return true;
        }
    }
    return false;
 }

// Create a fruit
void createFruit(int &x, int &y, int array_x[], int array_y[]) {
    do
    {
        int xa = WALL_MIN_X + 1;
        int xb = WALL_MAX_X - 1;
        int ya = WALL_MIN_Y + 1;
        int yb = WALL_MAX_Y - 1;
        x = rand() % (xb - xa + 1) + xa;
        y = rand() % (yb - ya + 1) + ya;
    }while(isCaseFruitCreateInsideSnake(x, y, array_x, array_y));
}
