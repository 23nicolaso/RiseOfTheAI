#include "Scene.h"

Scene::Scene() : mOrigin{{}} {}

Scene::Scene(Vector2 origin) : mOrigin{origin} 
{
    ClearBackground(BLACK);
}