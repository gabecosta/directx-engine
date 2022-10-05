#ifndef _PROGJOGOS_FONT_H_
#define _PROGJOGOS_FONT_H_

#ifdef ENGINE_EXPORTS
#define DLL __declspec( dllexport ) 
#else
#define DLL __declspec( dllimport )
#endif

#include "TileSet.h"                            
#include "Sprite.h"                             
#include "Types.h"                             
#include <string>                               
using std::string;                             

class DLL Font
{
private:
    TileSet * fontTiles;                        
    SpriteData sprite;                         
    short spacing[256];                         
    bool proportional;                          
    int  charWidth;                             
    
public:
    Font(const string & filename);              
    ~Font();                                    

    void Spacing(int width);                    
    void Spacing(const string & filename);      

    void Draw(                                  
        float x,                                
        float y,                               
        string text,                            
        Color color = { 1, 1, 1, 1 },          
        float depth = 0.0f,                    
        float scale = 1.0f,                     
        float rotation = 0.0f);                
};
#endif