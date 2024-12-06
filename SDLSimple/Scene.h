#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Utility.h"
#include "Entity.h"
#include "Map.h"

struct GameState
{
    // ————— GAME OBJECTS ————— //
    Map *map;
    Entity *player;
    Entity *enemies;
    Entity *lives_count;
    // ————— AUDIO ————— //
    Mix_Music *bgm;
    Mix_Chunk *munch_sfx;
    Mix_Chunk *death_sfx;
    Mix_Chunk *eat_ghost_sfx;
    Mix_Chunk *powerup;

    
    // ————— POINTERS TO OTHER SCENES ————— //
    int next_scene_id;
};

class Scene {    
public:
    
    GameState m_game_state;

    float cur_time = 0;

    // ————— ATTRIBUTES ————— //
    int m_number_of_enemies = 4;
    
    // ————— METHODS ————— //
    virtual void initialise(int lives) = 0;
    virtual void update(float delta_time) = 0;
    virtual void render(ShaderProgram *program) = 0;
    
    // ————— GETTERS ————— //
    GameState const get_state() const { return m_game_state;  }
};
