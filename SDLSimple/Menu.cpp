#include "Menu.h"
#include "Utility.h"

#define LEVEL_WIDTH 1
#define LEVEL_HEIGHT 1

unsigned int MENU_DATA[] = { 0 };
constexpr char FONTS_FILEPATH[] = "assets/fonts/font1.png";

MenuScreen::~MenuScreen()
{
    delete m_game_state.enemies;
    delete m_game_state.player;
    delete m_game_state.map;
    delete m_game_state.lives_count;
    Mix_FreeMusic(m_game_state.bgm);
}

void MenuScreen::initialise(int lives)
{
    m_game_state.next_scene_id = -1;

    GLuint map_texture_id = Utility::load_texture("assets/images/MazeTilesCopy.png");
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, MENU_DATA, map_texture_id, 1.0f, 1, 1);
    
    m_game_state.player = new Entity();
    m_game_state.player->set_entity_type(PLAYER);
    m_game_state.player->deactivate();
    
    m_game_state.enemies = new Entity();
    m_game_state.enemies->deactivate();
    
    GLuint lives_texture_id = Utility::load_texture("assets/images/lives.png");
    
    m_game_state.lives_count = new Entity();
    m_game_state.lives_count->set_entity_type(LIVES);
    m_game_state.lives_count->set_texture_id(lives_texture_id);
    m_game_state.lives_count->set_position(glm::vec3(12.5f, -9.0f, 0.0f));


    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    m_game_state.bgm = Mix_LoadMUS("assets/audio/ready.mp3");
    Mix_PlayMusic(m_game_state.bgm, 0);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 8.0f);
}

void MenuScreen::update(float delta_time)
{
    if (delta_time == -1) {
        m_game_state.next_scene_id = 1;
        }
    
    m_game_state.lives_count->update(delta_time, m_game_state.player, m_game_state.enemies, 0, m_game_state.map);

}


void MenuScreen::render(ShaderProgram* program)
{
    m_game_state.map->render(program);
    m_game_state.lives_count->render(program);
    GLuint text_texture_id = Utility::load_texture(FONTS_FILEPATH);
    Utility::draw_text(program, text_texture_id, "PacMan", 1.5f, -0.1f, glm::vec3(9.5f, -7.0f, 0.0f));
    Utility::draw_text(program, text_texture_id, "PRESS ENTER TO START", 1.0f, -0.1f, glm::vec3(5.0f, -11.0f, 0.0f));

}
