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
#include <time.h>
#include <raylib.h>
#include "../includes/Main.h"

/**
 * @details global variables
 */
float ZOOM = 1.0f;
char str[100];
unsigned long BodieSize = 3;

int main(void)
{
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Newton's Law of Universal Gravitation");
  SetTargetFPS(FPS);

  /**
   * @details load textures
   */
  Texture2D MeteoriteTexture = LoadTexture("assets/others/meteorite.png");
  MeteoriteTexture.height = AU(1);
  MeteoriteTexture.width = AU(1);

  Texture2D EarthTexture = LoadTexture("assets/words/earth.png");
  EarthTexture.height = AU(5);
  EarthTexture.width = AU(5);

  Texture2D SunTexture = LoadTexture("assets/others/sun.png");
  SunTexture.height = AU(25);
  SunTexture.width = AU(25);

  Body *Sun = BodyCreate("Sun", 100e15, AU(25) / 2, (Vector2){6200 / 2, 6200 / 2}, (Vector2){5, -5}, (BodyVector){0, 0}, SunTexture);
  Body *Earth = BodyCreate("Earth", 5.972e12, AU(5) / 2, (Vector2){0, -5}, (Vector2){40, -40}, (BodyVector){0, 0}, EarthTexture);
  Body *Meteorite = BodyCreate("Meteorite", 1e10, AU(1) / 2, (Vector2){300, 250}, (Vector2){40, -40}, (BodyVector){0, -0.1}, MeteoriteTexture);

  Body **bodies = malloc(3 * sizeof(Body *));
  bodies[0] = Earth;
  bodies[1] = Sun;
  bodies[2] = Meteorite;
  unsigned int meteorTarget = 0;

  Crosshair *crosshair = DrawCrosshair(10, WHITE);

  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(CLITERAL(Color){0x18, 0x18, 0x18, 0xFF});

    if (meteorTarget == 1)
    {
      crosshair->position.x = Earth->position.x;
      crosshair->position.y = Earth->position.y;
    }

    if (IsKeyDown(KEY_W))
      crosshair->position.y -= 1 + (CONST_PROPORTION / ZOOM);
    if (IsKeyDown(KEY_S))
      crosshair->position.y += 1 + (CONST_PROPORTION / ZOOM);
    if (IsKeyDown(KEY_A))
      crosshair->position.x -= 1 + (CONST_PROPORTION / ZOOM);
    if (IsKeyDown(KEY_D))
      crosshair->position.x += 1 + (CONST_PROPORTION / ZOOM);
    if (IsKeyPressed(KEY_SPACE))
    {
      meteorTarget = (meteorTarget == 0) ? 1 : 0;
    }

    ZOOM = ControlZoom(ZOOM);
    Camera2D *Camera = CreateCamera((Vector2){crosshair->position.x, crosshair->position.y}, (Vector2){SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2}, 0.0f, ZOOM);
    BeginMode2D(*Camera);
    float raioCircleLines = (CONST_PROPORTION / ZOOM) + 200;
    int fontSize = (CONST_PROPORTION / ZOOM) + 300;
    Color textColor = WHITE;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
      char meteoriteName[20];
      generateRandomMeteoriteName(meteoriteName, 15);

      Body *newMeteor = BodyCreate(meteoriteName, 1e10, AU(1) / 2, crosshair->position, (Vector2){40, -40}, (BodyVector){0, -0.1}, MeteoriteTexture);

      bodies = realloc(bodies, (BodieSize + 1) * sizeof(Body *));

      BodieSize++;
      bodies = realloc(bodies, BodieSize * sizeof(Body *));
      if (bodies == NULL)
      {
        printf("Erro ao alocar memória para o novo corpo\n");
        exit(1);
      }
      bodies[BodieSize - 1] = newMeteor;
    }

    BodiesUpdateAcceleration(bodies, BodieSize);
    for (unsigned long i = 0; i < BodieSize; i++)
    {
      Body *body = bodies[i];
      DrawTrail(&body->trail);
      BodyUpdateSpeed(body);
      BodyUpdatePosition(body);
      DrawTextureV(body->texture,
                   (Vector2){
                       body->position.x - body->texture.width / 2,
                       body->position.y - body->texture.height / 2},
                   WHITE);
      UpdateTrail(&body->trail, body->position);
      DrawText(body->name, body->position.x - MeasureText(body->name, fontSize) / 2, body->position.y - (body->radius) - fontSize, fontSize, textColor); // TERRA
      DrawCircleLines(body->position.x, body->position.y, raioCircleLines, GREEN);
    }
    DrawLine(crosshair->position.x - crosshair->size, crosshair->position.y, crosshair->position.x + crosshair->size, crosshair->position.y, crosshair->color);
    DrawLine(crosshair->position.x, crosshair->position.y - crosshair->size, crosshair->position.x, crosshair->position.y + crosshair->size, crosshair->color);

    EndMode2D();
    DrawBodyInfo(Earth);
    EndDrawing();
  }

  return 0;
}

Body *BodyCreate(char *name, float mass, float radius, Vector2 position, Vector2 speed, BodyVector acceleration, Texture2D texture)
{
  Body *body = malloc(sizeof(Body));
  assert(body != NULL);

  body->name = name;
  body->mass = mass;
  body->radius = radius;
  body->position = position;
  body->speed = speed;
  body->acceleration = acceleration;
  body->force = (Vector2){0, 0};
  body->texture = texture;
  body->trail = (Trail){0};

  return body;
}

void DrawBodyInfo(Body *body)
{
  snprintf(str, sizeof(str), "%s.speed.{x: %f, y: %f}", body->name, body->speed.x, body->speed.y);
  DrawText(str, 10, 10, 20, WHITE);

  snprintf(str, sizeof(str), "%s.acceleration.{x: %f, y: %f}", body->name, body->acceleration.x, body->acceleration.y);
  DrawText(str, 10, 30, 20, WHITE);

  snprintf(str, sizeof(str), "%s.position.{x: %f, y: %f", body->name, body->position.x, body->position.y);
  DrawText(str, 10, 50, 20, WHITE);
}

void UpdateTrail(Trail *trail, Vector2 newPosition)
{
  if (trail->count < TRAIL_LENGTH)
  {
    trail->positions[trail->count] = newPosition;
    trail->count++;
  }
  else
  {
    for (int i = 0; i < TRAIL_LENGTH - 1; i++)
    {
      trail->positions[i] = trail->positions[i + 1];
    }

    trail->positions[TRAIL_LENGTH - 1] = newPosition;
  }
}

float ControlZoom(float zoom)
{
  const float factor = 0.1f;
  float zoomPower = zoom * factor;
  zoomPower = (zoomPower > 0.1f) ? 0.1f : zoomPower;
  zoom += GetMouseWheelMove() * zoomPower;
  zoom = (zoom < 0.000005f) ? 0.000005f : zoom;
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

void BodiesUpdateAcceleration(Body *Bodies[], unsigned long size)
{
  for (unsigned long i = 0; i < size; i++)
  {
    Body *body1 = Bodies[i];

    body1->force.x = 0;
    body1->force.y = 0;

    for (unsigned long j = 0; j < size; j++)
    {
      Body *body2 = Bodies[j];
      if (body1 != body2)
      {
        double distance = BodyDistance(body1, body2);

        if (distance < body1->radius + body2->radius)
        {
          printf("Colisão entre %s e %s\n", body1->name, body2->name);

          if (body1->mass > body2->mass)
          {
            body1->mass += body2->mass;

            float body1Area = PI * (body1->radius * body1->radius);
            float body2Area = PI * (body2->radius * body2->radius);
            float area = body1Area + body2Area;
            float radius = sqrt(area / PI);

            body1->radius = radius;
            body1->texture.height = radius * 2;
            body1->texture.width = radius * 2;

            Bodies[j] = Bodies[size - 1];
            Bodies[size - 1] = NULL;
            size--;
            BodieSize--;
          }
          else
          {
            body2->mass += body1->mass;

            float body1Area = PI * (body1->radius * body1->radius);
            float body2Area = PI * (body2->radius * body2->radius);

            float area = body1Area + body2Area;
            float radius = sqrt(area / PI);

            body2->radius = radius;
            body2->texture.height = radius * 2;
            body2->texture.width = radius * 2;

            Bodies[i] = Bodies[size - 1];
            Bodies[size - 1] = NULL;
            size--;
            BodieSize--;
          }

          continue;
        }

        double force = G * ((body1->mass * body2->mass) / (distance * distance));

        double deltaX = body2->position.x - body1->position.x;
        double deltaY = body2->position.y - body1->position.y;

        double cosT = deltaX / distance;
        double senT = deltaY / distance;

        double forceX = force * cosT;
        double forceY = force * senT;

        body1->force.x += forceX;
        body1->force.y += forceY;

        if (VERBOSE == 1)
        {
          printf("BodiesUpdateAcceleration -> { Body1: '%s', Body2: '%s', Distance: %f, Force: { Force: %f, ForceX: %f, ForceY: %f }\n", body1->name, body2->name, distance, force, forceX, forceY);
        }
      }
    }

    body1->acceleration.x = body1->force.x / body1->mass;
    body1->acceleration.y = body1->force.y / body1->mass;
  }
}

void DrawTrail(const Trail *trail)
{
  for (int i = 0; i < trail->count - 1; i++)
  {
    int nextIndex = (i + 1) % TRAIL_LENGTH;
    DrawLineV(trail->positions[i], trail->positions[nextIndex], GREEN);
  }
}

void generateRandomMeteoriteName(char *name, int nameLength)
{
  const char *base = "Meteorite";
  const int baseLength = strlen(base);

  strcpy(name, base);
  srand(time(NULL));

  for (int i = baseLength; i < nameLength; i++)
  {
    char randomChar = 'A' + (rand() % 26);
    name[i] = randomChar;
  }

  name[nameLength] = '\0';
}

Crosshair *DrawCrosshair(int size, Color color)
{
  Crosshair *crosshair = malloc(sizeof(Crosshair));
  assert(crosshair != NULL);

  crosshair->position.x = 3100;
  crosshair->position.y = 3100;
  crosshair->size = size;
  crosshair->color = color;

  return crosshair;
}
