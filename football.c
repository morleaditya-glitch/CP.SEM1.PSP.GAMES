#include<raylib.h>
#include<stdlib.h>
#include<time.h>

#define MAX_OBSTACLES 3

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Raylib: Mega Obstacle Course");

    // --- AUDIO INITIALIZATION ---
    InitAudioDevice(); 

    // Load Sounds (Ensure these files exist in your project folder!)
    Sound jumpSound = LoadSound("jump.wav");
    Sound collisionSound = LoadSound("collision.wa");
    Sound scoreSound = LoadSound("score.wav"); // Sound when obstacle resets

    srand(time(NULL));

    // Player Variables
    Vector2 ballPos = { 100, 385 };
    float ballRadius = 15.0f;
    float ballVelocityY = 0.0f;
    int jumpCount = 0;
    float gravity = 0.6f;

    // Obstacles Array
    Rectangle obstacles[MAX_OBSTACLES];
    float obstacleSpeeds[MAX_OBSTACLES];
    
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        obstacles[i] = (Rectangle){ 800 + (i * 400), 370, 30, 30 };
        obstacleSpeeds[i] = 5.0f + i; 
    }

    Rectangle platform = { 500, 250, 150, 20 };
    int score = 0;
    bool gameOver = false;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (!gameOver) {
            // --- 1. MOVEMENT & JUMP ---
            if (IsKeyPressed(KEY_SPACE) && jumpCount < 2) {
                ballVelocityY = (jumpCount == 0) ? -10.0f : -12.0f;
                jumpCount++;
                PlaySound(jumpSound); // PLAY JUMP SOUND
            }
            ballVelocityY += gravity;
            ballPos.y += ballVelocityY;

            // --- 2. COLLISIONS ---
            if (ballPos.y >= 385) { ballPos.y = 385; ballVelocityY = 0; jumpCount = 0; }

            if (ballVelocityY > 0 && CheckCollisionCircleRec(ballPos, ballRadius, platform)) {
                if (ballPos.y < platform.y + 10) {
                    ballPos.y = platform.y - ballRadius;
                    ballVelocityY = 0;
                    jumpCount = 0;
                }
            }

            // --- 3. MANAGE MULTIPLE OBSTACLES ---
            for (int i = 0; i < MAX_OBSTACLES; i++) {
                obstacles[i].x -= obstacleSpeeds[i];

                if (obstacles[i].x < -obstacles[i].width) {
                    obstacles[i].x = screenWidth + (rand() % 200);
                    if (rand() % 2 == 0) {
                        obstacles[i].y = 370;
                        obstacles[i].height = 30 + (rand() % 40);
                    } else {
                        obstacles[i].y = 200 + (rand() % 100);
                        obstacles[i].height = 20;
                    }
                    score++;
                    PlaySound(scoreSound); // PLAY SCORE/PASS SOUND
                }

                // Check Crash
                if (CheckCollisionCircleRec(ballPos, ballRadius, obstacles[i])) {
                    gameOver = true;
                    PlaySound(collisionSound); // PLAY COLLISION SOUND
                }
            }

            platform.x -= 4.0f;
            if (platform.x < -platform.width) {
                platform.x = screenWidth + 100;
                platform.y = 180 + (rand() % 100);
            }

        } else if (IsKeyPressed(KEY_ENTER)) {
            for (int i = 0; i < MAX_OBSTACLES; i++) obstacles[i].x = 800 + (i * 400);
            ballPos.y = 385;
            score = 0;
            gameOver = false;
        }

        BeginDrawing();
            ClearBackground(SKYBLUE);
            DrawRectangle(0, 400, screenWidth, 50, DARKGREEN);
            for (int i = 0; i < MAX_OBSTACLES; i++) {
                DrawRectangleRec(obstacles[i], (obstacles[i].y < 300) ? ORANGE : RED);
            }
            DrawRectangleRec(platform, BROWN);
            DrawCircleV(ballPos, ballRadius, WHITE);
            DrawText(TextFormat("Score: %i", score), 10, 10, 20, WHITE);
            if (gameOver) DrawText("CRASHED! ENTER to Reset", 220, 200, 30, RAYWHITE);
        EndDrawing();
    }

    // --- UNLOAD & CLOSE ---
    UnloadSound(jumpSound);
    UnloadSound(collisionSound);
    UnloadSound(scoreSound);
    CloseAudioDevice(); 

    CloseWindow();
    return 0;
}