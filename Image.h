#ifndef _PROGJOGOS_IMAGE_H_
#define _PROGJOGOS_IMAGE_H_

#ifdef ENGINE_EXPORTS
#define DLL __declspec( dllexport ) 
#else
#define DLL __declspec( dllimport )
#endif

#include "Types.h"        
#include "Texture.h"      
#include <string>        
using std::string;       
class DLL Image
{
private:
    ID3D11ShaderResourceView * textureView;      
    uint width;                                  
    uint height;                                 

public:
    Image(string filename);                      

    Image(string filename,                      
        uint tileWidth,                          
        uint tileHeight,                        
        uint numColumns,                         
        int* tiledMap,                           
        uint tiledMapCols,                       
        uint tiledMapRows);                      

    ~Image();                                    

    uint Width() const;                          
    uint Height() const;                         
    ID3D11ShaderResourceView * View() const;     
};

inline uint Image::Width() const
{ 
    return width; 
}

inline uint Image::Height() const
{ 
    return height; 
}

inline ID3D11ShaderResourceView * Image::View() const
{ 
    return textureView; 
}

#endif




