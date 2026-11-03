
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

void DrawMainMenu(int ScreenWidth,int ScreenHeight){
  ClearBackground(BLACK);
  DrawText("BULLET HELL",(ScreenWidth/2)-350,200,100,WHITE);
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

int DeadScreenDraw(long int *Score, int ScreenWidth, int ScreenHeight, int *GameState){
  ClearBackground(RED);
  DrawText(TextFormat("GAME OVER\nPress Space to go to main menu\nor enter to continue"),(ScreenWidth/2) - 30 * 5,ScreenHeight/2,30,BLACK);
  DrawText(TextFormat("Your Score: %ld", *Score), (ScreenWidth - 300), (300), 30, WHITE);
  if(IsKeyDown(KEY_SPACE)){
    *Score = 0;
    *GameState = 0;
    return 0;
  }
  if(IsKeyDown(KEY_ENTER)){
    *Score = 0;
    *GameState = 1;
    return 1;
  }
  return -1;
}


