#include "Scene.h"

#ifndef Menu_H
#define Menu_H

class Menu : public Scene {
public:
    static constexpr float TILE_DIMENSION       = 75.0f,
                        ACCELERATION_OF_GRAVITY = 981.0f,
                        END_GAME_THRESHOLD      = 800.0f;
    static constexpr int   OFFSET               = 300.0f;

    const char * const MENU_TEXT = "Return of The AI\nPress enter to start!";

    Menu();
    Menu(Vector2 origin);
    ~Menu();
    
    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif