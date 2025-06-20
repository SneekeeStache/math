#include "opengl-framework/opengl-framework.hpp"
#include "utils.hpp"
#include "iostream"


glm::vec2 checkIntersect(glm::vec2 obj1Pos1,glm::vec2 obj1Pos2,glm::vec2 obj2Pos1, glm::vec2 obj2Pos2){
    glm::vec2 direction1= obj1Pos2-obj1Pos1;
    glm::vec2 direction2= obj2Pos2-obj2Pos1;
    glm::mat2 myMatrice = glm::mat2(direction1,-direction2);
    glm::mat2 matriveReverse = glm::inverse(myMatrice);
    glm::vec2 originObj1Obj2= obj2Pos1-obj1Pos1;
    glm::vec2 result= matriveReverse*originObj1Obj2;
    return result;
}

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

        float airFriction=3.0f;
        float SpringStiffness = 5.f;
        
        glm::vec2 direction = glm::vec2(cosResult,resultSin);
        
        float mass = utils::rand(1,5);
        glm::vec2 gravity = glm::vec2(0,-0.1f)*mass;
        glm::vec2 force;
        glm::vec2 acceleration;
        glm::vec2 friction;
        glm::vec2 spring;

        float maximumAge = utils::rand(5.f,10.f);
        float age = 0.f;

        float radius=0.015f;
        float currentRadius;

        glm::vec4 color = glm::vec4(utils::rand(0,1),utils::rand(0,1),utils::rand(0,1),1);
    };

    struct wall
    {
        glm::vec2 position1= glm::vec2(utils::rand(-1,1),utils::rand(-1,1));
        glm::vec2 position2= glm::vec2(utils::rand(-1,1),utils::rand(-1,1));
        float thickness=0.01;
        glm::vec4 color = glm::vec4(utils::rand(0,1),utils::rand(0,1),utils::rand(0,1),1);
        
    };

    
    

    // TODO: create an array of particles
    std::vector listParticule =  std::vector<particule>(100);
    wall testwall;
    wall testFromMouse;

    

    while (gl::window_is_open())
    {

        
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        // TODO update particles
        // TODO render particles
        
        for(particule& i : listParticule){
            i.currentRadius= i.radius * (1.0f - i.age / i.maximumAge);
            if(i.age < i.maximumAge){
                utils::draw_disk(i.position,i.currentRadius,i.color);
            }
            i.friction= -i.airFriction * i.direction;
            i.spring = i.SpringStiffness * (gl::mouse_position() - i.position); 
            i.force = i.gravity + i.friction + i.spring;
            i.acceleration= i.force/ i.mass;
            i.direction+= i.acceleration * gl::delta_time_in_seconds();
            i.position+= i.direction * gl::delta_time_in_seconds();

            i.age+= gl::delta_time_in_seconds();
            
            
        };
        utils::draw_line(testwall.position1,testwall.position2,testwall.thickness,testwall.color);
        utils::draw_line(testFromMouse.position1,gl::mouse_position(),testFromMouse.thickness,testFromMouse.color);
        glm::vec2 t = checkIntersect(testwall.position1,testwall.position2,testFromMouse.position1,gl::mouse_position());
        glm::vec2 intersection = testwall.position1 + t.x* (testwall.position2 - testwall.position1);

        if((t.x >=0 && t.x <= 1) && (t.y >=0 && t.y <= 1)){
            utils::draw_disk(intersection,0.01,glm::vec4(0.f,1.f,0.f,1.f));
        }
        
        
        
    }
    
}

