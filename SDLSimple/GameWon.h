#include "Scene.h"

class GameWon : public Scene {
public:
    // ————— STATIC ATTRIBUTES ————— //

    // ————— DESTRUCTOR ————— //
    ~GameWon();
    
    // ————— METHODS ————— //
    void initialise(int lives) override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
