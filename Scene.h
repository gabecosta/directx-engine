#ifndef _PROGJOGOS_SCENE_H_
#define _PROGJOGOS_SCENE_H_

#ifdef ENGINE_EXPORTS
#define DLL __declspec( dllexport ) 
#else
#define DLL __declspec( dllimport )
#endif

#include "Geometry.h"                         
#include <list>                                
using std::list;                              
using std::pair;                                
class Object;                                 

enum ObjectGroup
{
    STATIC,                                   
    MOVING                                     
};

using ObjectPair = pair<Object*, Object*>;    
using ObjectDel  = pair<Object*, int>;         

class DLL Scene
{
private:
    list<Object*> statics;                     
    list<Object*> moving;                       
    
    list<ObjectPair> collisions;                
    list<ObjectDel> toDelete;                   

    list<Object*>::iterator its;                
    list<Object*>::iterator itm;                
    list<Object*>::iterator it;                 

    int processing = STATIC;                    

    bool Collision(Point * p, Point * q);       
    bool Collision(Point * p, Rect * r);       
    bool Collision(Point * p, Circle * c);     
    bool Collision(Point * p, Poly * pol);      

    bool Collision(Rect * ra, Rect * rb);       
    bool Collision(Rect * r, Point * p);       
    bool Collision(Rect * r, Circle * c);      
    bool Collision(Rect* r, Poly* pol);       

    bool Collision(Circle * ca, Circle * cb);   
    bool Collision(Circle * c, Point * p);      
    bool Collision(Circle * c, Rect * r);       
    bool Collision(Circle * c, Poly * pol);     

    bool Collision(Poly* pa, Poly* pb);         
    bool Collision(Poly* pol, Point* p);       
    bool Collision(Poly* pol, Rect* r);         
    bool Collision(Poly* pol, Circle* c);      

    bool Collision(Mixed * m, Geometry * s);    
    bool Collision(Geometry * s, Mixed * m);    

    void ProcessDeleted();                      
    
public:
    Scene();                                    
    ~Scene();                                   

    void Add(Object * obj, int type);           
    void Remove(Object* obj, int type);         
    void Remove();                              
    void Delete(Object * obj, int type);       
    void Delete();                              
    uint Size();                                

    void Begin();                             
    Object * Next();                          

    void Update();                           
    void Draw();                               
    void DrawBBox();                          

    bool Collision(Object * oa, Object * ob);   
    void CollisionDetection();               
};

inline bool Scene::Collision(Rect* r, Point* p)
{ return Collision(p, r); }

inline bool Scene::Collision(Circle* c, Point* p)
{ return Collision(p, c); }

inline bool Scene::Collision(Circle* c, Rect* r)
{     return Collision(r, c); }

inline bool Scene::Collision(Poly* pol, Point* p)
{ return Collision(p, pol); }

inline bool Scene::Collision(Poly* pol, Rect* r)
{ return Collision(r, pol); }

inline bool Scene::Collision(Poly* pol, Circle* c)
{ return Collision(c, pol); }

inline bool Scene::Collision(Geometry* s, Mixed* m)
{     return Collision(m, s); }


#endif