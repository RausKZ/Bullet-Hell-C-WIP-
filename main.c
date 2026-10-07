#include "raylib.h"
#include <time.h> // For randomness
#include <stdlib.h>
#define MAXBULLETS 500000

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
    if((*Yaxis + CircleRadius) >= ScreenHeight){
        *Yaxis = ScreenHeight - CircleRadius;
    }
    if(*Yaxis <= CircleRadius){
        *Yaxis = CircleRadius;
    }
}

void ScoreBoard(long int *Score,int ScreenWidth,int ScreenHeight){
    DrawText(TextFormat("SCORE: %ld", *Score), (ScreenWidth - 300), (300), 30, WHITE);
    *Score += 1;
}



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

int main(){
    srand(time(NULL));
    // SCREEN SETTINGS
        const int ScreenWidth = 1280;
        const int ScreenHeight = 920;
    InitWindow(ScreenWidth,ScreenHeight,"Bullet hell");
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
            int MouseX = GetMouseX();
            int MouseY = GetMouseY();
            if(IsMouseButtonPressed(0)){
              if(MouseX>490 && MouseX <790 && MouseY > 360 && MouseY < 460){
                GameState = 1;
                EndDrawing();
                goto mainloop;
              }
              if(MouseX>490 && MouseX <790 && MouseY > 540 && MouseY < 640){
                EndDrawing();
                goto ending;
              }
            }
            BeginDrawing();
                int *Selection=malloc(sizeof(int));
                ClearBackground(BLACK);
                DrawText("BULLET HELL",(ScreenWidth/2)-350,200,100,WHITE);
                DrawRectangle((ScreenWidth/2)-150,(ScreenHeight/2)-100,300,100,WHITE);
                DrawRectangleLines((ScreenWidth/2)-150,(ScreenHeight/2)-100,300,100,RED);
                DrawText("PLAY",(ScreenWidth/2)-100,(ScreenHeight/2)-90,80,RED);
                DrawRectangle((ScreenWidth/2)-150,(ScreenHeight/2)+80,300,100,WHITE);
                DrawRectangleLines((ScreenWidth/2)-150,(ScreenHeight/2)+80,300,100,RED);
                DrawText("QUIT",(ScreenWidth/2)-100,(ScreenHeight/2)+90,80,RED);
            EndDrawing();
        }
    }
    mainloop:
    if(GameState == 1){
        while(!WindowShouldClose()){
            if(bulletCount >= MAXBULLETS){
                bulletCount = 0;
                bulletDisabledCount = 0;
            }
            spawnCooldownTimer--;
            if(spawnCooldownTimer < 0){
                spawnCooldownTimer = spawnCooldown;
                if(bulletCount < MAXBULLETS){
                        bullets[bulletCount].disabled = false;
                        bullets[bulletCount].position = (Vector2){(float)(rand()%((ScreenWidth-  1100 + 700) - (ScreenWidth - 1100) + 1) + (ScreenWidth - 1100)), 0.0f};
                        bullets[bulletCount].color = BLUE;
                        bullets[bulletCount].Acceleration = (Vector2){0,20};
                        bulletCount++;
                }
            }
            for(int i = 0; i < bulletCount;i++){
                if(!bullets[i].disabled){
                    bullets[i].position.y += bullets[i].Acceleration.y;
                }
                if(bullets[i].position.y > ScreenHeight || bullets[i].position.y < 0){
                    bullets[i].disabled = true;
                }
            }

            CircleMovement(&player.MovementCircleX,&player.MovementCircleY,ScreenWidth,ScreenHeight,CircleRadius);
            Vector2 Player = {player.MovementCircleX,player.MovementCircleY};

            BeginDrawing();
                ClearBackground(BLACK);
                DrawCircle(player.MovementCircleX,player.MovementCircleY,CircleRadius,WHITE);
                DrawCircleLines(player.MovementCircleX,player.MovementCircleY,CircleRadius,RED);
                ScoreBoard(&Score,ScreenWidth,ScreenHeight);
                    for(int i = 0; i<bulletCount; i++){
                        if(!bullets[i].disabled)
                            DrawRectangleV(bullets[i].position,Size,BLUE);
                            Rectangle bullet1 = {bullets[i].position.x, bullets[i].position.y, Size.x, Size.y};
                            if(CheckCollisionCircleRec(Player, CircleRadius, bullet1)){
                                EndDrawing();
                                GameState = 2;
                                goto failed;
                            }
                }
                DrawRectangleLines((ScreenWidth)-1100,0,700,ScreenHeight,WHITE);
            EndDrawing();
        };
    }
    failed:
    if(GameState == 2){
        while(!WindowShouldClose()){
            BeginDrawing();
                ClearBackground(RED);
                DrawText(TextFormat("GAME OVER\nPress Space to go to main menu\nor enter to continue"),(ScreenWidth/2) - 30 * 5,ScreenHeight/2,30,BLACK);
                DrawText(TextFormat("Your Score: %ld", Score), (ScreenWidth - 300), (300), 30, WHITE);
                if(IsKeyDown(KEY_SPACE)){
                    Score = 0;
                    GameState = 0;
                    goto start;
                }
                if(IsKeyDown(KEY_ENTER)){
                    Score = 0;
                    GameState = 1;
                    goto mainloop;
                }
            EndDrawing();
        };
    }
    ending:
    CloseWindow();
    free(bullets);
    return 0;
}
