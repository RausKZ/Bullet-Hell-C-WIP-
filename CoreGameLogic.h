#ifndef CoreGameLogic_H
#define CoreGameLogic_H
#include "raylib.h"
#include <math.h>
#define MAXBULLETS 500000

typedef struct{
    Vector2 position;
    Vector2 Acceleration;
    bool disabled;
    Color color;
}Bullets;

typedef struct{
    int MovementCircleX;
    int MovementCircleY;
}Player;

void CircleMovement(int *Xaxis, int *Yaxis,int ScreenWidth,int ScreenHeight,int CircleRadius);
void ScoreBoard(long int *Score,int ScreenWidth,int ScreenHeight, float DeltaTime);
void DrawMainMenu(int ScreenWidth,int ScreenHeight);
int MouseClickingLogic(int ScreenWidth, int ScreenHeight);
void SpawnBullet(float spawnCooldown, float *spawnCooldownTimer, Bullets *bullets, int *bulletCount, int ScreenWidth, int ScreenHeight);
void DisableBullets(int *bulletCount, Bullets *bullets, int ScreenHeight);
int DeadScreenDraw(long int *Score, int ScreenWidth, int ScreenHeight, int *GameState, int *bulletCount);
void DrawVideoMenu(float startX, float startY, Color cTextPrimary, Color cTextSecondary, Color cDropdownBg);
void DrawAudioMenu(float startX, float startY, Color cTextPrimary, Color cTextSecondary, Color cSliderTrack, Color cSliderThumb);
void DrawControlsMenu(float startX, float startY, Color cTextPrimary, Color cTextSecondary, Color cDropdownBg);
void DrawSettingsMenu(int activeTab);
int SettingsMenuPicker(int ScreenWidth, int ScreenHeight);
#endif
