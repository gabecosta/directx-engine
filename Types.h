#ifndef _PROGJOGOS_TYPES_H_
#define _PROGJOGOS_TYPES_H_

#ifdef ENGINE_EXPORTS
#define DLL __declspec( dllexport ) 
#else
#define DLL __declspec( dllimport )
#endif

#pragma warning(disable: 4251)

using uchar  = unsigned char;
using ushort = unsigned short;
using ulong  = unsigned long;
using llong  = long long;
using ullong = unsigned long long;
using uint   = unsigned int;

struct Canvas
{
    float width;        
    float height;       
    float centerX;     
    float centerY;      
};

struct DLL ViewPort
{
    float left;         
    float top;         
    float right;       
    float bottom;      
};

#endif