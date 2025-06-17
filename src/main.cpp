#include "opengl-framework/opengl-framework.hpp"
#include "utils.hpp"

int main()
{
    gl::init("Particules!");
    gl::maximize_window();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    struct particule
    {
        glm::vec2 position = glm::vec2(utils::rand(-1,1),utils::rand(-1,1));

        float random1=utils::rand(0,360);
        float random2=utils::rand(0,360);

        float cosResult = cos(random1);
        float resultSin = sin(random2);

        float airFriction=5.0f;

        
        glm::vec2 direction = glm::vec2(cosResult,resultSin);
        
        float mass = utils::rand(1,10);
        glm::vec2 gravity = glm::vec2(0,-0.1f)*mass;
        glm::vec2 force;
        glm::vec2 acceleration;
        
        
    };
    

    // TODO: create an array of particles
    std::vector listParticule =  std::vector<particule>(100);

    while (gl::window_is_open())
    {

        
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        // TODO update particles
        // TODO render particles
        
        for(particule& i : listParticule){
            utils::draw_disk(i.position,0.01,glm::vec4(1,1,1,1));
            glm::vec2 friction = -i.airFriction * i.direction;
            i.force = i.gravity + friction;
            i.acceleration= i.force/ i.mass;
            i.direction+= i.acceleration * gl::delta_time_in_seconds();
            i.position+= i.direction * gl::delta_time_in_seconds();
            
        };
    }
}