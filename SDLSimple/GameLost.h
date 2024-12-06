#include "Scene.h"

class GameLost : public Scene {
public:
    // ————— STATIC ATTRIBUTES ————— //

    // ————— DESTRUCTOR ————— //
    ~GameLost();
    
    // ————— METHODS ————— //
    void initialise(int lives) override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
