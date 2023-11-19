/**
 * @file main.c
 * @author Gabriel Barros Feitosa Sa (Colgate13) - https://github.com/Colgate13
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <raylib.h>

/**
 * @details macros declarations
 */
#define VERBOSE 1
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
  char *name;
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
Body *BodyCreate(char *name, float mass, float radius, Vector2 position, Vector2 speed, Vector2 acceleration, Texture2D texture);
void UpdateTrail(Trail *trail, Vector2 newPosition);
double BodyDistance(Body *body1, Body *body2);
void BodyUpdateSpeed(Body *body);
void BodyUpdatePosition(Body *body);
void BodyUpdateAcceleration(Body *body1, Body *body2);
void DrawTrail(const Trail *trail);
float ControlZoom(float ZOOM);
Camera2D *CreateCamera(Vector2 target, Vector2 offset, float rotation, float zoom);
void DrawBodyInfo(Body *body);

int main(void)
{
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Newton's Law of Universal Gravitation");
  SetTargetFPS(FPS);

  /**
   * @details load textures
   */
  Texture2D backgroundTexture = LoadTexture("assets/backgrounds/space.png");
  backgroundTexture.height = 3072;
  backgroundTexture.width = 5376;

  Texture2D MeteoriteTexture = LoadTexture("assets/others/meteorite.png");
  MeteoriteTexture.height = HEIGHT_PROPORTION(10);
  MeteoriteTexture.width = WIDTH_PROPORTION(10);

  Texture2D EarthTexture = LoadTexture("assets/words/earth3.png");
  EarthTexture.height = HEIGHT_PROPORTION(60);
  EarthTexture.width = WIDTH_PROPORTION(60);

  Body *Earth = BodyCreate("Earth", 5.972e12, HEIGHT_PROPORTION(60), (Vector2){SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2}, (Vector2){0, 0}, (Vector2){0, 0}, EarthTexture);
  Body *Meteorite = BodyCreate("Meteorite1", 1, HEIGHT_PROPORTION(10), (Vector2){300, 200}, (Vector2){1, -1.2}, (Vector2){0, -0.1}, MeteoriteTexture);

  // create array of bodies
  Body *bodies[] = {Earth, Meteorite};

  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(CLITERAL(Color){0x18, 0x18, 0x18, 0xFF});

    ZOOM = ControlZoom(ZOOM);
    Camera2D *Camera = CreateCamera((Vector2){Meteorite->position.x, Meteorite->position.y}, (Vector2){SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2}, 0.0f, ZOOM);
    BeginMode2D(*Camera);

    DrawTextureEx(backgroundTexture, (Vector2){-backgroundTexture.width / 2, -backgroundTexture.height / 2}, 0.0f, 1.0f, WHITE);

    BodyUpdateAcceleration(Meteorite, Earth);
    DrawTrail(&meteorTrail);
    for (int i = 0; i < sizeof(bodies) / sizeof(bodies[0]); i++)
    {
      Body *body = bodies[i];

      DrawTextureV(body->texture,
                   (Vector2){
                       body->position.x - body->texture.width / 2,
                       body->position.y - body->texture.height / 2},
                   WHITE);

      BodyUpdateSpeed(body);
      BodyUpdatePosition(body);
    }
    UpdateTrail(&meteorTrail, Meteorite->position);
    EndMode2D();
    DrawBodyInfo(Meteorite);
    EndDrawing();
  }

  return 0;
}

Body *BodyCreate(char *name, float mass, float radius, Vector2 position, Vector2 speed, Vector2 acceleration, Texture2D texture)
{
  Body *body = malloc(sizeof(Body));
  assert(body != NULL);

  body->name = name;
  body->mass = mass;
  body->radius = radius;
  body->position = position;
  body->speed = speed;
  body->acceleration = acceleration;
  body->texture = texture;

  return body;
}

void DrawBodyInfo(Body *body)
{
  snprintf(str, sizeof(str), "Meteorite.speed.{x: %f, y: %f}", body->speed.x, body->speed.y);
  DrawText(str, 10, 10, 20, WHITE);

  snprintf(str, sizeof(str), "body.acceleration.{x: %f, y: %f}", body->acceleration.x, body->acceleration.y);
  DrawText(str, 10, 30, 20, WHITE);

  snprintf(str, sizeof(str), "body.position.{x: %f, y: %f", body->position.x, body->position.y);
  DrawText(str, 10, 50, 20, WHITE);
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

float ControlZoom(float zoom)
{
  zoom += GetMouseWheelMove() * 0.05f;
  zoom = (zoom < 0.25f) ? 0.25f : zoom;

  return zoom;
}

Camera2D *CreateCamera(Vector2 target, Vector2 offset, float rotation, float zoom)
{
  Camera2D *camera = malloc(sizeof(Camera2D));
  assert(camera != NULL);

  camera->target = target;
  camera->offset = offset;
  camera->rotation = rotation;
  camera->zoom = zoom;

  return camera;
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

  // 6,67408×10^−11 × ((1×10^10 × 5,972×10^24) ÷ (259,699826723 × 259,699826723))
  double force = G * ((body1->mass * body2->mass) / (distance * distance));

  double deltaX = body2->position.x - body1->position.x;
  double deltaY = body2->position.y - body1->position.y;

  double cosT = deltaX / distance;
  double senT = deltaY / distance;

  float forceX = (float)force * (float)cosT;
  float forceY = (float)force * (float)senT;

  body1->acceleration.x = forceX / body1->mass;
  body1->acceleration.y = forceY / body1->mass;

  body2->acceleration.x = forceX / body2->mass;
  body2->acceleration.y = forceY / body2->mass;

  if (VERBOSE == 1)
  {
    printf("{ Body1: '%s', Body2: '%s', Distance: %f, Force: { Force: %f, ForceX: %f, ForceY: %f }\n", body1->name, body2->name, distance, force, forceX, forceY);
  }
}

void DrawTrail(const Trail *trail)
{
  for (int i = 0; i < trail->count; i++)
  {
    DrawCircleV(trail->positions[i], 2, GREEN);
  }
}
