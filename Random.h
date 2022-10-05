#ifndef _PROGJOGOS_RANDOM_H_
#define _PROGJOGOS_RANDOM_H_

#ifdef ENGINE_EXPORTS
#define DLL __declspec( dllexport ) 
#else
#define DLL __declspec( dllimport )
#endif

#include "Types.h"                          
#include <random>                          
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;
using std::uniform_real_distribution;

class DLL Random
{
protected:
    static random_device seed;             
    static mt19937 mt;                      
};


class DLL RandI : public Random
{
private:
    uniform_int_distribution<int> dist;    

public:
    RandI(int min, int max);                
    int Rand();                             
};

class DLL RandF : public Random
{
private:
    uniform_real_distribution<float> dist;  

public:
    RandF(float min, float max);            
    float Rand();                           
};

#endif
