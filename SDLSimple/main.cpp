/**
* Author: [Amy Li]
* Assignment: [Pacman]
* Date due: [Your presentation date], 2:00pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1
#define FIXED_TIMESTEP 0.0166666f

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "cmath"
#include <ctime>
#include <vector>
#include "Entity.h"
#include "Map.h"
#include "Utility.h"
#include "Scene.h"
#include "Menu.h"
#include "LevelA.h"
#include "LevelB.h"
#include "GameWon.h"
#include "GameLost.h"

#include <cstdlib>

// ––––– CONSTANTS ––––– //
constexpr int WINDOW_WIDTH  = 640*2,
          WINDOW_HEIGHT = 480*2;

constexpr int VIEWPORT_X = 0,
          VIEWPORT_Y = 0,
          VIEWPORT_WIDTH  = WINDOW_WIDTH,
          VIEWPORT_HEIGHT = WINDOW_HEIGHT;

constexpr char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
           F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

constexpr float MILLISECONDS_IN_SECOND = 1000.0;

enum AppStatus { RUNNING, TERMINATED };

Scene  *g_current_scene;
MenuScreen *g_menu;
LevelA *g_levelA;
LevelB *g_levelB;
GameWon *g_won_page;
GameLost *g_lose_page;

Scene *g_levels[5];

int g_total_lives_left = 3;

SDL_Window* g_display_window;

ShaderProgram g_shader_program;
glm::mat4 g_view_matrix, g_projection_matrix;

float g_previous_ticks = 0.0f;
float g_accumulator = 0.0f;

bool g_is_colliding_bottom = false;

AppStatus g_app_status = RUNNING;

void switch_to_scene(Scene *scene);
void initialise();
void process_input();
void update();
void render();
void shutdown();

void switch_to_scene(Scene *scene)
{
    g_current_scene = scene;
    if (g_current_scene == g_levelB)
    {
        g_total_lives_left -= 1;
        g_levelB->initialise(g_total_lives_left, g_levelA->m_game_state.player->get_position());
    }
    else
    {
        g_current_scene->initialise(g_total_lives_left);
    }
}

void initialise()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    g_display_window = SDL_CreateWindow("PACMAN!",
                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                      WINDOW_WIDTH, WINDOW_HEIGHT,
                                      SDL_WINDOW_OPENGL);
    
    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    
    g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH);
    
    g_view_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-15.5f, 15.5f, -12.5f, 12.5f, -1.0f, 1.0f);
    g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-13.0f, 10.5f, 0.0f));


    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);
    
    glUseProgram(g_shader_program.get_program_id());
    
    glClearColor(0, 0, 0, 1);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    g_menu = new MenuScreen();
    g_levelA = new LevelA();
    g_levelB = new LevelB();
    g_won_page = new GameWon();
    g_lose_page = new GameLost();
    
    g_levels[0] = g_menu;
    g_levels[1] = g_levelA;
    g_levels[2] = g_levelB;
    g_levels[3] = g_won_page;
    g_levels[4] = g_lose_page;
    
    switch_to_scene(g_levels[0]);
}

void process_input()
{
    // VERY IMPORTANT: If nothing is pressed, we don't want to go anywhere
    g_current_scene->get_state().player->set_movement(glm::vec3(0.0f));
    
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            // End game
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                g_app_status = TERMINATED;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_q:
                        // Quit the game with a keystroke
                        g_app_status = TERMINATED;
                        break;
                        
                    case SDLK_RETURN:
                        if (g_current_scene == g_menu)
                        {
                            g_current_scene->update(-1);
                        }
                        break;
                        
                    default:
                        break;
                }
            default:
                break;
                        }
    }
    
    const Uint8 *key_state = SDL_GetKeyboardState(NULL);
    
    if (g_total_lives_left != 0)
    {
        if (g_current_scene != g_levelB && g_current_scene != g_menu)
        {
            if (key_state[SDL_SCANCODE_LEFT])
            {
                g_current_scene->m_game_state.player->move_left();
                g_current_scene->m_game_state.player->m_animation_indices = g_current_scene->m_game_state.player->m_walking[LEFT];
            }
            else if (key_state[SDL_SCANCODE_RIGHT])
            {
                g_current_scene->m_game_state.player->move_right();
                g_current_scene->m_game_state.player->m_animation_indices = g_current_scene->m_game_state.player->m_walking[RIGHT];
            }
            else if (key_state[SDL_SCANCODE_UP])
            {
                g_current_scene->m_game_state.player->move_up();
                g_current_scene->m_game_state.player->m_animation_indices = g_current_scene->m_game_state.player->m_walking[UP];
            }
            else if (key_state[SDL_SCANCODE_DOWN])
            {
                g_current_scene->m_game_state.player->move_down();
                g_current_scene->m_game_state.player->m_animation_indices = g_current_scene->m_game_state.player->m_walking[DOWN];
            }
        }
        
        
        if (glm::length(g_current_scene->m_game_state.player->m_movement) > 1.0f)
        {
            g_current_scene->m_game_state.player->normalise_movement();
        }

    }
}

void update()
{
    float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
    float delta_time = ticks - g_previous_ticks;
    g_previous_ticks = ticks;
    
    delta_time += g_accumulator;
    
    if (delta_time < FIXED_TIMESTEP)
    {
        g_accumulator = delta_time;
        return;
    }
    
    while (delta_time >= FIXED_TIMESTEP)
    {
        g_current_scene->update(FIXED_TIMESTEP);
        //g_is_colliding_bottom = g_current_scene->get_state().player->get_collided_bottom();
        
        delta_time -= FIXED_TIMESTEP;
    }
    
    g_accumulator = delta_time;
    
    // Prevent the camera from showing anything outside of the "edge" of the level
    //g_view_matrix = glm::mat4(1.0f);

    if (g_current_scene->m_game_state.next_scene_id >= 0)
    {
        switch_to_scene(g_levels[g_current_scene->m_game_state.next_scene_id]);
    }
    
}

void render()
{
    g_shader_program.set_view_matrix(g_view_matrix);
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    g_current_scene->render(&g_shader_program);
        
    SDL_GL_SwapWindow(g_display_window);
    
}


void shutdown()
{
    SDL_Quit();
    delete g_menu;
    delete g_levelA;
    delete g_levelB;
    delete g_won_page;
    delete g_lose_page;
}

int main(int argc, char* argv[])
{
    initialise();
    
    std::srand(static_cast<unsigned>(std::time(0)));
    
    while (g_app_status == RUNNING)
    {
        process_input();
        update();
        render();
    }
    
    shutdown();
    return 0;
}
