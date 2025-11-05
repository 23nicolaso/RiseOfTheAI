#include "LevelC.h"

LevelC::LevelC()                                      : Scene { {0.0f} } {}
LevelC::LevelC(Vector2 origin) : Scene { origin } {}

LevelC::~LevelC() { shutdown(); }

void LevelC::initialise()
{
   mGameState.nextSceneID = 0;

   mGameState.bgm = LoadMusicStream("assets/game/soundtrack.mp3");
   SetMusicVolume(mGameState.bgm, 0.33f);
   PlayMusicStream(mGameState.bgm);

   mGameState.deathSound       = LoadSound("assets/game/death.wav");
   mGameState.jumpSound         = LoadSound("assets/game/jump.wav");
   mGameState.sceneChangeSound = LoadSound("assets/game/change.mp3");

   /*
      ----------- MAP -----------
   */
   mGameState.map = new Map(
      LEVEL_WIDTH, LEVEL_HEIGHT,   // map grid cols & rows
      (unsigned int *) mLevelData, // grid data
      "assets/game/tileset.png",   // texture filepath
      TILE_DIMENSION,              // tile size
      4, 1,                      // texture cols & rows
      mOrigin                      // in-game origin
   );

   /*
      ----------- PROTAGONIST -----------
   */
   std::map<Direction, std::vector<int>> soldierAnimationAtlas = {
      {RIGHT,  { 4,5,6,7 }},
      {UP,    { 0,1,2,3 }},
      {LEFT, { 8,9,10,11 }},
   };

   float sizeRatio  = 1.0f;

   mGameState.soldier = new Entity(
      {mGameState.map->getLeftBoundary()+50, mOrigin.y}, // position
      {70.0f, 70.0f},             // scale
      "assets/game/Soldier.png",                   // texture file address
      ATLAS,                                    // single image or atlas?
      { 4, 4 },                                 // atlas dimensions
      soldierAnimationAtlas,                    // actual atlas
      PLAYER                                    // entity type
   );

   mGameState.soldier->setJumpingPower(550.0f);
   mGameState.soldier->setColliderDimensions({
      mGameState.soldier->getScale().x,
      mGameState.soldier->getScale().y
   });
   mGameState.soldier->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
   mGameState.soldier->setFrameSpeed(4);

   /*
      ----------- CAMERA -----------
   */
   mGameState.camera = { 0 };                                    // zero initialize
   mGameState.camera.target = mGameState.soldier->getPosition(); // camera follows player
   mGameState.camera.offset = mOrigin;                           // camera offset to center of screen
   mGameState.camera.rotation = 0.0f;                            // no rotation
   mGameState.camera.zoom = 1.0f;                                // default zoom
}

void LevelC::update(float deltaTime)
{
   UpdateMusicStream(mGameState.bgm);

   mGameState.soldier->update(
      deltaTime,      // delta time / fixed timestep
      nullptr,        // player
      mGameState.map, // map
      nullptr,        // collidable entities
      0               // col. entity count
   );

   if (mGameState.soldier->getClearStatus() == true) mGameState.nextSceneID = 3;

   Vector2 currentPlayerPosition = { mGameState.soldier->getPosition().x, mOrigin.y };

   panCamera(&mGameState.camera, &currentPlayerPosition);
}

void LevelC::render()
{
   ClearBackground(BLACK);
   mGameState.soldier->render();
   mGameState.map->render();
}

void LevelC::shutdown()
{
   delete mGameState.soldier;
   delete mGameState.map;

   UnloadMusicStream(mGameState.bgm);
   UnloadSound(mGameState.deathSound);
}