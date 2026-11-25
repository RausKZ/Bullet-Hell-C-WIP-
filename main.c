#include "raylib.h"
#include <math.h>
#include <time.h> // For randomness
#include <stdlib.h>
#include "CoreGameLogic.h"
#define MAXBULLETS 500000

int main(){
  srand(time(NULL));
  // SCREEN SETTINGS
    const int ScreenWidth = 1280;
    const int ScreenHeight = 960;
    InitWindow(ScreenWidth,ScreenHeight,"Bullet hell");
    Texture2D background = LoadTexture("Assets/Mokou1.png");
  // PLAYER
    Player player = { (ScreenWidth/6+ScreenWidth/4),ScreenHeight/2};
    int CircleRadius = 5;
  // BULLET STUFF
    Bullets *bullets = malloc(MAXBULLETS*sizeof(Bullets)); 
    int bulletCount = 0;
    int bulletDisabledCount = 0;
    Vector2 Size = {10,20};
    Color color = BLUE;
  // SPAWNING STUFF 
    float spawnCooldown = 3;
    float spawnCooldownTimer = spawnCooldown;
    float DeltaTime = 0;
  // Score
    long int Score = 0;
  // GameState
  // 0 - main menu
  // 1 - Main Game Loop
  // 2 - Gameover Screen
  // 3 - Settings menu
    int GameState = 0;
    int checker;
    int CurrentTab = 1;
  SetExitKey(KEY_NULL); 
  SetTargetFPS(60);
  // MAIN GAME LOOP
  while(!WindowShouldClose()){
    switch(GameState){
      case 0:
        // Mouse position
        checker = MouseClickingLogic(ScreenWidth,ScreenHeight);
        if(checker == 1){
          GameState = 1;
          break;
        }
        if(checker == 0){
          CloseWindow();
          break;
        }
        if(checker == 2){
          GameState = 3;
          break;
        }
          BeginDrawing();
            int *Selection=malloc(sizeof(int));
            ClearBackground(BLACK);
            DrawTexture(background,0,0,WHITE);
            DrawMainMenu(ScreenWidth,ScreenHeight);
            free(Selection);
          EndDrawing();
        break;
      case 1:
        if(bulletCount >= MAXBULLETS){
          bulletCount = 0;
          bulletDisabledCount = 0;
        }
        spawnCooldownTimer--;
        if(spawnCooldownTimer < 0 && DeltaTime >= 1.5){
          SpawnBullet(spawnCooldown, &spawnCooldownTimer, bullets,&bulletCount, ScreenWidth, ScreenHeight);
        }
          DisableBullets(&bulletCount,bullets,ScreenHeight);
          CircleMovement(&player.MovementCircleX,&player.MovementCircleY,ScreenWidth,ScreenHeight,CircleRadius);
          Vector2 Player = {player.MovementCircleX,player.MovementCircleY};
          BeginDrawing();
            ClearBackground(BLACK);
            DrawCircle(player.MovementCircleX,player.MovementCircleY,CircleRadius,WHITE);
            DrawCircleLines(player.MovementCircleX,player.MovementCircleY,CircleRadius,RED);
            ScoreBoard(&Score,ScreenWidth,ScreenHeight,DeltaTime);
            for(int i = 0; i<bulletCount; i++){
              if(!bullets[i].disabled)
              DrawRectangleV(bullets[i].position,Size,BLUE);
              Rectangle bullet1 = {bullets[i].position.x, bullets[i].position.y, Size.x, Size.y};
              if(CheckCollisionCircleRec(Player, CircleRadius, bullet1)){
                GameState = 2;
                player.MovementCircleX = (ScreenWidth/6+ScreenWidth/4);
                player.MovementCircleY = ScreenHeight/2;
                EndDrawing();
                break;
              }
            }
            DrawRectangleLines(ScreenWidth/6,10,ScreenWidth / 2,ScreenHeight-20,WHITE);
          EndDrawing();
                DeltaTime += GetFrameTime();
        break;
      case 2:
        DeltaTime = 0;
        BeginDrawing();
          checker = DeadScreenDraw(&Score, ScreenWidth, ScreenHeight, &GameState, &bulletCount);
        EndDrawing();
        break;
      case 3:
        int ActiveTab = SettingsMenuPicker(ScreenWidth,ScreenHeight);
        if(ActiveTab != 0)
          CurrentTab = ActiveTab;
        BeginDrawing();
          ClearBackground(BLACK);
          DrawSettingsMenu(CurrentTab);
        EndDrawing();
        if (IsKeyPressed(KEY_ESCAPE)) {
          GameState = 0;
         }
        break;
      default:
        CloseWindow();
      }
  }
    ending:
    UnloadTexture(background);
    CloseWindow();
    free(bullets);
    return 0;
}
