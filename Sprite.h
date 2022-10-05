#ifndef _PROGJOGOS_SPRITE_H_
#define _PROGJOGOS_SPRITE_H_

#ifdef ENGINE_EXPORTS
#define DLL __declspec( dllexport ) 
#else
#define DLL __declspec( dllimport )
#endif

#include "Image.h"
#include "Types.h"
#include <DirectXMath.h>
using namespace DirectX;

struct DLL Layer
{
    static const float FRONT;
    static const float UPPER;
    static const float MIDDLE;
    static const float LOWER;
    static const float BACK;
};

struct DLL Color
{
    float r;
    float g;
    float b;
    float a;

    Color() : 
        r(0), g(0), b(0), a(1) {}

    Color(float red, float green, float blue, float alpha) :
        r(red), g(green), b(blue), a(alpha) {}
};

struct SpriteData
{
    float x, y;
    float scale;
    float depth;
    float rotation;
    uint  width;
    uint  height;
    ID3D11ShaderResourceView* texture;
    XMFLOAT2 texCoord;
    XMFLOAT2 texSize;
    Color color;

    SpriteData() :
        x(0), y(0),
        scale(0), depth(0), rotation(0),
        width(0), height(0),
        texture(nullptr),
        texCoord(XMFLOAT2(0, 0)),
        texSize(XMFLOAT2(0, 0)),
        color(Color(1, 1, 1, 1)) {}
};

class DLL Sprite
{
private:
    SpriteData sprite;              
    const Image * image;            
    bool localImage;                

public:
    Sprite(string filename);        
    Sprite(const Image * img);      
    ~Sprite();                      

    int Width();                    
    int Height();                   

    void Draw(                                                
        float x, float y, float z = Layer::MIDDLE,            
        float scale = 1.0f, float rotation = 0.0f,           
        Color color = { 1, 1, 1, 1 });                        

    void Draw(                                                
        float x, float y, float z,                            
        float scale, float rotation,                          
        const Color & color, const RECT & region);            
};

inline int Sprite::Width() 
{ 
    return image->Width(); 
}

inline int Sprite::Height() 
{ 
    return image->Height(); 
}

#endif