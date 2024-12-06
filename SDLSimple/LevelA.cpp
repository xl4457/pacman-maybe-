#include "LevelA.h"
#include "Utility.h"

#define LEVEL_WIDTH 23
#define LEVEL_HEIGHT 23

constexpr char PLAYER_FILEPATH[]  = "assets/images/pacman.png",
            TILE_FILEPATH[]       = "assets/images/MazeTilesCopy.png",
            BLINKY_FILEPATH[]     = "assets/images/blinky.png",
            CLYDE_FILEPATH[]      = "assets/images/clyde.png",
            INKY_FILEPATH[]       = "assets/images/inky.png",
            PINKY_FILEPATH[]      = "assets/images/pinky.png",
            DOT_FILEPATH[]        = "assets/images/dotA.png",
            FRUIT_FILEPATH[]      = "assets/images/strawberry.png",
            DEAD_PAC_FILEPATH[]   = "assets/images/dead_pacman.png",
            DEAD_ENEMY_FILEPATH[] = "assets/images/dead_enemy.png";


unsigned int LEVEL_A_DATA[] =
{
    1,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,   10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  48,
    3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,
    3,   0,  23,  20,  20,  22,   0,  23,  20,  20,  22,   0,   23,  20,  20,  22,   0,  23,  20,  20,  22,   0,   2,
    3,   0,  27,  20,  20,  26,   0,  27,  20,  20,  26,   0,   27,  20,  20,  26,   0,  27,  20,  20,  26,   0,   2,
    3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,
    3,   0,  23,  20,  20,  22,   0,  23,  20,  20,  22,   0,   23,  20,  20,  22,   0,  23,  20,  20,  22,   0,   2,
    3,   0,  27,  20,  20,  26,   0,  27,  20,  20,  26,   0,   27,  20,  20,  26,   0,  27,  20,  20,  26,   0,   2,
    3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,
    5,  12,  12,  12,  12,  28,   0,   0,   0,   0,   0,   0,    0,   0,   0,   0,   0,  29,  12,  12,  12,  12,   4,
    0,   0,   0,   0,   0,   3,   0,   0,  29,   0,   0,   0,    0,   0,  28,   0,   0,   2,   0,   0,   0,   0,   0,
   10,  10,  10,  10,  10,  30,   0,   0,  24,   0,   0,   0,    0,   0,  25,   0,   0,  31,  10,  10,  10,  10,  10,
    0,   0,   0,   0,   0,   0,   0,   0,  24,   0,   0,   0,    0,   0,  25,   0,   0,   0,   0,   0,   0,   0,   0,
   12,  12,  12,  12,  12,  28,   0,   0,  24,   0,   0,   0,    0,   0,  25,   0,   0,  29,  12,  12,  12,  12,  12,
    0,   0,   0,   0,   0,   3,   0,   0,  31,   0,   0,   0,    0,   0,  30,   0,   0,   2,   0,   0,   0,   0,   0,
    1,  10,  10,  10,  10,  30,   0,   0,   0,   0,   0,   0,    0,   0,   0,   0,   0,  31,  10,  10,  10,  10,  48,
    3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,
    3,   0,  23,  20,  20,  22,   0,  23,  20,  20,  22,   0,   23,  20,  20,  22,   0,  23,  20,  20,  22,   0,   2,
    3,   0,  27,  20,  20,  26,   0,  27,  20,  20,  26,   0,   27,  20,  20,  26,   0,  27,  20,  20,  26,   0,   2,
    3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,
    3,   0,  23,  20,  20,  22,   0,  23,  20,  20,  22,   0,   23,  20,  20,  22,   0,  23,  20,  20,  22,   0,   2,
    3,   0,  27,  20,  20,  26,   0,  27,  20,  20,  26,   0,   27,  20,  20,  26,   0,  27,  20,  20,  26,   0,   2,
    3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,
    5,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,   12,  12,  12,  12,  12,  12,  12,  12,  12,  12,   4

};

LevelA::~LevelA()
{
    delete [] m_game_state.enemies;
    delete    m_game_state.player;
    delete    m_game_state.map;
    delete [] m_game_state.lives_count;
    Mix_FreeChunk(m_game_state.munch_sfx);
    Mix_FreeMusic(m_game_state.bgm);
    Mix_FreeChunk(m_game_state.powerup);
    Mix_FreeChunk(m_game_state.eat_ghost_sfx);
    Mix_FreeChunk(m_game_state.death_sfx);
}

void LevelA::initialise(int lives)
{
    
    m_game_state.next_scene_id = -1;

    GLuint map_texture_id = Utility::load_texture(TILE_FILEPATH);
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_A_DATA, map_texture_id, 1.0f, 16, 3);
    
    GLuint player_texture_id = Utility::load_texture(PLAYER_FILEPATH);

    m_game_state.player = new Entity();
    m_game_state.player->set_entity_type(PLAYER);
    m_game_state.player->set_texture_id(player_texture_id);
    m_game_state.player->set_movement(glm::vec3(0.0f));
    m_game_state.player->set_speed(3.0f);
    m_game_state.player->set_position(glm::vec3(11.0f, -15.0f, 0.0f));
    m_game_state.player->set_width(0.7f);
    m_game_state.player->set_height(0.7f);

    m_game_state.player->m_walking[LEFT]  = new int[2] {2, 3};
    m_game_state.player->m_walking[RIGHT] = new int[2] {0, 1};
    m_game_state.player->m_walking[UP]    = new int[2] {4, 5};
    m_game_state.player->m_walking[DOWN]  = new int[2] {6, 7};
    m_game_state.player->m_animation_indices = m_game_state.player->m_walking[RIGHT];
    m_game_state.player->m_animation_frames = 2;
    m_game_state.player->m_animation_index  = 0;
    m_game_state.player->m_animation_time   = 0.0f;
    m_game_state.player->m_animation_cols   = 2;
    m_game_state.player->m_animation_rows   = 4;


    GLuint blinky_texture_id = Utility::load_texture(BLINKY_FILEPATH);
    GLuint clyde_texture_id = Utility::load_texture(CLYDE_FILEPATH);
    GLuint inky_texture_id = Utility::load_texture(INKY_FILEPATH);
    GLuint pinky_texture_id = Utility::load_texture(PINKY_FILEPATH);
    
    GLuint dot_texture_id = Utility::load_texture(DOT_FILEPATH);
    GLuint fruit_texture_id = Utility::load_texture(FRUIT_FILEPATH);

    if (lives == 3)
    {
        m_game_state.enemies = new Entity[237];
    }
    
    ENEMY_COUNT = 4;
    
    cur = 0;
    dot_count = 0;
    // TOP
    // 1st row
    for (int i = 0; i < 21; i++)
    {
        m_game_state.enemies[i+cur].m_texture_id = dot_texture_id;
        m_game_state.enemies[i+cur].set_ai_type(DOT);
        m_game_state.enemies[i+cur].set_position(glm::vec3(1.0f + i, -1.0f, 0.0f));
        dot_count++;
    }
    m_game_state.enemies[cur].set_ai_type(FRUIT);
    m_game_state.enemies[cur].m_texture_id = fruit_texture_id;
    //m_game_state.enemies[cur].set_scale(glm::vec3(0.5f, 0.5f, 0.0f));
    cur += dot_count;
    dot_count = 0;
    
    //second row
    for (int i = 0; i < 19; i++)
    {
        m_game_state.enemies[i+cur].m_texture_id = dot_texture_id;
        m_game_state.enemies[i+cur].set_ai_type(DOT);
        m_game_state.enemies[i+cur].set_position(glm::vec3(2.0f + i, -4.0f, 0.0f));
        dot_count++;
    }
    cur += dot_count;
    dot_count = 0;
    
    // 3rd row
    for (int i = 0; i < 19; i++)
    {
        m_game_state.enemies[i+cur].m_texture_id = dot_texture_id;
        m_game_state.enemies[i+cur].set_ai_type(DOT);
        m_game_state.enemies[i+cur].set_position(glm::vec3(2.0f + i, -7.0f, 0.0f));
        dot_count++;
    }
    cur += dot_count;
    dot_count = 0;
    
    // left 1st col
    for(int i = 0; i < 6; i++)
    {
        m_game_state.enemies[i+cur].m_texture_id = dot_texture_id;
        m_game_state.enemies[i+cur].set_ai_type(DOT);
        m_game_state.enemies[i+cur].set_position(glm::vec3(1.0f, -2.0f - i, 0.0f));
        dot_count++;
    }
    cur += dot_count;
    dot_count = 0;
    
    // 2nd col
    for(int i = 0; i < 2; i++)
    {
        m_game_state.enemies[i+cur].m_texture_id = dot_texture_id;
        m_game_state.enemies[i+cur].set_ai_type(DOT);
        m_game_state.enemies[i+cur].set_position(glm::vec3(6.0f, -2.0f - i, 0.0f));
        dot_count++;
    }
    cur += dot_count;
    dot_count = 0;
    for(int i = 0; i < 2; i++)
    {
        m_game_state.enemies[i+cur].m_texture_id = dot_texture_id;
        m_game_state.enemies[i+cur].set_ai_type(DOT);
        m_game_state.enemies[i+cur].set_position(glm::vec3(6.0f, -5.0f - i, 0.0f));
        dot_count++;
    }
    cur += dot_count;
    dot_count = 0;
    
    // 3rd col
    for(int i = 0; i < 2; i++)
    {
        m_game_state.enemies[i+cur].m_texture_id = dot_texture_id;
        m_game_state.enemies[i+cur].set_ai_type(DOT);
        m_game_state.enemies[i+cur].set_position(glm::vec3(11.0f, -2.0f - i, 0.0f));
        dot_count++;
    }
    cur += dot_count;
    dot_count = 0;
    for(int i = 0; i < 2; i++)
    {
        m_game_state.enemies[i+cur].m_texture_id = dot_texture_id;
        m_game_state.enemies[i+cur].set_ai_type(DOT);
        m_game_state.enemies[i+cur].set_position(glm::vec3(11.0f, -5.0f - i, 0.0f));
        dot_count++;
    }
    cur += dot_count;
    dot_count = 0;
    
    // 4th col
    for(int i = 0; i < 2; i++)
    {
        m_game_state.enemies[i+cur].m_texture_id = dot_texture_id;
        m_game_state.enemies[i+cur].set_ai_type(DOT);
        m_game_state.enemies[i+cur].set_position(glm::vec3(16.0f, -2.0f - i, 0.0f));
        dot_count++;
    }
    cur += dot_count;
    dot_count = 0;
    for(int i = 0; i < 2; i++)
    {
        m_game_state.enemies[i+cur].m_texture_id = dot_texture_id;
        m_game_state.enemies[i+cur].set_ai_type(DOT);
        m_game_state.enemies[i+cur].set_position(glm::vec3(16.0f, -5.0f - i, 0.0f));
        dot_count++;
    }
    cur += dot_count;
    dot_count = 0;
    
    // rightmost col
    for(int i = 0; i < 6; i++)
    {
        m_game_state.enemies[i+cur].m_texture_id = dot_texture_id;
        m_game_state.enemies[i+cur].set_ai_type(DOT);
        m_game_state.enemies[i+cur].set_position(glm::vec3(21.0f, -2.0f - i, 0.0f));
        dot_count++;
    }
    cur += dot_count;
    dot_count = 0;
    
    // MIDDLE
    //middle row left
    for (int i = 0; i < 7; i++)
    {
        m_game_state.enemies[i+cur].m_texture_id = dot_texture_id;
        m_game_state.enemies[i+cur].set_ai_type(DOT);
        m_game_state.enemies[i+cur].set_position(glm::vec3(1.0f + i, -11.0f, 0.0f));
        dot_count++;
    }
    cur += dot_count;
    dot_count = 0;
    
    // middle row right
    for (int i = 0; i < 7; i++)
    {
        m_game_state.enemies[i+cur].m_texture_id = dot_texture_id;
        m_game_state.enemies[i+cur].set_ai_type(DOT);
        m_game_state.enemies[i+cur].set_position(glm::vec3(15.0f + i, -11.0f, 0.0f));
        dot_count++;
    }
    cur += dot_count;
    dot_count = 0;
    
    // left col
    for(int i = 0; i < 3; i++)
    {
        m_game_state.enemies[i+cur].m_texture_id = dot_texture_id;
        m_game_state.enemies[i+cur].set_ai_type(DOT);
        m_game_state.enemies[i+cur].set_position(glm::vec3(6.0f, -8.0f - i, 0.0f));
        dot_count++;
    }
    cur += dot_count;
    dot_count = 0;
    for(int i = 0; i < 3; i++)
    {
        m_game_state.enemies[i+cur].m_texture_id = dot_texture_id;
        m_game_state.enemies[i+cur].set_ai_type(DOT);
        m_game_state.enemies[i+cur].set_position(glm::vec3(6.0f, -12.0f - i, 0.0f));
        dot_count++;
    }
    cur += dot_count;
    dot_count = 0;
    
    // right col
    for(int i = 0; i < 3; i++)
    {
        m_game_state.enemies[i+cur].m_texture_id = dot_texture_id;
        m_game_state.enemies[i+cur].set_ai_type(DOT);
        m_game_state.enemies[i+cur].set_position(glm::vec3(16.0f, -8.0f - i, 0.0f));
        dot_count++;
    }
    cur += dot_count;
    dot_count = 0;
    for(int i = 0; i < 3; i++)
    {
        m_game_state.enemies[i+cur].m_texture_id = dot_texture_id;
        m_game_state.enemies[i+cur].set_ai_type(DOT);
        m_game_state.enemies[i+cur].set_position(glm::vec3(16.0f, -12.0f - i, 0.0f));
        dot_count++;
    }
    cur += dot_count;
    dot_count = 0;
    
    // BOTTOM
    // 1st row
    for (int i = 0; i < 19; i++)
    {
        m_game_state.enemies[i+cur].m_texture_id = dot_texture_id;
        m_game_state.enemies[i+cur].set_ai_type(DOT);
        m_game_state.enemies[i+cur].set_position(glm::vec3(20.0f - i, -15.0f, 0.0f));
        dot_count++;
    }
    cur += dot_count;
    dot_count = 0;
    
    // 2nd row
    for (int i = 0; i < 19; i++)
    {
        m_game_state.enemies[i+cur].m_texture_id = dot_texture_id;
        m_game_state.enemies[i+cur].set_ai_type(DOT);
        m_game_state.enemies[i+cur].set_position(glm::vec3(20.0f - i, -18.0f, 0.0f));
        dot_count++;
    }
    cur += dot_count;
    dot_count = 0;
    
    // 3rd row
    for (int i = 0; i < 20; i++)
    {
        m_game_state.enemies[i+cur].m_texture_id = dot_texture_id;
        m_game_state.enemies[i+cur].set_ai_type(DOT);
        m_game_state.enemies[i+cur].set_position(glm::vec3(21.0f - i, -21.0f, 0.0f));
        dot_count++;
    }
    m_game_state.enemies[cur].set_ai_type(FRUIT);
    m_game_state.enemies[cur].m_texture_id = fruit_texture_id;
    cur += dot_count;
    dot_count = 0;
    
    // left 1st col
    for(int i = 0; i < 7; i++)
    {
        m_game_state.enemies[i+cur].m_texture_id = dot_texture_id;
        m_game_state.enemies[i+cur].set_ai_type(DOT);
        m_game_state.enemies[i+cur].set_position(glm::vec3(1.0f, -15.0f - i, 0.0f));
        dot_count++;
    }
    cur += dot_count;
    dot_count = 0;
    
    // 2nd col
    for(int i = 0; i < 2; i++)
    {
        m_game_state.enemies[i+cur].m_texture_id = dot_texture_id;
        m_game_state.enemies[i+cur].set_ai_type(DOT);
        m_game_state.enemies[i+cur].set_position(glm::vec3(6.0f, -16.0f - i, 0.0f));
        dot_count++;
    }
    cur += dot_count;
    dot_count = 0;
    for(int i = 0; i < 2; i++)
    {
        m_game_state.enemies[i+cur].m_texture_id = dot_texture_id;
        m_game_state.enemies[i+cur].set_ai_type(DOT);
        m_game_state.enemies[i+cur].set_position(glm::vec3(6.0f, -19.0f - i, 0.0f));
        dot_count++;
    }
    cur += dot_count;
    dot_count = 0;
    
    // 3rd col
    for(int i = 0; i < 2; i++)
    {
        m_game_state.enemies[i+cur].m_texture_id = dot_texture_id;
        m_game_state.enemies[i+cur].set_ai_type(DOT);
        m_game_state.enemies[i+cur].set_position(glm::vec3(11.0f, -16.0f - i, 0.0f));
        dot_count++;
    }
    cur += dot_count;
    dot_count = 0;
    for(int i = 0; i < 2; i++)
    {
        m_game_state.enemies[i+cur].m_texture_id = dot_texture_id;
        m_game_state.enemies[i+cur].set_ai_type(DOT);
        m_game_state.enemies[i+cur].set_position(glm::vec3(11.0f, -19.0f - i, 0.0f));
        dot_count++;
    }
    cur += dot_count;
    dot_count = 0;
    
    // 4th col
    for(int i = 0; i < 2; i++)
    {
        m_game_state.enemies[i+cur].m_texture_id = dot_texture_id;
        m_game_state.enemies[i+cur].set_ai_type(DOT);
        m_game_state.enemies[i+cur].set_position(glm::vec3(16.0f, -16.0f - i, 0.0f));
        dot_count++;
    }
    cur += dot_count;
    dot_count = 0;
    for(int i = 0; i < 2; i++)
    {
        m_game_state.enemies[i+cur].m_texture_id = dot_texture_id;
        m_game_state.enemies[i+cur].set_ai_type(DOT);
        m_game_state.enemies[i+cur].set_position(glm::vec3(16.0f, -19.0f - i, 0.0f));
        dot_count++;
    }
    cur += dot_count;
    dot_count = 0;
    
    // rightmost col
    for(int i = 0; i < 6; i++)
    {
        m_game_state.enemies[i+cur].m_texture_id = dot_texture_id;
        m_game_state.enemies[i+cur].set_ai_type(DOT);
        m_game_state.enemies[i+cur].set_position(glm::vec3(21.0f, -15.0f - i, 0.0f));
        dot_count++;
    }
    cur += dot_count;
    dot_count = 0;
        
    ENEMY_COUNT += cur;
    m_game_state.enemies[cur].m_texture_id = clyde_texture_id;
    m_game_state.enemies[cur+1].m_texture_id = inky_texture_id;
    m_game_state.enemies[cur+2].m_texture_id = blinky_texture_id;
    m_game_state.enemies[cur+3].m_texture_id = pinky_texture_id;
    for (int i = cur; i < ENEMY_COUNT; i++)
    {
        m_game_state.enemies[i].set_entity_type(ENEMY);
        m_game_state.enemies[i].set_ai_type(WALKER);
        m_game_state.enemies[i].set_ai_state(IDLE);
        m_game_state.enemies[i].set_position(glm::vec3((ENEMY_COUNT - i) + 9.0f, -11.0f, 0.0f));
        m_game_state.enemies[i].set_speed(3.0f);
        m_game_state.enemies[i].set_width(0.8f);
        m_game_state.enemies[i].set_height(0.8f);
        
        m_game_state.enemies[i].m_walking[LEFT]  = new int[2] {2, 3};
        m_game_state.enemies[i].m_walking[RIGHT] = new int[2] {0, 1};
        m_game_state.enemies[i].m_walking[UP]    = new int[2] {4, 5};
        m_game_state.enemies[i].m_walking[DOWN]  = new int[2] {6, 7};
        m_game_state.enemies[i].m_animation_indices = m_game_state.enemies[i].m_walking[LEFT];
        
        m_game_state.enemies[i].m_animation_frames = 2;
        m_game_state.enemies[i].m_animation_index  = 0;
        m_game_state.enemies[i].m_animation_time   = 0.0f;
        m_game_state.enemies[i].m_animation_cols   = 8;
        m_game_state.enemies[i].m_animation_rows   = 1;
    }
    
    m_game_state.player->dot_count = cur;
    
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        if (!m_game_state.enemies[i].get_active_status())
        {
            m_game_state.player->dot_count -= 1;
        }
    }
     
    // LIVES COUNT
    m_game_state.lives_count = new Entity [3];
    GLuint lives_texture_id = Utility::load_texture("assets/images/lives.png");
    for (int i = 0; i < 3; i++)
    {
        m_game_state.lives_count[i].set_entity_type(LIVES);
        m_game_state.lives_count[i].set_width(0.8f);
        m_game_state.lives_count[i].set_height(0.8f);
        m_game_state.lives_count[i].set_position(glm::vec3(23.5f + i, -21.0f, 0.0f));
        m_game_state.lives_count[i].m_texture_id = lives_texture_id;
    }
    
    for(int i = lives; i < 3; i++)
    {
        m_game_state.lives_count[i].deactivate();
    }


    Mix_OpenAudio(0100, MIX_DEFAULT_FORMAT, 2, 4096);

    m_game_state.bgm = Mix_LoadMUS("assets/audio/bgm.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 8.0f);
    
    m_game_state.munch_sfx = Mix_LoadWAV("assets/audio/munch.wav");
    
    m_game_state.powerup = Mix_LoadWAV("assets/audio/powerup.wav");

}

void LevelA::update(float delta_time)
{
    GLuint blinky_texture_id = Utility::load_texture(BLINKY_FILEPATH);
    GLuint clyde_texture_id = Utility::load_texture(CLYDE_FILEPATH);
    GLuint inky_texture_id = Utility::load_texture(INKY_FILEPATH);
    GLuint pinky_texture_id = Utility::load_texture(PINKY_FILEPATH);
        
    cur_time += delta_time;
    
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.map);

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        if (m_game_state.enemies[i].get_ai_type() == WALKER)
        {
            m_game_state.enemies[i].ai_walk();
        }
        m_game_state.enemies[i].update(delta_time, m_game_state.player, NULL, 0, m_game_state.map);
    }
    
    for (int i = cur; i < ENEMY_COUNT; i++)
    {
        if (m_game_state.enemies[i].get_position().x < 9.5 && m_game_state.enemies[i].get_position().x > 7.5 && m_game_state.enemies[i].get_position().y > -14 && m_game_state.enemies[i].get_position().y < -8)
        {
            m_game_state.enemies[i].set_ai_state(MOVEUP);
            break;
        }
    }
    // win or lose
    if (m_game_state.player->dot_count == 0)
    {
        m_game_state.next_scene_id = 3;
    }
    if (m_game_state.player->die)
    {
        Mix_PauseMusic();
        m_game_state.next_scene_id = 2;
    }
    
    
    if (m_game_state.player->eat)
    {
        m_game_state.player->eat= false;
        Mix_PlayChannel(1, m_game_state.munch_sfx, 0);
        Mix_Volume(1, MIX_MAX_VOLUME / 8.0f);
    }
    
    GLuint dead_enemy_texture_id = Utility::load_texture(DEAD_ENEMY_FILEPATH);
    
    if (m_game_state.player->power_up)
    {
        m_game_state.player->powerup_time += delta_time;
        Mix_PauseMusic();
        Mix_PlayChannel(2, m_game_state.powerup, -1);
        Mix_Volume(2, MIX_MAX_VOLUME / 8.0f);
        m_game_state.player->set_speed(4.0f);
        
        for (int i = cur; i < ENEMY_COUNT; i++)
        {
            m_game_state.enemies[i].set_speed(3.5f);
            m_game_state.enemies[i].m_texture_id = dead_enemy_texture_id;
            m_game_state.enemies[i].m_animation_cols = 2;
            if (m_game_state.enemies[i].die)
            {
                m_game_state.enemies[i].die = false;
                Mix_Pause(2);
                Mix_PlayChannel(3, m_game_state.eat_ghost_sfx, 0);
                Mix_Volume(3, MIX_MAX_VOLUME / 8.0f);
                Mix_Resume(2);
                m_game_state.enemies[i].set_position(glm::vec3(10.0f, -11.0f, 0.0f));
                cur_time = 0;
            }
        }
        
        if (m_game_state.player->powerup_time > 8)
        {
            m_game_state.player->power_up = false;
        }
    }
    else
    {
        Mix_Pause(2);
        Mix_ResumeMusic();
        m_game_state.player->powerup_time = 0;
        for (int i = cur; i < ENEMY_COUNT; i++)
        {
            m_game_state.enemies[i].set_speed(2.5f);
            m_game_state.enemies[i].m_animation_cols = 8;
        }
        m_game_state.enemies[cur].m_texture_id = clyde_texture_id;
        m_game_state.enemies[cur+1].m_texture_id = inky_texture_id;
        m_game_state.enemies[cur+2].m_texture_id = blinky_texture_id;
        m_game_state.enemies[cur+3].m_texture_id = pinky_texture_id;
    }
    
    // across screen
    if (m_game_state.player->get_position().x < 0)
    {
        m_game_state.player->set_position(glm::vec3(23.0f, m_game_state.player->get_position().y, 0.0f));
    }
    
    if (m_game_state.player->get_position().x > 23)
    {
        m_game_state.player->set_position(glm::vec3(0.0f, m_game_state.player->get_position().y, 0.0f));
    }
    
    // lives count update
    for (int i = 0; i < 3; i++)
    {
        m_game_state.lives_count[i].update(delta_time, m_game_state.player, NULL, 0, m_game_state.map);
    }
    
}


void LevelA::render(ShaderProgram *program)
{
    m_game_state.map->render(program);
    m_game_state.player->render(program);

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        if (m_game_state.enemies[i].get_active_status()) { m_game_state.enemies[i].render(program); }
    }
    
    for (int i = 0; i < 3; i++)
    {
        if (m_game_state.lives_count[i].get_active_status()) { m_game_state.lives_count[i].render(program); }
    }
    

}
