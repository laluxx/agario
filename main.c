#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <time.h>

const int sw = 1920;
const int sh = 1080;

const int mapWidth = 1920;
const int mapHeight = 1080;

#define FOOD_COUNT 80
const float foodRadius = 5.0f;

typedef struct {
    Vector2 pos;
    float speed;
    float radius;
    Color color;
    float mass;
} Player;

typedef struct {
    Vector2 pos;
    Color color;
} Food;

Player player;
Food foods[FOOD_COUNT];

void InitGame() {
    // Initialize player
    player.pos = (Vector2){sw / 2.0, sh / 2.0};
    player.speed = 5.0f;
    player.radius = 20.0f;
    player.color = BLUE;
    player.mass = 1.0f;

    // Initialize random seed
    srand(time(NULL));

    // Initialize food positions
    for (int i = 0; i < FOOD_COUNT; i++) {
        foods[i].pos = (Vector2){(float)(rand() % mapWidth), (float)(rand() % mapHeight)};
        foods[i].color = GREEN;
    }
}

float foodMassFactor = 0.07f;
void UpdateGame() {
    // Player movement (smoothly follow the mouse)
    Vector2 mousePos = GetMousePosition();
    Vector2 direction = Vector2Normalize(Vector2Subtract(mousePos, player.pos));
    player.pos = Vector2Add(player.pos, Vector2Scale(direction, player.speed));

    // Collision detection and eating food
    for (int i = 0; i < FOOD_COUNT; i++) {
        if (CheckCollisionCircles(player.pos, player.radius * player.mass,
                                  foods[i].pos, foodRadius)) {
            // Collision detected, eat the food
            player.mass += foodMassFactor;
            // Respawn the food in a random position
            foods[i].pos =
                (Vector2){(float)(rand() % mapWidth), (float)(rand() % mapHeight)};
        }
    }
}

void DrawGame() {
    BeginDrawing();
    ClearBackground(BLACK);


    // Draw food
    for (int i = 0; i < FOOD_COUNT; i++) {
        DrawCircleV(foods[i].pos, foodRadius, foods[i].color);
    }

    // Draw player
    DrawCircleV(player.pos, player.radius * player.mass, player.color);

    EndDrawing();
}

int main() {
    InitWindow(sw, sh, "Agario");
    SetTargetFPS(60);

    InitGame();

    while (!WindowShouldClose()) {
        UpdateGame();
        DrawGame();
    }

    CloseWindow();
    return 0;
}

