#include "Scene.h"

class LevelB : public Scene {
public:
    int ENEMY_COUNT = 1;
    
    ~LevelB();
    
    int player_lives;
    void initialise(int lives);
    void initialise(int temp, glm::vec3 player_pos);
    void update(float delta_time);
    void render(ShaderProgram *program) override;
};
