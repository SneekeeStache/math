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

/*
glm::vec2 checkCircleIntersect(){

}
*/

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

    struct particule
    {

        //glm::vec2 position = glm::vec2(utils::rand(-1,1),utils::rand(-1,1));

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

        float a = utils::rand(0.f, 1.f);
        float b = utils::rand(0.f, 1.f);
    };

    struct wall
    {
        glm::vec2 position1= glm::vec2(utils::rand(-1,1),utils::rand(-1,1));
        glm::vec2 position2= glm::vec2(utils::rand(-1,1),utils::rand(-1,1));
        float thickness=0.01;
        glm::vec4 color = glm::vec4(utils::rand(0,1),utils::rand(0,1),utils::rand(0,1),1);
        
    };

    
    

    // TODO: create an array of particles
    std::vector listParticule =  std::vector<particule>(300);
    wall testwall;
    wall testFromMouse;

    
    Parallelogramme maZoneDeSpawn;
    maZoneDeSpawn.origin=glm::vec2(-0.5,0);
    maZoneDeSpawn.u=glm::vec2(1.f,-0.6);
    maZoneDeSpawn.v=glm::vec2(0.8f,1.f);
    
    

    while (gl::window_is_open())
    {

        
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        // TODO update particles
        // TODO render particles
        
        /*
        utils::draw_line(testwall.position1,testwall.position2,testwall.thickness,testwall.color);
        utils::draw_line(testFromMouse.position1,gl::mouse_position(),testFromMouse.thickness,testFromMouse.color);
        
        glm::vec2 intersection = checkIntersect(testwall.position1,testwall.position2,testFromMouse.position1,gl::mouse_position());
        glm::vec2 t = findT(testwall.position1,testwall.position2,testFromMouse.position1,gl::mouse_position());

        if((t.x >=0 && t.x <= 1) && (t.y >=0 && t.y <= 1)){
            utils::draw_disk(intersection,0.01,glm::vec4(0.f,1.f,0.f,1.f));
        }
        */

        for(particule& i : listParticule){
            /*
            i.currentRadius= i.radius * (1.0f - i.age / i.maximumAge);
            if(i.age < i.maximumAge){
                utils::draw_disk(i.position,i.currentRadius,i.color);
            }
            i.friction= -i.airFriction * i.velocity;
            i.spring = i.SpringStiffness * (gl::mouse_position() - i.position); 
            i.force = i.gravity + i.friction + i.spring;
            i.acceleration= i.force/ i.mass;
            i.velocity+= i.acceleration * gl::delta_time_in_seconds();
            glm::vec2 tWallPartCheck=findT(testwall.position1,testwall.position2,i.position,(i.position * 1.1f));
            if((tWallPartCheck.x >=0 && tWallPartCheck.x <= 1) && (tWallPartCheck.y >=0 && tWallPartCheck.y <= 1)){
                i.velocity= reflectParticule(testwall.position1,testwall.position2,i.position,i.velocity);
            }
            i.position+= i.velocity * gl::delta_time_in_seconds();
            

            i.age+= gl::delta_time_in_seconds();
            */

            
            
            glm::vec2 spawnArea=maZoneDeSpawn.origin+i.a*maZoneDeSpawn.u+i.b*maZoneDeSpawn.v;
            utils::draw_disk(spawnArea,i.radius,i.color);
        };
        
    }
    
}

