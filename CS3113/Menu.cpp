#include "Menu.h"

Menu::Menu()               : Scene { {0.0f}   } {}
Menu::Menu(Vector2 origin) : Scene { origin } {}

Menu::~Menu() { shutdown(); }

void Menu::initialise()
{
   mGameState.bgm = LoadMusicStream("assets/game/soundtrack.mp3");
   mGameState.nextSceneID = 0;
   SetMusicVolume(mGameState.bgm, 0.33f);
   PlayMusicStream(mGameState.bgm);

   /*
      ----------- CAMERA -----------
   */
   mGameState.camera = { 0 };                                    // zero initialize
   mGameState.camera.target = mOrigin;
   mGameState.camera.offset = mOrigin;                           // camera offset to center of screen
   mGameState.camera.rotation = 0.0f;                            // no rotation
   mGameState.camera.zoom = 1.0f;                                // default zoom
}

void Menu::update(float deltaTime)
{   
   UpdateMusicStream(mGameState.bgm);
}

void Menu::render()
{  
   DrawText(MENU_TEXT, mOrigin.x-OFFSET, mOrigin.y-200, 50, WHITE);
}

void Menu::shutdown()
{   
   UnloadMusicStream(mGameState.bgm);
}