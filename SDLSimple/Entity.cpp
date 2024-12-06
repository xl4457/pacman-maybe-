#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Entity.h"

void Entity::ai_activate(Entity *player, Map* map)
{
    switch (m_ai_type)
    {
        case WALKER:
            ai_walk();
            break;

        default:
            break;
    }
}

void Entity::ai_walk()
{
    switch (m_ai_state)
    {
        case IDLE:
            direction_int = rand() % 4;
            if (direction_int == 0) m_ai_state = MOVERIGHT;
            else if (direction_int == 1) m_ai_state = MOVEUP;
            else if (direction_int == 2) m_ai_state = MOVEDOWN;
            else m_ai_state = MOVELEFT;
            break;
        case MOVELEFT:
            move_left();
            m_animation_indices = m_walking[LEFT];
            if (m_collided_left)
            {
                m_ai_state = NEWDIRECTION;
            }
            break;
            
        case MOVERIGHT:
            move_right();
            m_animation_indices = m_walking[RIGHT];
            if (m_collided_right)
            {
                m_ai_state = NEWDIRECTION;
            }
            break;
            
        case MOVEUP:
            move_up();
            m_animation_indices = m_walking[UP];
            if (m_collided_top)
            {
                m_ai_state = NEWDIRECTION;
            }
            break;
            
        case MOVEDOWN:
            move_down();
            m_animation_indices = m_walking[DOWN];
            if (m_collided_bottom)
            {
                m_ai_state = NEWDIRECTION;
            }
            break;
            
        case NEWDIRECTION:
            stop();
            direction_int = std::rand() % 4;
            if (direction_int == 0)
            {
                m_ai_state = MOVELEFT;
            }
            else if (direction_int == 1)
            {
                m_ai_state = MOVERIGHT;
            }
            else if (direction_int == 2)
            {
                m_ai_state = MOVEUP;
            }
            else if (direction_int == 3)
            {
                m_ai_state = MOVEDOWN;
            }

            break;
            
        default:
            break;
    }
}

//void Entity::ai_guard(Entity *player, Map* map){}

// Default constructor
Entity::Entity()
{
    // ––––– PHYSICS ––––– //
    m_position = glm::vec3(0.0f);
    m_velocity = glm::vec3(0.0f);
    m_acceleration = glm::vec3(0.0f);
    
    // ––––– TRANSLATION ––––– //
    m_movement = glm::vec3(0.0f);
    m_speed = 0;
    m_model_matrix = glm::mat4(1.0f);
    
}

Entity::Entity(GLuint texture_id, float speed, float width, float height, EntityType EntityType, AIType AIType, AIState AIState)
: m_position(0.0f), m_movement(0.0f), m_scale(1.0f, 1.0f, 0.0f), m_model_matrix(1.0f), m_speed(speed), m_animation_cols(0), m_animation_frames(0), m_animation_index(0), m_animation_rows(0), m_animation_indices(nullptr), m_animation_time(0.0f), m_texture_id(texture_id), m_velocity(0.0f), m_acceleration(0.0f), m_width(width), m_height(height),m_entity_type(EntityType), m_ai_type(AIType), m_ai_state(AIState)
{
    
}

Entity::~Entity()
{
    delete[] m_animation_up;
    delete[] m_animation_down;
    delete[] m_animation_left;
    delete[] m_animation_right;
    delete[] m_walking;
}

void Entity::draw_sprite_from_texture_atlas(ShaderProgram* program, GLuint texture_id, int index)
{
    // Step 1: Calculate the UV location of the indexed frame
    float u_coord = (float)(index % m_animation_cols) / (float)m_animation_cols;
    float v_coord = (float)(index / m_animation_cols) / (float)m_animation_rows;

    // Step 2: Calculate its UV size
    float width = 1.0f / (float)m_animation_cols;
    float height = 1.0f / (float)m_animation_rows;

    // Step 3: Just as we have done before, match the texture coordinates to the vertices
    float tex_coords[] =
    {
        u_coord, v_coord + height, u_coord + width, v_coord + height, u_coord + width, v_coord,
        u_coord, v_coord + height, u_coord + width, v_coord, u_coord, v_coord
    };

    float vertices[] =
    {
        -0.5, -0.5, 0.5, -0.5,  0.5, 0.5,
        -0.5, -0.5, 0.5,  0.5, -0.5, 0.5
    };

    // Step 4: And render
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->get_position_attribute());

    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords);
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}

bool const Entity::check_collision(Entity* other) const
{
    if (other == this) return false;
    if (!m_is_active || !other->m_is_active) return false;
    
    float x_distance = fabs(m_position.x - other->m_position.x) - ((m_width + other->m_width) / 2.0f);
    float y_distance = fabs(m_position.y - other->m_position.y) - ((m_height + other->m_height) / 2.0f);

    return x_distance < 0.0f && y_distance < 0.0f;
}

void const Entity::check_collision_y(Entity *collidable_entities, int collidable_entity_count)
{
    for (int i = 0; i < collidable_entity_count; i++)
    {
        Entity *collidable_entity = &collidable_entities[i];
        
        if (check_collision(collidable_entity))
        {
            if (collidable_entity->get_ai_type() == DOT)
            {
                dot_count--;
                collidable_entity->deactivate();
                eat = true;
                break;
            }
            else if (collidable_entity->get_ai_type() == FRUIT)
            {
                power_up = true;
                dot_count--;
                collidable_entity->deactivate();
                break;
            }
            else if (power_up)
            {
                collidable_entity->die = true;
                collidable_entity->set_position(glm::vec3(8.0f, -11.0f, 0.0f));
                break;
            }
            else if (!power_up)
            {
                die = true;
                break;
            }
            
            float y_distance = fabs(m_position.y - collidable_entity->m_position.y);
            float y_overlap = fabs(y_distance - (m_height / 2.0f) - (collidable_entity->m_height / 2.0f));
            if (m_velocity.y > 0)
            {
                m_position.y   -= y_overlap;
                m_velocity.y    = 0;

                // Collision!
                m_collided_top  = true;
            }
            else if (m_velocity.y < 0)
            {
                m_position.y      += y_overlap;
                m_velocity.y       = 0;

                // Collision!
                m_collided_bottom  = true;
            }
        }
    }
}

void const Entity::check_collision_x(Entity *collidable_entities, int collidable_entity_count)
{
    for (int i = 0; i < collidable_entity_count; i++)
    {
        Entity *collidable_entity = &collidable_entities[i];
        
        if (check_collision(collidable_entity))
        {
            if (collidable_entity->get_ai_type() == DOT)
            {
                dot_count--;
                collidable_entity->deactivate();
                eat = true;
                break;
            }
            else if(collidable_entity->get_ai_type() == FRUIT)
            {
                power_up = true;
                dot_count--;
                collidable_entity->deactivate();
                break;
            }
            else if (power_up)
            {
                collidable_entity->die = true;
                collidable_entity->set_position(glm::vec3(11.0f,-8.0f,1.0f));
                break;
            }
            else if (!power_up)
            {
                die = true;
                break;
            }
            
            float x_distance = fabs(m_position.x - collidable_entity->m_position.x);
            float x_overlap = fabs(x_distance - (m_width / 2.0f) - (collidable_entity->m_width / 2.0f));
            if (m_velocity.x > 0)
            {
                m_position.x     -= x_overlap;
                m_velocity.x      = 0;

                // Collision!
                m_collided_right  = true;
                
            }
            else if (m_velocity.x < 0)
            {
                m_position.x    += x_overlap;
                m_velocity.x     = 0;
 
                // Collision!
                m_collided_left  = true;
            }
        }
    }
}

void const Entity::check_collision_y(Map *map)
{
    // Probes for tiles above
    glm::vec3 top = glm::vec3(m_position.x, m_position.y + (m_height / 2), m_position.z);
    glm::vec3 top_left = glm::vec3(m_position.x - (m_width / 2), m_position.y + (m_height / 2), m_position.z);
    glm::vec3 top_right = glm::vec3(m_position.x + (m_width / 2), m_position.y + (m_height / 2), m_position.z);
    
    // Probes for tiles below
    glm::vec3 bottom = glm::vec3(m_position.x, m_position.y - (m_height / 2), m_position.z);
    glm::vec3 bottom_left = glm::vec3(m_position.x - (m_width / 2), m_position.y - (m_height / 2), m_position.z);
    glm::vec3 bottom_right = glm::vec3(m_position.x + (m_width / 2), m_position.y - (m_height / 2), m_position.z);
    
    float penetration_x = 0;
    float penetration_y = 0;
    
    // If the map is solid, check the top three points
    if (map->is_solid(top, &penetration_x, &penetration_y) && m_velocity.y > 0)
    {
        m_position.y -= penetration_y;
        m_velocity.y = 0;
        m_collided_top = true;
    }
    else if (map->is_solid(top_left, &penetration_x, &penetration_y) && m_velocity.y > 0)
    {
        m_position.y -= penetration_y;
        m_velocity.y = 0;
        m_collided_top = true;
    }
    else if (map->is_solid(top_right, &penetration_x, &penetration_y) && m_velocity.y > 0)
    {
        m_position.y -= penetration_y;
        m_velocity.y = 0;
        m_collided_top = true;
    }
    
    // And the bottom three points
    if (map->is_solid(bottom, &penetration_x, &penetration_y) && m_velocity.y < 0)
    {
        m_position.y += penetration_y;
        m_velocity.y = 0;
        m_collided_bottom = true;
    }
    else if (map->is_solid(bottom_left, &penetration_x, &penetration_y) && m_velocity.y < 0)
    {
            m_position.y += penetration_y;
            m_velocity.y = 0;
            m_collided_bottom = true;
    }
    else if (map->is_solid(bottom_right, &penetration_x, &penetration_y) && m_velocity.y < 0)
    {
        m_position.y += penetration_y;
        m_velocity.y = 0;
        m_collided_bottom = true;
        
    }
}

void const Entity::check_collision_x(Map *map)
{
    // Probes for tiles; the x-checking is much simpler
    glm::vec3 left  = glm::vec3(m_position.x - (m_width / 2), m_position.y, m_position.z);
    glm::vec3 right = glm::vec3(m_position.x + (m_width / 2), m_position.y, m_position.z);
    
    float penetration_x = 0;
    float penetration_y = 0;
    
    if (map->is_solid(left, &penetration_x, &penetration_y) && m_velocity.x < 0)
    {
        m_position.x += penetration_x;
        m_velocity.x = 0;
        m_collided_left = true;
    }
    if (map->is_solid(right, &penetration_x, &penetration_y) && m_velocity.x > 0)
    {
        m_position.x -= penetration_x;
        m_velocity.x = 0;
        m_collided_right = true;
    }
}


void Entity::update(float delta_time, Entity *player, Entity *collidable_entities, int collidable_entity_count, Map* map)
{
    if (!m_is_active) return;
 
    m_collided_top    = false;
    m_collided_bottom = false;
    m_collided_left   = false;
    m_collided_right  = false;
    
    if (m_entity_type == ENEMY) ai_activate(player, map);
    
    if (m_animation_indices != NULL)
    {
        if (glm::length(m_movement) != 0 || sizeof(m_walking[RIGHT]) > 7)
        {
            m_animation_time += delta_time;
            float frames_per_second = (float)1 / SECONDS_PER_FRAME;

            if (m_animation_time >= frames_per_second)
            {
                m_animation_time = 0.0f;
                m_animation_index++;

                if (m_animation_index >= m_animation_frames)
                {
                    m_animation_index = 0;
                }
            }
        }
    }
    
    m_velocity.x = m_movement.x * m_speed;
    m_position.x += m_velocity.x * delta_time;
    
    check_collision_x(collidable_entities, collidable_entity_count);
    check_collision_x(map);
    
    m_velocity.y = m_movement.y * m_speed;
    m_position.y += m_velocity.y * delta_time;
    
    check_collision_y(collidable_entities, collidable_entity_count);
    check_collision_y(map);
    
    
    m_model_matrix = glm::mat4(1.0f);
    m_model_matrix = glm::translate(m_model_matrix, m_position);
}


void Entity::render(ShaderProgram* program)
{
    if (m_is_active)
    {
        program->set_model_matrix(m_model_matrix);

        if (m_animation_indices != NULL)
        {
            draw_sprite_from_texture_atlas(program, m_texture_id, m_animation_indices[m_animation_index]);
            return;
        }

        float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
        float tex_coords[] = { 0.0,  1.0, 1.0,  1.0, 1.0, 0.0,  0.0,  1.0, 1.0, 0.0,  0.0, 0.0 };

        glBindTexture(GL_TEXTURE_2D, m_texture_id);

        glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
        glEnableVertexAttribArray(program->get_position_attribute());
        glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords);
        glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisableVertexAttribArray(program->get_position_attribute());
        glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
    }
}
