#ifndef _PROGJOGOS_VECTOR_H_
#define _PROGJOGOS_VECTOR_H_

#ifdef ENGINE_EXPORTS
#define DLL __declspec( dllexport ) 
#else
#define DLL __declspec( dllimport )
#endif

#include <cmath>

class DLL Vector
{
private:
    static const double PI;             
    float angle;                        
    float magnitude;                    

public:
    Vector();                           
    Vector(float ang, float mag);       

    void Scale(float factor);           
    void ScaleTo(float value);          
    void Rotate(float theta);           
    void RotateTo(float value);        
    void Add(const Vector& v);          
    
    float Angle() const;                
    float Magnitude() const;            
    float XComponent() const;           
    float YComponent() const;           
    float Radians() const;              
}; 

inline void Vector::ScaleTo(float value)
{ magnitude = value; }

inline void Vector::RotateTo(float value)
{ angle = value; }

inline float Vector::Angle() const
{ return angle; }

inline float Vector::Magnitude() const
{ return magnitude; }

inline float Vector::XComponent() const
{ return magnitude * cos(Radians()); }    

inline float Vector::YComponent() const
{ return magnitude * sin(Radians()); }    

inline float Vector::Radians() const
{ return float(angle * PI / 180.0); }

#endif