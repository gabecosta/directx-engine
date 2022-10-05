/**********************************************************************************        
**********************************************************************************/

#include "Particles.h"
#include "Geometry.h"
#include <cmath> 

Particles::Particles(const Generator & generator) : config(generator)
{
    sprite = new Sprite(config.imgFile);
    timer.Start();
} 

Particles::~Particles()
{
    delete sprite;
    for (const auto & p : particles)
        delete p;
} 

void Particles::Build(float posX, float posY)
{ 
    Particle * particle = new Particle(); 
    particle->x = posX;
    particle->y = posY; 
    float angle = config.angle + spread.Rand(); 
    if (angle > 360)
        angle -= 360;
    else 
        if (angle < 0)
        angle += 360; 
    particle->speed.RotateTo(angle);    
    particle->speed.ScaleTo(speed.Rand());
    particle->timestamp = timer.Stamp(); 
    particles.push_back(particle);
} 

void Particles::Generate(float x, float y, int count)
{
    if (count == 1)
    { 
        if (timer.Elapsed(config.frequency)) 
        {
            Build(x, y);
            timer.Start();
        }
    } 
    else 
    { 
        for (int i = 0; i < count; ++i)
            Build(x, y);
    }
} 

void Particles::Update(float delta)
{
    float lifePercent = 0;
    float timeElapsed = 0; 
    auto it = particles.begin();
    while (it != particles.end())
    { 
        Particle * p = *it; 
        p->x += p->speed.XComponent() * delta;
        p->y -= p->speed.YComponent() * delta; 
        timeElapsed = timer.Elapsed(p->timestamp);
        lifePercent = timeElapsed/config.lifetime; 
        if (lifePercent > config.percentToDim)
        { 
            config.color.a = 1.0f - pow(lifePercent, 3.0f);
        }

        if (timeElapsed > config.lifetime)
        { 
            delete p;
            it = particles.erase(it);
        }
        else
        { 
            ++it;
        }
    }
} 

void Particles::Draw(float z, float factor)
{
    float lifePercent = 0.0f;
    float timeElapsed = 0.0f; 
    for (Particle * p : particles)
    { 
        timeElapsed = timer.Elapsed(p->timestamp);
        lifePercent = timeElapsed/config.lifetime; 
        float scale = 1.0f + pow(lifePercent, 2.0f) * factor; 
        sprite->Draw(p->x, p->y, z, scale, -p->speed.Angle(), config.color);
    }    
} 
