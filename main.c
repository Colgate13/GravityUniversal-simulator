/**
 * @file main.c
 * @author Gabriel Barros Feitosa Sa (Colgate13) - https://github.com/Colgate13
*/

#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <raylib.h>

/**
 * @details macros declarations
*/
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 700
#define HEIGHT_PROPORTION(x) ((int)(SCREEN_HEIGHT * (x) / 300))
#define WIDTH_PROPORTION(x) ((int)(SCREEN_HEIGHT * (x) / 300))
#define FPS 60
#define G 6.67408e-11
#define TRAIL_LENGTH 1000000

/**
 * @details types declarations
*/
typedef struct
{
  Vector2 positions[TRAIL_LENGTH];
  int index;
  int count;
} Trail;

typedef struct
{
  float mass;
  float radius;
  Vector2 position;
  Vector2 speed;
  Vector2 acceleration;
  Texture2D texture;
} Body;

/**
 * @details global variables
*/
Trail meteorTrail = {0};
float ZOOM = 1.0f;
char str[100];

/**
 * @details functions declarations
*/
void UpdateTrail(Trail *trail, Vector2 newPosition);
double BodyDistance(Body *body1, Body *body2);
void BodyUpdateSpeed(Body *body);
void BodyUpdatePosition(Body *body);
void BodyUpdateAcceleration(Body *body1, Body *body2);
void DrawTrail(const Trail *trail);


int main(void)
{
  printf("Hello!\n");

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "local");
  SetTargetFPS(FPS);

  Texture2D backgroundTexture = LoadTexture("assets/backgrounds/space.png");
  backgroundTexture.height = 3072;
  backgroundTexture.width = 5376;

  Texture2D MeteoriteTexture = LoadTexture("assets/others/meteorite.png");
  MeteoriteTexture.height = HEIGHT_PROPORTION(10);
  MeteoriteTexture.width = WIDTH_PROPORTION(10);

  Texture2D EarthTexture = LoadTexture("assets/words/earth3.png");
  EarthTexture.height = HEIGHT_PROPORTION(60);
  EarthTexture.width = WIDTH_PROPORTION(60);

  Vector2 EarthPosition = {
      .x = SCREEN_WIDTH / 2,
      .y = SCREEN_HEIGHT / 2,
  };

  Body Earth = {
      .mass = 5.972e12,
      .radius = HEIGHT_PROPORTION(60),
      .position = EarthPosition,
      .acceleration = {0, 0},
      .speed = {0, 0},
      .texture = EarthTexture,
  };

  Vector2 MeteoritePosition = {
      .x = 300,
      .y = 200,
  };

  Body Meteorite = {
      .mass = 1,
      .radius = 2,
      .position = MeteoritePosition,
      .speed = {1, -1.2},
      .acceleration = {0, -0.1},
      .texture = MeteoriteTexture,
  };

  while (!WindowShouldClose())
  {
    ZOOM += GetMouseWheelMove() * 0.05f;
    ZOOM = (ZOOM < 0.25f) ? 0.25f : ZOOM;

    if (IsKeyDown(KEY_W))
      Meteorite.speed.y -= 0.00001;
    if (IsKeyDown(KEY_S))
      Meteorite.speed.y += 0.00001;
    if (IsKeyDown(KEY_A))
      Meteorite.speed.x -= 0.00001;
    if (IsKeyDown(KEY_D))
      Meteorite.speed.x += 0.00001;
    if (IsKeyDown(KEY_SPACE))
    {
      Meteorite.acceleration.x = 0;
      Meteorite.acceleration.y = -0.1;
      Meteorite.speed.x = 1;
      Meteorite.speed.y = -1.2;
      Meteorite.position.x = 300;
      Meteorite.position.y = 200;
    }

    BeginDrawing();
    ClearBackground(CLITERAL(Color){0x18, 0x18, 0x18, 0xFF});

    Camera2D Camera = {
        .target = (Vector2){Meteorite.position.x, Meteorite.position.y},
        .offset = (Vector2){SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2},
        .rotation = 0.0f,
        .zoom = ZOOM,
    };

    BeginMode2D((Camera2D)Camera);
    DrawTextureEx(backgroundTexture, (Vector2){-backgroundTexture.width / 2, -backgroundTexture.height / 2}, 0.0f, 1.0f, WHITE);

    DrawTextureV(Earth.texture,
                 (Vector2){
                     Earth.position.x - Earth.texture.width / 2,
                     Earth.position.y - Earth.texture.height / 2},
                 WHITE);
    DrawTextureV(Meteorite.texture,
                 (Vector2){
                     Meteorite.position.x - Meteorite.texture.width / 2,
                     Meteorite.position.y - Meteorite.texture.height / 2},
                 WHITE);
    DrawTrail(&meteorTrail);
    BodyUpdateAcceleration(&Meteorite, &Earth);
    BodyUpdateSpeed(&Meteorite);
    BodyUpdatePosition(&Meteorite);
    UpdateTrail(&meteorTrail, Meteorite.position);

    snprintf(str, sizeof(str), "Meteorite.speed.{x: %f, y: %f}", Meteorite.speed.x, Meteorite.speed.y);
    DrawText(str, 10, 10, 20, WHITE);

    snprintf(str, sizeof(str), "Meteorite.acceleration.{x: %f, y: %f}", Meteorite.acceleration.x, Meteorite.acceleration.y);
    DrawText(str, 10, 30, 20, WHITE);

    snprintf(str, sizeof(str), "Meteorite.position.{x: %f, y: %f", Meteorite.position.x, Meteorite.position.y);
    DrawText(str, 10, 50, 20, WHITE);

    snprintf(str, sizeof(str), "Distancia Meteorite to Earth: %f", BodyDistance(&Meteorite, &Earth));
    DrawText(str, 10, 70, 20, WHITE);

    EndMode2D();
    EndDrawing();
  }

  return 0;
}

void UpdateTrail(Trail *trail, Vector2 newPosition)
{
  trail->positions[trail->index] = newPosition;
  trail->index = (trail->index + 1) % TRAIL_LENGTH;
  if (trail->count < TRAIL_LENGTH)
  {
    trail->count++;
  }
}

double BodyDistance(Body *body1, Body *body2)
{
  // pitagoras = a² + b² = c²
  // H = sqrt((x2 - x1)² + (y2 - y1)²)
  // if (body1 == NULL || body2 == NULL) {
  //     return -1;
  // }

  double a = body2->position.x - body1->position.x;
  double b = body2->position.y - body1->position.y;

  double distance = sqrt(a * a + b * b);
  return distance;
}

void BodyUpdateSpeed(Body *body)
{
  body->speed.x = body->speed.x + body->acceleration.x;
  body->speed.y = body->speed.y + body->acceleration.y;
}

void BodyUpdatePosition(Body *body)
{
  body->position.x = body->position.x + body->speed.x;
  body->position.y = body->position.y + body->speed.y;
}

void BodyUpdateAcceleration(Body *body1, Body *body2)
{
  double distance = BodyDistance(body1, body2);

  printf("distance: %f\n", distance);

  // 6,67408×10^−11 × ((1×10^10 × 5,972×10^24) ÷ (259,699826723 × 259,699826723))
  double force = G * ((body1->mass * body2->mass) / (distance * distance));

  printf("force: %f\n", force);

  double deltaX = body2->position.x - body1->position.x;
  double deltaY = body2->position.y - body1->position.y;

  double cosT = deltaX / distance;
  double senT = deltaY / distance;

  float forceX = (float)force * (float)cosT;
  float forceY = (float)force * (float)senT;

  body1->acceleration.x = forceX / body1->mass;
  body1->acceleration.y = forceY / body1->mass;

  printf("forceX: %f\n", forceX);
  printf("forceY: %f\n", forceY);

  printf("body1->acceleration.x: %f\n", body1->acceleration.x);
  printf("body1->acceleration.y: %f\n", body1->acceleration.y);
}

void DrawTrail(const Trail *trail)
{
  for (int i = 0; i < trail->count; i++)
  {
    DrawCircleV(trail->positions[i], 2, GREEN);
  }
}
