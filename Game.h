#ifndef _PROGJOGOS_GAME_H_
#define _PROGJOGOS_GAME_H_

#ifdef ENGINE_EXPORTS
#define DLL __declspec( dllexport ) 
#else
#define DLL __declspec( dllimport )
#endif

#include "Types.h"
#include "Window.h"

class DLL Game
{
private:
    static Canvas world;                            

protected:
    static Window* & window;                        
    static Game*   & game;                          
    static float   & gameTime;                     

public:
    ViewPort viewport;                              
    
    Game();                                        

    virtual ~Game();                               
    virtual void OnPause();                         

    static void Size(float width, float height);  
    static float Width();                          
    static float Height();                         
    static float CenterX();                        
    static float CenterY();                         

    virtual void Init() = 0;                        
    virtual void Update() = 0;                      
    virtual void Draw() = 0;                        
    virtual void Finalize() = 0;                    
};

inline float Game::Width()
{ return world.width; }

inline float Game::Height()
{ return world.height; }

inline float Game::CenterX()
{ return world.centerX; }

inline float Game::CenterY()
{ return world.centerY; }

#endif