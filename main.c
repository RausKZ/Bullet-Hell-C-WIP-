#include "raylib.h"
#include <time.h> // For randomness
#include <stdlib.h>
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

void CircleMovement(int *Xaxis, int *Yaxis,int ScreenWidth,int ScreenHeight,int CircleRadius){
    int FastMovementSpeed = 6;
    int SlowMovementSpeed = 3;
    // Y AXIS
    if(IsKeyDown(KEY_W)){
        if(IsKeyDown(KEY_LEFT_SHIFT)){
            *Yaxis-=SlowMovementSpeed;
        }else{
            *Yaxis-=FastMovementSpeed;
        }
    }
    if(IsKeyDown(KEY_S)){
        if(IsKeyDown(KEY_LEFT_SHIFT)){
            *Yaxis+=SlowMovementSpeed;
        }else{
            *Yaxis+=FastMovementSpeed;
        }
    }
    // X AXIS
    if(IsKeyDown(KEY_D)){
        if(IsKeyDown(KEY_LEFT_SHIFT)){
            *Xaxis+=SlowMovementSpeed;
        }else{
            *Xaxis+=FastMovementSpeed;
        }
    }
    if(IsKeyDown(KEY_A)){
        if(IsKeyDown(KEY_LEFT_SHIFT)){
            *Xaxis-=SlowMovementSpeed;
        }else{
            *Xaxis-=FastMovementSpeed;
        }
    }
    // X AXIS BOUNDARIES
    if((*Xaxis + CircleRadius) >= (ScreenWidth-  1100 + 700)){
        *Xaxis = (ScreenWidth-  1100 + 700) - CircleRadius;
    }
    if(*Xaxis <= CircleRadius + (ScreenWidth - 1100)){
        *Xaxis = CircleRadius + (ScreenWidth - 1100);
    }
    // Y AXIS BOUNDARIES
    if((*Yaxis + CircleRadius) >= ScreenHeight-10){
        *Yaxis = ScreenHeight - CircleRadius - 10;
    }
    if(*Yaxis <= CircleRadius+10){
        *Yaxis = CircleRadius+10;
    }
}

void ScoreBoard(long int *Score,int ScreenWidth,int ScreenHeight, float DeltaTime){
    DrawText(TextFormat("SCORE: %ld", *Score), (ScreenWidth - 300), (300), 30, WHITE);
    if(DeltaTime >= 1.5)
      *Score += 1;
}

void DrawMainMenu(int ScreenWidth,int ScreenHeight){
  DrawText("BULLET HELL",(ScreenWidth/2)-350,200,100,BLACK);
  DrawRectangle((ScreenWidth/2)-150,(ScreenHeight/2)-100,300,100,WHITE);
  DrawRectangleLines((ScreenWidth/2)-150,(ScreenHeight/2)-100,300,100,RED);
  DrawText("PLAY",(ScreenWidth/2)-100,(ScreenHeight/2)-90,80,RED);
  DrawRectangle((ScreenWidth/2)-150,(ScreenHeight/2)+80,300,100,WHITE);
  DrawRectangleLines((ScreenWidth/2)-150,(ScreenHeight/2)+80,300,100,RED);
  DrawText("QUIT",(ScreenWidth/2)-100,(ScreenHeight/2)+90,80,RED);
}

int MouseClickingLogic(){
  int MouseX = GetMouseX();
  int MouseY = GetMouseY();
  if(IsMouseButtonPressed(0)){
    // Press Play
    if(MouseX>490 && MouseX <790 && MouseY > 360 && MouseY < 460){
      EndDrawing();
      return 1;
    }
    //Press Quit
    if(MouseX>490 && MouseX <790 && MouseY > 540 && MouseY < 640){
      EndDrawing();
      return 0;
    }
  }
  // Do none of the above
  return 3;
}

void SpawnBullet(float spawnCooldown, float *spawnCooldownTimer, Bullets *bullets, int *bulletCount, int ScreenWidth, int ScreenHeight){
  *spawnCooldownTimer = spawnCooldown;
  if(*bulletCount < MAXBULLETS){
    bullets[*bulletCount].disabled = false;
    bullets[*bulletCount].position = (Vector2){(float)(rand()%((ScreenWidth-  1100 + 700) - (ScreenWidth - 1100) + 1) + (ScreenWidth - 1100)), 0.0f};
    bullets[*bulletCount].color = BLUE;
    bullets[*bulletCount].Acceleration = (Vector2){0,20};
    (*bulletCount)++;
  }
}

void DisableBullets(int *bulletCount, Bullets *bullets, int ScreenHeight){
  for(int i = 0; i < *bulletCount;i++){
    if(!bullets[i].disabled){
      bullets[i].position.y += bullets[i].Acceleration.y;
    }
    if(bullets[i].position.y > ScreenHeight || bullets[i].position.y < 0){
      bullets[i].disabled = true;
    }
  }
}

int DeadScreenDraw(long int *Score, int ScreenWidth, int ScreenHeight, int *GameState, int *bulletCount){
  ClearBackground(RED);
  DrawText(TextFormat("GAME OVER\nPress Space to go to main menu\nor enter to continue"),(ScreenWidth/2) - 30 * 5,ScreenHeight/2,30,BLACK);
  DrawText(TextFormat("Your Score: %ld", *Score), (ScreenWidth - 300), (300), 30, WHITE);
  if(IsKeyDown(KEY_SPACE)){
    *bulletCount = 0;
    *Score = 0;
    *GameState = 0;
    return 0;
  }
  if(IsKeyDown(KEY_ENTER)){
    *bulletCount = 0;
    *Score = 0;
    *GameState = 1;
    return 1;
  }
  return -1;
}


int main(){
    srand(time(NULL));
    // SCREEN SETTINGS
        const int ScreenWidth = 1280;
        const int ScreenHeight = 920;
        InitWindow(ScreenWidth,ScreenHeight,"Bullet hell");
        Texture2D background = LoadTexture("Assets/Mokou1.png");
    // PLAYER
        Player player = {640,460};
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
        int GameState = 0;
    SetTargetFPS(60);
    start:
    if(GameState == 0){
        while(!WindowShouldClose()){
          // Mouse position
          int checker = MouseClickingLogic();
          if(checker == 1){
            GameState = 1;
            goto mainloop;
          }
          if(checker == 0)
            goto ending;
          BeginDrawing();
              int *Selection=malloc(sizeof(int));
              ClearBackground(BLACK);
              DrawTexture(background,0,0,WHITE);
              DrawMainMenu(ScreenWidth,ScreenHeight);
              free(Selection);
          EndDrawing();
        }
    }
    mainloop:
    if(GameState == 1){
      DeltaTime = 0;
        while(!WindowShouldClose()){
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
                      EndDrawing();
                      GameState = 2;
                      player.MovementCircleX = 640;
                      player.MovementCircleY = 460;
                      goto failed;
                    }
                }
                DrawRectangleLines((ScreenWidth)-1100,10,700,ScreenHeight-20,WHITE);
            EndDrawing();
            DeltaTime += GetFrameTime();
        };
    }
    failed:
    if(GameState == 2){
        while(!WindowShouldClose()){
            BeginDrawing();
              int checker = DeadScreenDraw(&Score, ScreenWidth, ScreenHeight, &GameState, &bulletCount);
              if(checker == 0)
                goto start;
              if(checker == 1)
                goto mainloop;
            EndDrawing();
        };
    }
    ending:
    UnloadTexture(background);
    CloseWindow();
    free(bullets);
    return 0;
}
