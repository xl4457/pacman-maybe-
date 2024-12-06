#include "GameWon.h"
#include "Utility.h"

#define LEVEL_WIDTH 14
#define LEVEL_HEIGHT 8

constexpr char PLAYER_FILEPATH[] = "assets/images/pacman.png",
            PLATFORM_FILEPATH[]    = "assets/images/MazeTilesCopy.png",
            FONT_FILEPATH[] = "assets/fonts/font1.png";


unsigned int WIN_LEVEL_DATA[] =
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

GameWon::~GameWon()
{
    delete    m_game_state.player;
    delete    m_game_state.map;
    delete m_game_state.enemies;
    Mix_FreeMusic(m_game_state.bgm);
    Mix_FreeChunk(m_game_state.munch_sfx);
    Mix_FreeChunk(m_game_state.death_sfx);
    Mix_FreeChunk(m_game_state.eat_ghost_sfx);
    Mix_FreeChunk(m_game_state.powerup);
}

void GameWon::initialise(int lives)
{
    m_game_state.next_scene_id = -1;

    
    GLuint map_texture_id = Utility::load_texture(PLATFORM_FILEPATH);
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, WIN_LEVEL_DATA, map_texture_id, 1.0f, 4, 1);
    
    GLuint player_texture_id = Utility::load_texture(PLAYER_FILEPATH);

    m_game_state.player = new Entity();
    m_game_state.player->set_entity_type(PLAYER);
    m_game_state.player->set_texture_id(player_texture_id);
    m_game_state.player->set_position(glm::vec3(2.0f, -5.0f, 0.0f));
    m_game_state.player->set_movement(glm::vec3(0.0f));
    m_game_state.player->deactivate();


    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    m_game_state.bgm = Mix_LoadMUS("assets/audio/victory.mp3");
    Mix_PlayMusic(m_game_state.bgm, 0);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 13.0f);

}

void GameWon::update(float delta_time) {}


void GameWon::render(ShaderProgram *program)
{
    GLuint font_texture_id = Utility::load_texture(FONT_FILEPATH);

    Utility::draw_text(program, font_texture_id, "You Win", 1.8f, -0.1f, glm::vec3(7.5f, -10.0f, 0.0f));
}
