#include "Scene.h"

class MenuScreen : public Scene {
public:

    ~MenuScreen();

    void initialise(int lives) override;
    void update(float delta_time) override;
    void render(ShaderProgram* program) override;
};
