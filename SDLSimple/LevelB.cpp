#include "LevelB.h"
#include "Utility.h"


#define LEVEL_B_WIDTH 14
#define LEVEL_B_HEIGHT 8


unsigned int LEVEL_B_DATA[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

LevelB::~LevelB()
{
    delete m_game_state.enemies;
    delete m_game_state.player;
    delete m_game_state.map;
    delete m_game_state.lives_count;
    Mix_FreeChunk(m_game_state.munch_sfx);
    Mix_FreeMusic(m_game_state.bgm);
    Mix_FreeChunk(m_game_state.powerup);
    Mix_FreeChunk(m_game_state.eat_ghost_sfx);
    Mix_FreeChunk(m_game_state.death_sfx);
}

void LevelB::initialise(int lives) {}

void LevelB::initialise(int temp, glm::vec3 player_pos)
{
    m_game_state.next_scene_id = -1;

    GLuint map_texture_id = Utility::load_texture("assets/images/MazeTilesCopy.png");
    m_game_state.map = new Map(LEVEL_B_WIDTH, LEVEL_B_HEIGHT, LEVEL_B_DATA, map_texture_id, 1, 16, 3);
    player_lives = temp;

    m_game_state.player = new Entity();
    m_game_state.player->set_entity_type(PLAYER);
    m_game_state.player->set_speed(2.5f);
    m_game_state.player->set_position(player_pos);
    m_game_state.player->set_texture_id(Utility::load_texture("assets/images/dead_pacman.png"));

    m_game_state.player->m_walking[RIGHT] = new int[11] {0,1,2,3,4,5,6,7,8,9,10};
    m_game_state.player->m_walking[LEFT] = new int[1] {10};
    
    m_game_state.player->m_animation_indices = m_game_state.player->m_walking[RIGHT];
    m_game_state.player->m_animation_frames = 11;
    m_game_state.player->m_animation_index  = 0;
    m_game_state.player->m_animation_time   = 0.0f;
    m_game_state.player->m_animation_cols   = 11;
    m_game_state.player->m_animation_rows   = 1;
    
    m_game_state.enemies = new Entity();
    m_game_state.enemies->deactivate();

    
    m_game_state.death_sfx = Mix_LoadWAV("assets/audio/death.wav");
    Mix_PlayChannel(1, m_game_state.death_sfx, 0);
    Mix_Volume(1, MIX_MAX_VOLUME / 8.0f);
}

void LevelB::update(float delta_time)
{
    cur_time += delta_time;
    
    if (cur_time >= 2.8)
    {
        cur_time = 0;
        if (player_lives == 0)
        {
            Mix_Pause(1);
            m_game_state.next_scene_id = 4;
        }
        else
        {
            m_game_state.next_scene_id = 1;
        }
    }
    
    m_game_state.player->update(delta_time, m_game_state.player, NULL, 0, m_game_state.map);

}


void LevelB::render(ShaderProgram *program)
{
    m_game_state.player->render(program);
   
}
