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

void draw_parametric(std::function<glm::vec2(float)> const& parametric)
{
    
}

glm::vec2 lerp(glm::vec2 P1,glm::vec2 P2,float t){

    return P1 +(P2-P1) * t;

}

glm::vec2 deCasteljau(const std::vector<glm::vec2>& points, float t) {
    if (points.size() == 1) {
        return points[0];
    }
    
    std::vector<glm::vec2> newPoints;
    for (size_t i = 0; i < points.size() - 1; i++) {
        newPoints.push_back(lerp(points[i], points[i+1], t));
    }
    
    return deCasteljau(newPoints, t);

}

glm::vec2 bezier1(glm::vec2 pos1,glm::vec2 pos2,float t){
    return lerp(pos1,pos2,t);
}

glm::vec2 bezier2(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, float t) {
    std::vector<glm::vec2> points = {p0, p1, p2};
    return deCasteljau(points, t);
}

glm::vec2 bernstein2(const glm::vec2& p0,const glm::vec2& p1,const glm::vec2& p2,float t) {
    float u = 1 - t;
    return p0 * (u * u) + p1 * (2 * u * t) + p2 * (t * t);
}

glm::vec2 bezier3(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, float t) {
    std::vector<glm::vec2> points = {p0, p1, p2, p3};
    return deCasteljau(points, t);
}


glm::vec2 bezier3Tangent(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, float t) {
    float t2 = t * t;
    
    glm::vec2 tangent = 3.0f * (1.0f - t) * (1.0f - t) * (p1 - p0) +
                       6.0f * (1.0f - t) * t * (p2 - p1) +
                       3.0f * t2 * (p3 - p2);
                       
    return glm::normalize(tangent);
}

glm::vec2 bezier3Normal(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, float t) {
    glm::vec2 tangent = bezier3Tangent(p0, p1, p2, p3, t);
    return glm::vec2(-tangent.y, tangent.x);
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
        glm::vec2 gravity = glm::vec2(0,-0.5f)*mass;
        glm::vec2 force;
        glm::vec2 acceleration;
        glm::vec2 friction;
        glm::vec2 spring;

        float maximumAge = utils::rand(5.f,10.f);
        float age = 0.f;

        float radius=0.01f;
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
    std::vector listParticule =  std::vector<particule>(300);
    wall testwall;
    wall testFromMouse;

    
  
    sphere maSphereDeSpawn;
    maSphereDeSpawn.origin=glm::vec2(-0.5,-0.5);
    maSphereDeSpawn.radius=0.5;
    

    std::vector points = std::vector<glm::vec2>(300);
    std::vector normals = std::vector<glm::vec2>(300);

    glm::vec2 pos1=glm::vec2(-0.5,-0.5);
    glm::vec2 pos2=glm::vec2(0.5,0.5);



    glm::vec2 p0 = glm::vec2(-0.8, -0.8);
    glm::vec2 p2 = glm::vec2(0.4, -0.8);
    glm::vec2 p3 = glm::vec2(0.8, 0.8);

    bool particulesSpawned=false;

    while (gl::window_is_open())
    {

        
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        utils::draw_disk(p0, 0.02f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)); 
        utils::draw_disk(gl::mouse_position(), 0.02f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)); 
        utils::draw_disk(p2, 0.02f, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)); 
        utils::draw_disk(p3, 0.02f, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

        utils::draw_line(p0, gl::mouse_position(), 0.005f, glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
        utils::draw_line(gl::mouse_position(), p2, 0.005f, glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
        utils::draw_line(p2, p3, 0.005f, glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));

        glm::vec2 previousPoint = glm::vec2(0, 0);
        
        

        for(int i =0; i< points.size()-1 ;i++){
            
            glm::vec2& point= points[i];
            float t = static_cast<float>(i) /(listParticule.size() -1);

            point = bezier3(p0, gl::mouse_position(), p2,p3, t);
            normals[i]=bezier3Normal(p0, gl::mouse_position(), p2, p3, t);


            if(previousPoint != glm::vec2(0,0)){
                utils::draw_line(previousPoint,point,0.001,glm::vec4(1,1,1,1));
            }

            previousPoint = point;
            if(!particulesSpawned){
                particule& uneParticule = listParticule[i];
                uneParticule.position= point;
            }
            

        };
        for(int i =0; i< listParticule.size()-1 ;i++){
            
            particule& Particule= listParticule[i];
            Particule.acceleration= normals[i] * 0.5f;
            Particule.velocity= Particule.acceleration * gl::delta_time_in_seconds();
            utils::draw_disk(Particule.position,Particule.radius,Particule.color);
            Particule.position+= Particule.velocity * 10.f * gl::delta_time_in_seconds();
            
        }
        particulesSpawned=true;
    }
    
}

