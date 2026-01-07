#include "raylib.h"
#include <stdlib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define CELL_SIZE 20
#define MAX_SNAKE_LENGTH 1000

typedef struct {
    Vector2 position;
} SnakeSegment;

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake Game - Raylib");
    SetTargetFPS(10);

    // Snake
    SnakeSegment snake[MAX_SNAKE_LENGTH];
    int snakeLength = 3;

    for (int i = 0; i < snakeLength; i++)
    {
        snake[i].position = (Vector2){ 200 - i * CELL_SIZE, 200 };
    }

    Vector2 direction = { CELL_SIZE, 0 };

    // Food
    Vector2 food = {
        (GetRandomValue(0, (SCREEN_WIDTH / CELL_SIZE) - 1)) * CELL_SIZE,
        (GetRandomValue(0, (SCREEN_HEIGHT / CELL_SIZE) - 1)) * CELL_SIZE
    };

    bool gameOver = false;

    while (!WindowShouldClose())
    {
        // Input
        if (IsKeyPressed(KEY_UP) && direction.y == 0) direction = (Vector2){ 0, -CELL_SIZE };
        if (IsKeyPressed(KEY_DOWN) && direction.y == 0) direction = (Vector2){ 0, CELL_SIZE };
        if (IsKeyPressed(KEY_LEFT) && direction.x == 0) direction = (Vector2){ -CELL_SIZE, 0 };
        if (IsKeyPressed(KEY_RIGHT) && direction.x == 0) direction = (Vector2){ CELL_SIZE, 0 };

        if (!gameOver)
        {
            // Move body
            for (int i = snakeLength - 1; i > 0; i--)
            {
                snake[i].position = snake[i - 1].position;
            }

            // Move head
            snake[0].position.x += direction.x;
            snake[0].position.y += direction.y;

            // Wall collision
            if (snake[0].position.x < 0 || snake[0].position.x >= SCREEN_WIDTH ||
                snake[0].position.y < 0 || snake[0].position.y >= SCREEN_HEIGHT)
            {
                gameOver = true;
            }

            // Self collision
            for (int i = 1; i < snakeLength; i++)
            {
                if (Vector2Equals(snake[0].position, snake[i].position))
                {
                    gameOver = true;
                }
            }

            // Food collision
            if (Vector2Equals(snake[0].position, food))
            {
                if (snakeLength < MAX_SNAKE_LENGTH)
                    snakeLength++;

                food = (Vector2){
                    (GetRandomValue(0, (SCREEN_WIDTH / CELL_SIZE) - 1)) * CELL_SIZE,
                    (GetRandomValue(0, (SCREEN_HEIGHT / CELL_SIZE) - 1)) * CELL_SIZE
                };
            }
        }

        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw food
        DrawRectangle(food.x, food.y, CELL_SIZE, CELL_SIZE, RED);

        // Draw snake
        for (int i = 0; i < snakeLength; i++)
        {
            DrawRectangle(
                snake[i].position.x,
                snake[i].position.y,
                CELL_SIZE,
                CELL_SIZE,
                GREEN
            );
        }

        // Game Over text
        if (gameOver)
        {
            DrawText("GAME OVER", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, 40, RED);
            DrawText("Press ESC to exit", SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 50, 20, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}