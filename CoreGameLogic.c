#include "raylib.h"
#include "CoreGameLogic.h"
#include <time.h> // For randomness
#include <stdlib.h>

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
    if((*Xaxis + CircleRadius) >= ScreenWidth/6 + ScreenWidth/3*1.5){
        *Xaxis = ScreenWidth/6 + ScreenWidth/3*1.5 - CircleRadius;
    }
    if(*Xaxis <= CircleRadius + ScreenWidth/6){
        *Xaxis = CircleRadius + ScreenWidth/6;
    }
    // Y AXIS BOUNDARIES
    if((*Yaxis + CircleRadius) >= ScreenHeight - 10){
        *Yaxis = ScreenHeight - CircleRadius - 10;
    }
    if(*Yaxis <= CircleRadius + 10){
        *Yaxis = CircleRadius + 10;
    }
}

void ScoreBoard(long int *Score,int ScreenWidth,int ScreenHeight, float DeltaTime){
    DrawText(TextFormat("SCORE: %ld", *Score), (ScreenWidth - 300), (300), 30, WHITE);
    if(DeltaTime >= 1.5)
      *Score += 1;
}

void DrawMainMenu(int ScreenWidth,int ScreenHeight){
  // TITLE
  DrawText("BULLET HELL",(ScreenWidth/2)-350,(ScreenHeight/2)-100-200,100,BLACK);
  // PLAY BUTTON
  DrawRectangle((ScreenWidth/2)-150,(ScreenHeight/2)-90,300,100,WHITE);
  DrawRectangleLines((ScreenWidth/2)-150,(ScreenHeight/2)-90,300,100,RED);
  DrawText("PLAY",(ScreenWidth/2)-100,(ScreenHeight/2)-80,80,RED);
  //SETTINGS BUTTON
  DrawRectangle((ScreenWidth/2)-150,(ScreenHeight/2)+70,300,100,WHITE);
  DrawRectangleLines((ScreenWidth/2)-150,(ScreenHeight/2)+70,300,100,RED);
  DrawText("SETTINGS",(ScreenWidth/2)-135,(ScreenHeight/2)+90,50,RED);
  // QUIT BUTTON
  DrawRectangle((ScreenWidth/2)-150,(ScreenHeight/2)+210,300,100,WHITE);
  DrawRectangleLines((ScreenWidth/2)-150,(ScreenHeight/2)+210,300,100,RED);
  DrawText("QUIT",(ScreenWidth/2)-100,(ScreenHeight/2)+220,80,RED);
}

int MouseClickingLogic(int ScreenWidth, int ScreenHeight){
  int MouseX = GetMouseX();
  int MouseY = GetMouseY();
  if(IsMouseButtonPressed(0)){
    // Press Play
    if(MouseX>((ScreenWidth/2)-150) && MouseX <((ScreenWidth/2)-150+300) && MouseY > ((ScreenHeight/2)-100) && MouseY < (ScreenHeight/2)){
      return 1;
    }
    //Press Quit
    if(MouseX>(ScreenWidth/2)-150 && MouseX <(ScreenWidth/2)-150+300 && MouseY > (ScreenHeight/2)+210 && MouseY < (ScreenHeight/2)+210+100){
      return 0;
    }
    //Press Settings
    if(MouseX>(ScreenWidth/2)-150 && MouseX <(ScreenWidth/2)-150+300 && MouseY > (ScreenHeight/2)+70 && MouseY < (ScreenHeight/2)+70+100){
      return 2;
    }
  }
  // Do none of the above
  return 3;
}

void SpawnBullet(float spawnCooldown, float *spawnCooldownTimer, Bullets *bullets, int *bulletCount, int ScreenWidth, int ScreenHeight){
  *spawnCooldownTimer = spawnCooldown;
  if(*bulletCount < MAXBULLETS){
    bullets[*bulletCount].disabled = false;
    bullets[*bulletCount].position = (Vector2){(float)(rand()%((ScreenWidth/6 + ScreenWidth/2 - 10) - (ScreenWidth/6) + 1) + (ScreenWidth/6)), 0.0f};
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
void DrawVideoMenu(float startX, float startY, Color cTextPrimary, Color cTextSecondary, Color cDropdownBg) {
    float labelWidth = 120;
    float controlX = startX + labelWidth;
    float rowSpacing = 35;

    DrawText("WINDOW", startX, startY, 16, cTextPrimary);

    DrawText("Resolution", startX, startY + 35, 14, cTextSecondary);
    DrawRectangleRounded((Rectangle){ controlX, startY + 30, 150, 24 }, 0.3f, 5, cDropdownBg);
    DrawText("1920x1080", controlX + 8, startY + 34, 14, cTextSecondary);
    DrawTriangle(
        (Vector2){ controlX + 150 - 15, startY + 40 },
        (Vector2){ controlX + 150 - 10, startY + 45 },
        (Vector2){ controlX + 150 - 5, startY + 40 },
        cTextSecondary
    );

    DrawText("Aspect ratio", startX, startY + 35 + rowSpacing, 14, cTextSecondary);
    DrawRectangleRounded((Rectangle){ controlX, startY + 30 + rowSpacing, 80, 24 }, 0.3f, 5, cDropdownBg);
    DrawText("16:9", controlX + 8, startY + 34 + rowSpacing, 14, cTextSecondary);
    DrawTriangle(
        (Vector2){ controlX + 80 - 15, startY + 40 + rowSpacing },
        (Vector2){ controlX + 80 - 10, startY + 45 + rowSpacing },
        (Vector2){ controlX + 80 - 5, startY + 40 + rowSpacing },
        cTextSecondary
    );

    DrawText("Display mode", startX, startY + 35 + rowSpacing*2, 14, cTextSecondary);
    DrawRectangleRounded((Rectangle){ controlX, startY + 30 + rowSpacing*2, 120, 24 }, 0.3f, 5, cDropdownBg);
    DrawText("Fullscreen", controlX + 8, startY + 34 + rowSpacing*2, 14, cTextSecondary);
    DrawTriangle(
        (Vector2){ controlX + 120 - 15, startY + 40 + rowSpacing*2 },
        (Vector2){ controlX + 120 - 10, startY + 45 + rowSpacing*2 },
        (Vector2){ controlX + 120 - 5, startY + 40 + rowSpacing*2 },
        cTextSecondary
    );

    float generalY = startY + 160;
    DrawText("GENERAL", startX, generalY, 16, cTextPrimary);

    DrawText("Anti alias", startX, generalY + 35, 14, cTextSecondary);
    DrawRectangleRounded((Rectangle){ controlX, generalY + 30, 80, 24 }, 0.3f, 5, cDropdownBg);
    DrawText("TAA", controlX + 8, generalY + 34, 14, cTextSecondary);
    DrawTriangle(
        (Vector2){ controlX + 80 - 15, generalY + 40 },
        (Vector2){ controlX + 80 - 10, generalY + 45 },
        (Vector2){ controlX + 80 - 5, generalY + 40 },
        cTextSecondary
    );

    DrawText("Render quality", startX, generalY + 35 + rowSpacing, 14, cTextSecondary);
    DrawRectangleRounded((Rectangle){ controlX, generalY + 30 + rowSpacing, 80, 24 }, 0.3f, 5, cDropdownBg);
    DrawText("High", controlX + 8, generalY + 34 + rowSpacing, 14, cTextSecondary);
    DrawTriangle(
        (Vector2){ controlX + 80 - 15, generalY + 40 + rowSpacing },
        (Vector2){ controlX + 80 - 10, generalY + 45 + rowSpacing },
        (Vector2){ controlX + 80 - 5, generalY + 40 + rowSpacing },
        cTextSecondary
    );

    DrawText("Render detail", startX, generalY + 35 + rowSpacing*2, 14, cTextSecondary);
    DrawRectangleRounded((Rectangle){ controlX, generalY + 30 + rowSpacing*2, 80, 24 }, 0.3f, 5, cDropdownBg);
    DrawText("Custom", controlX + 8, generalY + 34 + rowSpacing*2, 14, cTextSecondary);
    DrawTriangle(
        (Vector2){ controlX + 80 - 15, generalY + 40 + rowSpacing*2 },
        (Vector2){ controlX + 80 - 10, generalY + 45 + rowSpacing*2 },
        (Vector2){ controlX + 80 - 5, generalY + 40 + rowSpacing*2 },
        cTextSecondary
    );
}

void DrawControlsMenu(float startX, float startY, Color cTextPrimary, Color cTextSecondary, Color cDropdownBg) {
    float rowSpacing = 45;
    float keyBoxWidth = 120;

    DrawText("CONTROLS", startX, startY, 16, cTextPrimary);

    const char* actions[] = { "Move", "Slow Move", "Shoot", "Use Bomb" };
    const char* keys[] = { "WASD", "L-SHIFT", "J", "K" };

    for (int i = 0; i < 4; i++) {
        float y = startY + 50 + (i * rowSpacing);

        DrawText(actions[i], startX, y + 5, 16, cTextSecondary);

        DrawRectangleRounded((Rectangle){ startX + 200, y, keyBoxWidth, 30 }, 0.3f, 5, cDropdownBg);
        int textWidth = MeasureText(keys[i], 14);
        DrawText(keys[i], startX + 200 + (keyBoxWidth - textWidth) / 2, y + 8, 14, cTextPrimary);
    }
}

void DrawAudioMenu(float startX, float startY, Color cTextPrimary, Color cTextSecondary, Color cSliderTrack, Color cSliderThumb) {
    float rowSpacing = 60;
    float labelWidth = 150;

    DrawText("AUDIO SETTINGS", startX, startY, 16, cTextPrimary);

    DrawText("Master Volume", startX, startY + 40, 16, cTextSecondary);
    DrawRectangleRounded((Rectangle){ startX + labelWidth, startY + 45, 200, 8 }, 1.0f, 5, cSliderTrack);
    DrawCircle(startX + labelWidth + (200 * 0.8f), startY + 49, 8, cSliderThumb);
    DrawText("80%", startX + labelWidth + 210, startY + 41, 14, cTextSecondary);

    DrawText("Music", startX, startY + 40 + rowSpacing, 16, cTextSecondary);
    DrawRectangleRounded((Rectangle){ startX + labelWidth, startY + 45 + rowSpacing, 200, 8 }, 1.0f, 5, cSliderTrack);
    DrawCircle(startX + labelWidth + (200 * 0.5f), startY + 49 + rowSpacing, 8, cSliderThumb);
    DrawText("50%", startX + labelWidth + 210, startY + 41 + rowSpacing, 14, cTextSecondary);

    DrawText("SFX", startX, startY + 40 + rowSpacing*2, 16, cTextSecondary);
    DrawRectangleRounded((Rectangle){ startX + labelWidth, startY + 45 + rowSpacing*2, 200, 8 }, 1.0f, 5, cSliderTrack);
    DrawCircle(startX + labelWidth + (200 * 0.9f), startY + 49 + rowSpacing*2, 8, cSliderThumb);
    DrawText("90%", startX + labelWidth + 210, startY + 41 + rowSpacing*2, 14, cTextSecondary);
}
void DrawSettingsMenu(int activeTab) {
    // Tokyo Night palette
    Color cPanel        = (Color){ 26, 27, 38, 230 };
    Color cTextPrimary  = (Color){ 192, 202, 245, 255 };
    Color cTextSecondary= (Color){ 169, 177, 214, 255 };
    Color cBtnNormal    = (Color){ 36, 40, 59, 200 };
    Color cBtnSelected  = (Color){ 122, 162, 247, 230 };
    Color cSliderTrack  = (Color){ 65, 72, 104, 255 };
    Color cSliderThumb  = (Color){ 125, 207, 255, 255 };
    Color cAccentBtn    = (Color){ 187, 154, 247, 230 };
    Color cDropdownBg   = (Color){ 22, 22, 30, 200 };

    const int VIRTUAL_WIDTH = 1000;
    const int VIRTUAL_HEIGHT = 700;

    static RenderTexture2D target = { 0 };
    if (target.id == 0) {
        target = LoadRenderTexture(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    }

    BeginTextureMode(target);
    ClearBackground(BLANK);

    float panelX = 30, panelY = 30;
    float panelW = VIRTUAL_WIDTH - 60, panelH = VIRTUAL_HEIGHT - 60;
    DrawRectangleRounded((Rectangle){ panelX, panelY, panelW, panelH }, 0.05f, 10, cPanel);
    DrawRectangleRoundedLinesEx((Rectangle){ panelX, panelY, panelW, panelH }, 0.05f, 10, 1.0f, (Color){ 86, 95, 137, 100 });

    DrawText("GAME SETTINGS", panelX + 30, panelY + 30, 24, cTextPrimary);

    // Sidebar
    float sideX = panelX + 40;
    float sideY = panelY + 100;
    float sideW = 180;
    float sideH = 35;
    float spacing = 15;

    const char* sideLabels[] = {"Video", "Audio", "Controls"};

    for (int i = 0; i < 3; i++) {
        bool isSelected = ((i + 1) == activeTab);
        Color bgColor = isSelected ? cBtnSelected : cBtnNormal;
        Color textColor = isSelected ? (Color){ 22, 22, 30, 255 } : cTextSecondary;
        Rectangle btnRec = { sideX, sideY + i * (sideH + spacing), sideW, sideH };

        DrawRectangleRounded(btnRec, 0.5f, 10, bgColor);

        int fontSize = 16;
        int textWidth = MeasureText(sideLabels[i], fontSize);
        DrawText(sideLabels[i], btnRec.x + (btnRec.width - textWidth) / 2, btnRec.y + (btnRec.height - fontSize) / 2 + 2,
                 fontSize, textColor);
    }

    float contentX = 320;
    float contentY = panelY + 100;

    if (activeTab == 1) {
        DrawVideoMenu(contentX, contentY, cTextPrimary, cTextSecondary, cDropdownBg);
    } else if (activeTab == 2) {
        DrawAudioMenu(contentX, contentY, cTextPrimary, cTextSecondary, cSliderTrack, cSliderThumb);
    } else if (activeTab == 3) {
        DrawControlsMenu(contentX, contentY, cTextPrimary, cTextSecondary, cDropdownBg);
    }

    // Bottom buttons
    float btnY = panelY + panelH - 60;

    DrawRectangleRounded((Rectangle){ panelX + panelW - 220, btnY, 90, 35 }, 0.4f, 10, cBtnNormal);
    int defTextW = MeasureText("DEFAULT", 14);
    DrawText("DEFAULT", (panelX + panelW - 220) + (90 - defTextW) / 2, btnY + 10, 14, cTextPrimary);

    DrawRectangleRounded((Rectangle){ panelX + panelW - 110, btnY, 70, 35 }, 0.4f, 10, cAccentBtn);
    int saveTextW = MeasureText("SAVE", 14);
    DrawText("SAVE", (panelX + panelW - 110) + (70 - saveTextW) / 2, btnY + 10, 14, (Color){ 22, 22, 30, 255 });

    EndTextureMode();

    ClearBackground(BLACK);

    DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){ 41, 46, 66, 255 }, (Color){ 17, 17, 27, 255 });

    float scale = fminf((float)GetScreenWidth() / VIRTUAL_WIDTH, (float)GetScreenHeight() / VIRTUAL_HEIGHT);
    float finalWidth = VIRTUAL_WIDTH * scale;
    float finalHeight = VIRTUAL_HEIGHT * scale;

    float posX = (GetScreenWidth() - finalWidth) / 2.0f;
    float posY = (GetScreenHeight() - finalHeight) / 2.0f;

    Rectangle sourceRec = { 0.0f, 0.0f, (float)target.texture.width, -(float)target.texture.height };
    Rectangle destRec = { posX, posY, finalWidth, finalHeight };

    DrawTexturePro(target.texture, sourceRec, destRec, (Vector2){ 0, 0 }, 0.0f, WHITE);
}
int SettingsMenuPicker(int ScreenWidth, int ScreenHeight){
    int MouseX = GetMouseX();
    int MouseY = GetMouseY();
    if(IsMouseButtonPressed(0)){
        // Press Video
        if(MouseX > (ScreenWidth*0.07f) && MouseX < ((ScreenWidth*0.07f)+(ScreenWidth*0.18f)) && MouseY > (ScreenHeight * 0.1857f) && MouseY < ((ScreenHeight * 0.1857f)+(ScreenHeight * 0.05f))){
            return 1;
        }
        //Press Audio
        if(MouseX > (ScreenWidth*0.07f) && MouseX < ((ScreenWidth*0.07f)+(ScreenWidth*0.18f)) && MouseY > (ScreenHeight * 0.2571f) && MouseY < ((ScreenHeight * 0.2571f)+(ScreenHeight * 0.05f))){
            return 2;
        }
        //Press Controls
        if(MouseX > (ScreenWidth*0.07f) && MouseX < ((ScreenWidth*0.07f)+(ScreenWidth*0.18f)) && MouseY > (ScreenHeight * 0.3285f) && MouseY < ((ScreenHeight * 0.3285f)+(ScreenHeight * 0.05f))){
            return 3;
        }
    }
    return 0;
}
