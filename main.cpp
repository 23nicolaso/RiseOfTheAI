/**
* Author: Nicolas Ollivier
* Assignment: Rise of the AI
* Date due: 2025-11-08, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/Menu.h"
#include "CS3113/LevelA.h"
#include "CS3113/LevelB.h"
#include "CS3113/LevelC.h"

// Global Constants
constexpr int SCREEN_WIDTH     = 1000,
              SCREEN_HEIGHT    = 600,
              FPS              = 120,
              NUMBER_OF_LEVELS = 3;

constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
            
constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

// Global Variables
AppStatus gAppStatus   = RUNNING;
float gPreviousTicks   = 0.0f,
      gTimeAccumulator = 0.0f;
int   gLives           = 3;

Scene *gCurrentScene = nullptr;
std::vector<Scene*> gLevels = {};

Menu   *gMenu   = nullptr;
LevelA *gLevelA = nullptr;
LevelB *gLevelB = nullptr;
LevelC *gLevelC = nullptr;

// Function Declarations
void switchToScene(Scene *scene);
void initialise();
void processInput();
void update();
void render();
void shutdown();

void switchToScene(Scene *scene)
{   
    if (gCurrentScene == scene){
        // HANDLE GAME WIN HERE
        ClearBackground(BLACK);
        DrawText("YOU WON :D", ORIGIN.x, ORIGIN.y-100, 50, GREEN);
        switchToScene(gLevels[0]);
    }
    else {
        gCurrentScene = scene;
        gCurrentScene->initialise();
    }
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Scenes");
    InitAudioDevice();

    gMenu   = new Menu(ORIGIN);
    gLevelA = new LevelA(ORIGIN);
    gLevelB = new LevelB(ORIGIN);
    gLevelC = new LevelC(ORIGIN);

    gLevels.push_back(gMenu);
    gLevels.push_back(gLevelA);
    gLevels.push_back(gLevelB);
    gLevels.push_back(gLevelC);

    switchToScene(gLevels[0]);

    SetTargetFPS(FPS);
}

void processInput() 
{
    if (gCurrentScene == gMenu){
        // Only check for ENTER in menu
        if (IsKeyDown(KEY_ENTER)){
            switchToScene(gLevels[1]);
            gLives = 3;
            PlaySound(gCurrentScene->getState().sceneChangeSound);
        }
    }
    else{
        gCurrentScene->getState().soldier->resetMovement();

        if      (IsKeyDown(KEY_A)) gCurrentScene->getState().soldier->moveLeft();
        else if (IsKeyDown(KEY_D)) gCurrentScene->getState().soldier->moveRight();

        if (IsKeyPressed(KEY_W) && 
            gCurrentScene->getState().soldier->isCollidingBottom())
        {
            PlaySound(gCurrentScene->getState().jumpSound);
            gCurrentScene->getState().soldier->jump();
        }

        if (GetLength(gCurrentScene->getState().soldier->getMovement()) > 1.0f) 
            gCurrentScene->getState().soldier->normaliseMovement();
        
        if (IsKeyPressed(KEY_RIGHT_BRACKET)){
            // A little debug tool ignore this!
            printf("%f\n", gCurrentScene->getState().soldier->getPosition().x - 
            gCurrentScene->getState().map->getLeftBoundary());
        }
        if (IsKeyPressed(KEY_LEFT_BRACKET)){
            // A little debug tool ignore this!
            printf("%f\n", gCurrentScene->getState().soldier->getPosition().y);
        }
    }
    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() 
{
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        gCurrentScene->update(FIXED_TIMESTEP);
        if (gCurrentScene->getState().soldier->getPosition().y > 800.0f){
            gLives -= 1;
            PlaySound(gCurrentScene->getState().deathSound);
            if (gLives <= 0){
                // HANDLE A GAME LOSS HERE
                ClearBackground(BLACK);
                DrawText("YOU LOST :(", ORIGIN.x, ORIGIN.y-100, 50, RED);
                switchToScene(gLevels[0]);
            }
            else{
                gCurrentScene -> initialise();
            }
        }
        deltaTime -= FIXED_TIMESTEP;
    }
}

void render()
{
    BeginDrawing();
    BeginMode2D(gCurrentScene->getState().camera);

    gCurrentScene->render();

    EndMode2D();

    if (gCurrentScene != gMenu){
        DrawText(TextFormat("Lives: %i", gLives), 100, 80, 20, RED);
    }
    
    EndDrawing();
}

void shutdown() 
{
    delete gMenu;
    delete gLevelA;
    delete gLevelB;
    delete gLevelC;

    for (int i = 0; i < NUMBER_OF_LEVELS; i++) gLevels[i] = nullptr;

    CloseAudioDevice();
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();

        if (gCurrentScene->getState().nextSceneID > 0)
        {
            int id = gCurrentScene->getState().nextSceneID;
            switchToScene(gLevels[id]);
            PlaySound(gCurrentScene->getState().sceneChangeSound);
        }

        render();
    }

    shutdown();

    return 0;
}