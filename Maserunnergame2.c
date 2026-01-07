#include "raylib.h"
#include <stdlib.h>
#include <time.h>

// --- Game Constants ---
#define TILE_SIZE 40
#define INITIAL_SIZE 15

typedef struct {
    int x, y;
} Coord;

// --- Global Game State ---
int **maze = NULL;
int mazeWidth = INITIAL_SIZE;
int mazeHeight = INITIAL_SIZE;
int level = 1;
float levelTimer = 60.0f; // 60 seconds to solve
Coord player;
Coord exitPos;

// --- Memory Management ---
void InitMaze(int w, int h) {
    maze = (int **)malloc(h * sizeof(int *));
    for (int i = 0; i < h; i++) {
        maze[i] = (int *)malloc(w * sizeof(int));
        for (int j = 0; j < w; j++) maze[i][j] = 1; // Start as all walls
    }
}

void FreeMaze() {
    if (maze != NULL) {
        for (int i = 0; i < mazeHeight; i++) free(maze[i]);
        free(maze);
    }
}

// Recursive Backtracking Algorithm
// This ensures a "Perfect Maze" (one path to every point, no loops)
void GenerateMaze(int x, int y) {
    int dirs[4][2] = {{0,2}, {0,-2}, {2,0}, {-2,0}};
    
    // Shuffle directions for randomness
    for (int i = 0; i < 4; i++) {
        int r = GetRandomValue(0, 3);
        int tempX = dirs[r][0], tempY = dirs[r][1];
        dirs[r][0] = dirs[i][0]; dirs[r][1] = dirs[i][1];
        dirs[i][0] = tempX; dirs[i][1] = tempY;
    }

    for (int i = 0; i < 4; i++) {
        int nx = x + dirs[i][0];
        int ny = y + dirs[i][1];

        if (nx > 0 && nx < mazeWidth-1 && ny > 0 && ny < mazeHeight-1 && maze[ny][nx] == 1) {
            maze[ny][nx] = 0; // Create path
            maze[y + dirs[i][1]/2][x + dirs[i][0]/2] = 0; // Remove wall between
            GenerateMaze(nx, ny);
        }
    }
}

void StartNewLevel() {
    FreeMaze();
    mazeWidth = INITIAL_SIZE + (level * 2); 
    mazeHeight = INITIAL_SIZE + (level * 2);
    InitMaze(mazeWidth, mazeHeight);
    
    maze[1][1] = 0;
    GenerateMaze(1, 1);
    
    player = (Coord){1, 1};
    exitPos = (Coord){mazeWidth - 2, mazeHeight - 2};
    maze[exitPos.y][exitPos.x] = 2; // Green Exit
    levelTimer = 60.0f + (level * 5); // More time for bigger levels
}

int main() {
    srand(time(NULL));
    InitWindow(1200, 800, "Labyrinth OS - Advanced Maze System");
    SetTargetFPS(60);

    StartNewLevel();
    
    Camera2D camera = { 0 };
    camera.zoom = 1.0f;
    camera.offset = (Vector2){ 600, 400 };

    while (!WindowShouldClose()) {
        // --- Update Logic ---
        if (levelTimer > 0) levelTimer -= GetFrameTime();

        Coord next = player;
        if (IsKeyPressed(KEY_W)) next.y--;
        if (IsKeyPressed(KEY_S)) next.y++;
        if (IsKeyPressed(KEY_A)) next.x--;
        if (IsKeyPressed(KEY_D)) next.x++;

        // Collision Check
        if (maze[next.y][next.x] != 1) {
            player = next;
            // Breadcrumb effect
            if (maze[player.y][player.x] == 0) maze[player.y][player.x] = 3;
        }

        // Win Condition
        if (player.x == exitPos.x && player.y == exitPos.y) {
            level++;
            StartNewLevel();
        }

        camera.target = (Vector2){ player.x * TILE_SIZE, player.y * TILE_SIZE };

        // --- Drawing ---
        BeginDrawing();
            ClearBackground((Color){15, 15, 25, 255});

            BeginMode2D(camera);
                for (int y = 0; y < mazeHeight; y++) {
                    for (int x = 0; x < mazeWidth; x++) {
                        Rectangle r = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                        
                        if (maze[y][x] == 1) DrawRectangleRec(r, (Color){45, 50, 65, 255}); // Wall
                        else if (maze[y][x] == 2) DrawRectangleRec(r, LIME); // Goal
                        else if (maze[y][x] == 3) DrawRectangleRec(r, (Color){25, 25, 45, 255}); // Path Taken
                        
                        DrawRectangleLinesEx(r, 0.5f, (Color){100, 100, 150, 30});
                    }
                }
                
                // Draw Player
                DrawCircle(player.x * TILE_SIZE + 20, player.y * TILE_SIZE + 20, 15, SKYBLUE);
                DrawCircleGradient(player.x * TILE_SIZE + 20, player.y * TILE_SIZE + 20, 30, Fade(SKYBLUE, 0.4f), BLANK);
            EndMode2D();

            // --- HUD (UI Elements) ---
            DrawRectangle(20, 20, 250, 120, Fade(BLACK, 0.7f));
            DrawText(TextFormat("LEVEL: %02i", level), 40, 40, 25, RAYWHITE);
            DrawText(TextFormat("TIME: %.1fs", levelTimer), 40, 75, 20, (levelTimer < 10) ? RED : GOLD);
            DrawText("REACH THE GREEN ZONE", 40, 105, 15, SKYBLUE);

            // Minimap Rendering
            int mapS = 3; // Minimap pixel size
            int mapOffset = 20;
            for (int y = 0; y < mazeHeight; y++) {
                for (int x = 0; x < mazeWidth; x++) {
                    Color mc = (maze[y][x] == 1) ? DARKGRAY : WHITE;
                    if (x == player.x && y == player.y) mc = SKYBLUE;
                    DrawRectangle(GetScreenWidth() - (mazeWidth * mapS) - mapOffset + (x * mapS), 
                                  mapOffset + (y * mapS), mapS, mapS, mc);
                }
            }

            // Game Over Screen
            if (levelTimer <= 0) {
                DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.8f));
                DrawText("TIME EXPIRED!", GetScreenWidth()/2 - 120, GetScreenHeight()/2 - 20, 40, RED);
                DrawText("Press 'R' to Try Again", GetScreenWidth()/2 - 110, GetScreenHeight()/2 + 30, 20, RAYWHITE);
                if (IsKeyPressed(KEY_R)) {
                    level = 1;
                    StartNewLevel();
                }
            }

        EndDrawing();
    }

    FreeMaze();
    CloseWindow();
    return 0;
}