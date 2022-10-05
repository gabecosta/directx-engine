/**********************************************************************************        
**********************************************************************************/

#include "Random.h" 

random_device Random::seed;
mt19937 Random::mt { seed() }; 

RandI::RandI(int min, int max): dist(min, max) {}

int RandI::Rand() { return dist(mt); } 

RandF::RandF(float min, float max) : dist(min, max) {}

float RandF::Rand() { return dist(mt); } 
