#include "opengl-framework/opengl-framework.hpp"
#include "utils.hpp"
#include "iostream"

glm::vec2 findT(glm::vec2 obj1Pos1,glm::vec2 obj1Pos2,glm::vec2 obj2Pos1, glm::vec2 obj2Pos2){
    glm::vec2 direction1= obj1Pos2-obj1Pos1;
    glm::vec2 direction2= obj2Pos2-obj2Pos1;
    glm::mat2 myMatrice = glm::mat2(direction1,-direction2);
    glm::mat2 matriveReverse = glm::inverse(myMatrice);
    glm::vec2 originObj1Obj2= obj2Pos1-obj1Pos1;
    glm::vec2 result= matriveReverse*originObj1Obj2;
    return result;
}
glm::vec2 checkIntersect(glm::vec2 obj1Pos1,glm::vec2 obj1Pos2,glm::vec2 obj2Pos1, glm::vec2 obj2Pos2){

    glm::vec2 t = findT(obj1Pos1,obj1Pos2,obj2Pos1,obj2Pos2);
    glm::vec2 result= obj1Pos1 +t.x * (obj1Pos2-obj1Pos1);
    return result;
}

glm::vec2 getNormalWall(glm::vec2 wallPos1,glm::vec2 wallPos2){
    glm::vec2 direction= wallPos2-wallPos1;
    return glm::normalize(glm::vec2(-direction.y,direction.x));
}


glm::vec2 reflectParticule(glm::vec2 wallPos1,glm::vec2 wallPos2,glm::vec2 particulePos,glm::vec2 velocity){
    glm::vec2 particuleDirection = particulePos - (particulePos * 1.1f);
    glm::vec2 t=findT(wallPos1,wallPos2,particuleDirection,(particulePos * 1.1f));
    glm::vec2 intersection = checkIntersect(wallPos1,wallPos2,particuleDirection,(particulePos * 1.1f));
    glm::vec2 normal = getNormalWall(wallPos1,wallPos2);
    return glm::reflect(velocity,normal);
}



int main()
{
    gl::init("Particules!");
    gl::maximize_window();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    struct Parallelogramme
    {
        glm::vec2 origin;     // Point de départ
        glm::vec2 u;          // Vecteur côté 1
        glm::vec2 v;          // Vecteur côté 2

    };

    struct sphere
    {
        glm::vec2 origin;
        float radius;
    };

    struct particule
    {
        glm::vec2 position = glm::vec2(utils::rand(-1,1),utils::rand(-1,1));

        float random1=utils::rand(0,360);
        float random2=utils::rand(0,360);

        float cosResult = cos(random1);
        float resultSin = sin(random2);

        float airFriction=3.0f;
        float SpringStiffness = 5.f;
        
        glm::vec2 velocity = glm::vec2(cosResult,resultSin);
        
        float mass = utils::rand(1,5);
        glm::vec2 gravity = glm::vec2(0,-3.f)*mass;
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
    std::vector listParticule =  std::vector<particule>(5000);
    wall testwall;
    wall testFromMouse;

  
    sphere maSphereDeSpawn;
    maSphereDeSpawn.origin=glm::vec2(-0.5,-0.5);
    maSphereDeSpawn.radius=0.5;
    

    while (gl::window_is_open())
    {

        
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        

        for(particule& i : listParticule){
           
            float angle = utils::rand(0.f, 2.f * 3.141592653589793f);
            float radius = maSphereDeSpawn.radius * sqrt(utils::rand(0.f, 1.f));
            i.position = maSphereDeSpawn.origin + glm::vec2(radius * cos(angle), radius * sin(angle));
            utils::draw_disk(i.position, i.radius, i.color);
        };
        
    }
    
}

