#ifndef _PROGJOGOS_PARTICLES_H_
#define _PROGJOGOS_PARTICLES_H_

#ifdef ENGINE_EXPORTS
#define DLL __declspec( dllexport ) 
#else
#define DLL __declspec( dllimport )
#endif
#include "Sprite.h"                                           
#include "Timer.h"                                            
#include "Types.h"                                             
#include "Vector.h"                                             
#include "Random.h"                                             
#include <list>                                                 
using std::list;

struct DLL Generator
{
    string imgFile;                                             
    float  angle;                                               
    float  spread;                                              
    float  lifetime;                                            
    float  frequency;                                           
    float  percentToDim;                                        
    float  minSpeed;                                            
    float  maxSpeed;                                            
    Color  color;                                               
};

struct Particle
{
    float  x, y;                                                
    Vector speed;                                               
    llong  timestamp;                                          
};

class DLL Particles
{
private:
    Generator config;                                           
    Sprite * sprite;                                            
    list<Particle*> particles;                                  

    Timer timer;                                                
    RandF spread 
    { 
        -config.spread/2, config.spread/2 
    };        
    RandF speed 
    { 
        config.minSpeed, config.maxSpeed 
    };       
    void Build(float posX, float posY);                         

public:
    Particles(const Generator & generator);                     
    ~Particles();                                               

    uint Size();                                                
    bool Inactive();                                            
    Generator& Config();                                        
    void Generate(float x, float y, int count = 1);             
    void Update(float delta);                                  
    void Draw(float z, float factor = 0.0f);                         
}; 

inline uint Particles::Size()
{ 
    return uint(particles.size()); 
}

inline bool Particles::Inactive()
{ 
    return (particles.size() == 0); 
}

inline Generator & Particles::Config()
{ 
    return config; 
}

#endif