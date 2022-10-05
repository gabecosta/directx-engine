#ifndef _PROGJOGOS_BACKGROUND_H_
#define _PROGJOGOS_BACKGROUND_H_

#ifdef ENGINE_EXPORTS
#define DLL __declspec( dllexport ) 
#else
#define DLL __declspec( dllimport )
#endif


#include "Types.h"                     
#include "Image.h"                      
#include "Sprite.h"                    
#include <string>                       
using std::string;                      


class DLL Background
{
private:
    SpriteData spriteData;              
    const Image * image;                
    bool localImage;                    

public:
    Background(string filename);       
    Background(const Image* img);       
    ~Background();                      

    void Draw(ViewPort & sourceRect);  
    float Width();                      
    float Height();                     
}; 

inline float Background::Width()
{ return float(image->Width()); }

inline float Background::Height()
{ return float(image->Height()); }


#endif