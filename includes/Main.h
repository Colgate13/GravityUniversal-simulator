#ifndef MAIN_H
#define MAIN_H
#define TRAIL_LENGTH 1000000

/**
 * @details macros declarations
 */
#define VERBOSE 1
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 700

#define MAX_BODIES 100
#define CONST_PROPORTION 10
#define FPS 60
#define G 6.67408e-11
#define TRAIL_LENGTH 1000000
#define ONE_AU 100 // 1 AU = 100 pixels
#define AU(x) ((int)(x * ONE_AU))

#include <raylib.h>

typedef struct
{
  Vector2 positions[TRAIL_LENGTH];
  int index;
  int count;
} Trail;


typedef struct
{
  double x;
  double y;
} BodyVector;

typedef struct
{
  char *name;
  float mass;
  float radius;
  Vector2 position;
  Vector2 speed;
  BodyVector acceleration;
  Vector2 force;
  Texture2D texture;
  Trail trail;
} Body;

typedef struct
{
  Vector2 position;
  unsigned long size;
  Color color;
} Crosshair;

/**
 * @details functions declarations
 */
Body *BodyCreate(char *name, float mass, float radius, Vector2 position, Vector2 speed, BodyVector acceleration, Texture2D texture);
void UpdateTrail(Trail *trail, Vector2 newPosition);
double BodyDistance(Body *body1, Body *body2);
void BodyUpdateSpeed(Body *body);
void BodyUpdatePosition(Body *body);
void BodiesUpdateAcceleration(Body *Bodies[], unsigned long size);
void DrawTrail(const Trail *trail);
float ControlZoom(float ZOOM);
Camera2D *CreateCamera(Vector2 target, Vector2 offset, float rotation, float zoom);
void DrawBodyInfo(Body *body);
void generateRandomMeteoriteName(char *name, int nameLength);
Crosshair *DrawCrosshair(int size, Color color);


#endif