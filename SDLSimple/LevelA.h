#include "Scene.h"

class LevelA : public Scene {
public:
    // ————— STATIC ATTRIBUTES ————— //
    int ENEMY_COUNT = 4;
    
    // ————— DESTRUCTOR ————— //
    ~LevelA();
    
    // ————— METHODS ————— //
    int cur = 0;
    int dot_count = 0;
    
    void initialise(int lives) override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
    
    ShaderProgram level_a_shader_program;
};
