/**********************************************************************************             
**********************************************************************************/

#include "Engine.h"
#include "Scene.h"
#include "Object.h" 

Scene::Scene()
{

} 

Scene::~Scene()
{ 
    for (auto obj : statics)
        delete obj; 
    for (auto obj : moving)
        delete obj;
} 


void Scene::Add(Object* obj, int type)
{ 
    if (type == STATIC)
        statics.push_back(obj);
    else
        moving.push_back(obj);
} 

void Scene::Remove(Object* obj, int type)
{ 
    if (type == STATIC)
        statics.remove(obj);
    else
        moving.remove(obj);
} 

void Scene::Remove()
{ 
    if (processing == STATIC)
        statics.remove(*it);
    else
        moving.remove(*it);
} 

void Scene::Delete(Object* obj, int type)
{
    toDelete.push_back(ObjectDel{ obj, type });
} 

void Scene::Delete()
{
    toDelete.push_back(ObjectDel{ *it, processing });
} 

uint Scene::Size()
{
    return uint(moving.size() + statics.size());
} 

void Scene::ProcessDeleted()
{ 
    toDelete.sort();
    toDelete.unique();

    for (const auto & [obj, type] : toDelete)
    { 
        delete obj; 
        if (type == STATIC)
            statics.remove(obj);
        else
            moving.remove(obj);
    } 
    toDelete.clear();
} 

void Scene::Update()
{ 
    processing = STATIC;
    for (it = statics.begin(); it != statics.end(); ++it)
        (*it)->Update(); 
    processing = MOVING;
    for (it = moving.begin(); it != moving.end(); ++it)
        (*it)->Update();

    ProcessDeleted();
} 

void Scene::Draw()
{ 
    processing = STATIC;
    for (it = statics.begin(); it != statics.end(); ++it)
        (*it)->Draw(); 
    processing = MOVING;
    for (it = moving.begin(); it != moving.end(); ++it)
        (*it)->Draw();
} 

void Scene::DrawBBox()
{ 
    Engine::renderer->BeginPixels(); 
    for (const auto& obj : statics)
    {
        if (obj->BBox())
        {
            Engine::renderer->Draw(obj->BBox(), 0xffff00ff);
            if (obj->BBox()->Type() == POLYGON_T)
                Engine::renderer->Draw(((Poly*)obj->BBox())->BBox(), 0x7fff00ff);
        }
    } 
    for (const auto& obj : moving)
    {
        if (obj->BBox())
        {
            Engine::renderer->Draw(obj->BBox(), 0xffff00ff);
            if (obj->BBox()->Type() == POLYGON_T)
                Engine::renderer->Draw(((Poly*)obj->BBox())->BBox(), 0x7fff00ff);
        }
    } 
    Engine::renderer->EndPixels();
} 

void Scene::Begin()
{ 
    its = statics.begin();
    itm = moving.begin();
    processing = STATIC;
} 

Object * Scene::Next()
{ 
    if (its != statics.end())
    {  
        it = its++;
        return *it;
    }
    else
        if (itm != moving.end())
        {
            processing = MOVING;
            it = itm++;
            return *it;
        }
        else 
            return nullptr;
} 

bool Scene::Collision(Point* p, Point* q)
{ 
    if (int(p->X()) == int(q->X()) && int(p->Y()) == int(q->Y()))
        return true; 
    return false;
} 

bool Scene::Collision(Point * p, Rect * r)
{ 
    if (p->X() >= r->Left() && p->X() <= r->Right())
        if (p->Y() >= r->Top() && p->Y() <= r->Bottom())
            return true; 
    return false;
} 

bool Scene::Collision(Point * p, Circle * c)
{  
    if (Point::Distance(*p, Point(c->CenterX(), c->CenterY())) <= c->Radius())
        return true;
    else
        return false;
} 

bool Scene::Collision(Point* p, Poly* pol)
{   

    if (Collision(p, pol->BBox()))
    {
        bool crossings = false;
        int nvertex = pol->vertexCount;
        Point * vertex = pol->vertexList;

        float Ix, Iy, Jx, Jy;
        float Ixr, Iyr, Jxr, Jyr;
        float Ixs, Iys, Jxs, Jys;
        const double PIunder180 = 0.0174532925194444; 
        float theta = float(pol->Rotation() * PIunder180);

        for (int i = 0, j = nvertex - 1; i < nvertex; j = i++)
        { 
            Ixr = float(vertex[i].X() * cos(theta) - vertex[i].Y() * sin(theta));
            Iyr = float(vertex[i].X() * sin(theta) + vertex[i].Y() * cos(theta));
            Jxr = float(vertex[j].X() * cos(theta) - vertex[j].Y() * sin(theta));
            Jyr = float(vertex[j].X() * sin(theta) + vertex[j].Y() * cos(theta)); 
            Ixs = Ixr * pol->Scale();
            Iys = Iyr * pol->Scale();
            Jxs = Jxr * pol->Scale();
            Jys = Jyr * pol->Scale(); 
            Ix = pol->X() + Ixs;
            Iy = pol->Y() + Iys;
            Jx = pol->X() + Jxs;
            Jy = pol->Y() + Jys;

            if (((Iy < p->Y()) && (Jy >= p->Y())) || ((Jy < p->Y()) && (Iy >= p->Y())))
                if (Jx - (((Jy - p->Y()) * (Jx - Ix)) / (Jy - Iy)) < p->X())
                    crossings = !crossings;
        }

        return crossings;
    }

    return false;
} 

bool Scene::Collision(Rect * ra, Rect * rb)
{ 
    bool overlapX = (rb->Left() <= ra->Right() && ra->Left() <= rb->Right()); 
    bool overlapY = (rb->Top() <= ra->Bottom() && ra->Top() <= rb->Bottom()); 
    return overlapX && overlapY;
} 

bool Scene::Collision(Rect * r, Circle * c)
{ 
    float px, py; 
    if (c->CenterX() < r->Left())
        px = r->Left();
    else
        if (c->CenterX() > r->Right())
            px = r->Right();
        else
            px = c->CenterX(); 
    if (c->CenterY() < r->Top())
        py = r->Top();
    else
        if (c->CenterY() > r->Bottom())
            py = r->Bottom();
        else
            py = c->CenterY(); 
    Point point(px, py);
    return Collision(&point, c);
} 

bool Scene::Collision(Rect * r, Poly * pol)
{   

    if (Collision(r, pol->BBox()))
    { 
        Point corners[4] =
        {
            Point(r->Left(), r->Top()),
            Point(r->Right(), r->Top()),
            Point(r->Right(), r->Bottom()),
            Point(r->Left(), r->Bottom())
        }; 
        for (int i = 0; i < 4; ++i)
            if (Collision(&corners[i], pol))
                return true; 
        int nv = pol->vertexCount;
        Point * v = pol->vertexList; 
        const double PIunder180 = 0.0174532925194444;
        float theta = float(pol->Rotation() * PIunder180); 
        float pX, pY;
        float pXr, pYr;
        float pXs, pYs;

        for (int i = 0; i < nv; ++i)
        { 
            pXr = float(v[i].X() * cos(theta) - v[i].Y() * sin(theta));
            pYr = float(v[i].X() * sin(theta) + v[i].Y() * cos(theta)); 
            pXs = pXr * pol->Scale();
            pYs = pYr * pol->Scale(); 
            pX = pol->X() + pXs;
            pY = pol->Y() + pYs;

            Point p(pX, pY);

            if (Collision(&p, r))
                return true;
        }
    }

    return false;
} 

bool Scene::Collision(Circle * ca, Circle * cb)
{  
    float deltaX = abs(ca->CenterX() - cb->CenterX());
    float deltaY = abs(ca->CenterY() - cb->CenterY()); 
    float distance = float(sqrt(double(deltaX) * double(deltaX) + double(deltaY) * double(deltaY)));  
    if (distance <= (ca->Radius() + cb->Radius()))
        return true; 
    return false;
} 

bool Scene::Collision(Circle* c, Poly* pol)
{   

    if (Collision(c, pol->BBox()))
    { 
        int nv = pol->vertexCount;
        Point * v = pol->vertexList;   
        const double PIunder180 = 0.0174532925194444;
        float theta = float(pol->Rotation() * PIunder180); 
        float pX, pY;
        float pXr, pYr;
        float pXs, pYs;

        for (int i = 0; i < nv; ++i)
        { 
            pXr = float(v[i].X() * cos(theta) - v[i].Y() * sin(theta));
            pYr = float(v[i].X() * sin(theta) + v[i].Y() * cos(theta)); 
            pXs = pXr * pol->Scale();
            pYs = pYr * pol->Scale(); 
            pX = pol->X() + pXs;
            pY = pol->Y() + pYs;

            Point p(pX, pY);

            if (Collision(&p, c))
                return true;
        }
    }

    return false;

} 

bool Scene::Collision(Poly* pa, Poly* pb)
{   

    if (Collision(pa->BBox(), pb->BBox()))
    { 
        int nva = pa->vertexCount;
        Point * va = pa->vertexList;
        
        float pX, pY;
        float pXr, pYr;
        float pXs, pYs;
        float theta; 
        const double PIunder180 = 0.0174532925194444;
        theta = float(pa->Rotation() * PIunder180); 
        for (int i = 0; i < nva; ++i)
        { 
            pXr = float(va[i].X() * cos(theta) - va[i].Y() * sin(theta));
            pYr = float(va[i].X() * sin(theta) + va[i].Y() * cos(theta)); 
            pXs = pXr * pa->Scale();
            pYs = pYr * pa->Scale(); 
            pX = pa->X() + pXs;
            pY = pa->Y() + pYs;

            Point p(pX, pY);

            if (Collision(&p, pb))
                return true;
        } 
        int nvb = pb->vertexCount;
        Point * vb = pb->vertexList; 
        theta = float(pb->Rotation() * PIunder180); 
        for (int i = 0; i < nvb; ++i)
        { 
            pXr = float(vb[i].X() * cos(theta) - vb[i].Y() * sin(theta));
            pYr = float(vb[i].X() * sin(theta) + vb[i].Y() * cos(theta)); 
            pXs = pXr * pb->Scale();
            pYs = pYr * pb->Scale(); 
            pX = pb->X() + pXs;
            pY = pb->Y() + pYs;

            Point p(pX, pY);

            if (Collision(&p, pa))
                return true;
        }
    }

    return false;
} 

bool Scene::Collision(Mixed * m, Geometry * s)
{
    bool collision = false; 
    auto i = m->shapes.begin();

    while (!collision && i != m->shapes.end())
    {
        switch ((*i)->Type())
        { 
        case POINT_T:
            switch (s->Type())
            {
            case POINT_T:     collision = Collision((Point*)(*i), (Point*)s); break;
            case CIRCLE_T:    collision = Collision((Point*)(*i), (Circle*)s); break;
            case RECTANGLE_T: collision = Collision((Point*)(*i), (Rect*)s); break;
            case POLYGON_T:   collision = Collision((Point*)(*i), (Poly*)s); break;
            case MIXED_T:     collision = Collision((Point*)(*i), (Mixed*)s); break;
            }
            break; 
        case CIRCLE_T:
            switch (s->Type())
            {
            case POINT_T:     collision = Collision((Circle*)(*i), (Point*)s); break;
            case CIRCLE_T:    collision = Collision((Circle*)(*i), (Circle*)s); break;
            case RECTANGLE_T: collision = Collision((Circle*)(*i), (Rect*)s); break;
            case POLYGON_T:   collision = Collision((Circle*)(*i), (Poly*)s); break;
            case MIXED_T:     collision = Collision((Circle*)(*i), (Mixed*)s); break;
            }
            break; 
        case RECTANGLE_T:
            switch (s->Type())
            {
            case POINT_T:     collision = Collision((Rect*)(*i), (Point*)s); break;
            case CIRCLE_T:    collision = Collision((Rect*)(*i), (Circle*)s); break;
            case RECTANGLE_T: collision = Collision((Rect*)(*i), (Rect*)s); break;
            case POLYGON_T:   collision = Collision((Rect*)(*i), (Poly*)s); break;
            case MIXED_T:     collision = Collision((Rect*)(*i), (Mixed*)s); break;
            }
            break; 
        case POLYGON_T:
            switch (s->Type())
            {
            case POINT_T:     collision = Collision((Poly*)(*i), (Point*)s); break;
            case CIRCLE_T:    collision = Collision((Poly*)(*i), (Circle*)s); break;
            case RECTANGLE_T: collision = Collision((Poly*)(*i), (Rect*)s); break;
            case POLYGON_T:   collision = Collision((Poly*)(*i), (Poly*)s); break;
            case MIXED_T:     collision = Collision((Poly*)(*i), (Mixed*)s); break;
            }
            break; 
        case MIXED_T:
            collision = Collision((Mixed*)(*i), s);
            break;
        } 
        ++i;
    }

    return collision;
} 

bool Scene::Collision(Object* oa, Object* ob)
{ 
    if (!(oa->BBox() && ob->BBox()))
        return false;

    switch (oa->BBox()->Type())
    { 
    case POINT_T:

        switch (ob->BBox()->Type())
        {
        case POINT_T: 
            return Collision((Point*)oa->BBox(), (Point*)ob->BBox());

        case CIRCLE_T: 
            return Collision((Point*)oa->BBox(), (Circle*)ob->BBox());

        case RECTANGLE_T: 
            return Collision((Point*)oa->BBox(), (Rect*)ob->BBox());

        case POLYGON_T: 
            return Collision((Point*)oa->BBox(), (Poly*)ob->BBox());

        case MIXED_T: 
            return Collision(oa->BBox(), (Mixed*)ob->BBox());

        default: 
            return false;
        } 
    case CIRCLE_T:

        switch (ob->BBox()->Type())
        {
        case POINT_T: 
            return Collision((Circle*)oa->BBox(), (Point*)ob->BBox());

        case CIRCLE_T: 
            return Collision((Circle*)oa->BBox(), (Circle*)ob->BBox());

        case RECTANGLE_T: 
            return Collision((Circle*)oa->BBox(), (Rect*)ob->BBox());

        case POLYGON_T: 
            return Collision((Circle*)oa->BBox(), (Poly*)ob->BBox());

        case MIXED_T: 
            return Collision(oa->BBox(), (Mixed*)ob->BBox());

        default: 
            return false;
        } 
    case RECTANGLE_T:

        switch (ob->BBox()->Type())
        {
        case POINT_T: 
            return Collision((Rect*)oa->BBox(), (Point*)ob->BBox());

        case CIRCLE_T: 
            return Collision((Rect*)oa->BBox(), (Circle*)ob->BBox());

        case RECTANGLE_T: 
            return Collision((Rect*)oa->BBox(), (Rect*)ob->BBox());

        case POLYGON_T: 
            return Collision((Rect*)oa->BBox(), (Poly*)ob->BBox());

        case MIXED_T: 
            return Collision(oa->BBox(), (Mixed*)ob->BBox());

        default: 
            return false;
        } 
    case POLYGON_T:

        switch (ob->BBox()->Type())
        {
        case POINT_T: 
            return Collision((Poly*)oa->BBox(), (Point*)ob->BBox());

        case CIRCLE_T: 
            return Collision((Poly*)oa->BBox(), (Circle*)ob->BBox());

        case RECTANGLE_T: 
            return Collision((Poly*)oa->BBox(), (Rect*)ob->BBox());

        case POLYGON_T: 
            return Collision((Poly*)oa->BBox(), (Poly*)ob->BBox());

        case MIXED_T: 
            return Collision(oa->BBox(), (Mixed*)ob->BBox());

        default: 
            return false;
        } 
    case MIXED_T: 
        return Collision((Mixed*)oa->BBox(), ob->BBox()); 
    default:
        return false;
    }
} 

void Scene::CollisionDetection()
{ 
    list<Object*>::iterator start, end; 
    collisions.clear();    
    if (moving.size() >= 2)
    { 
        end = moving.end();
        end--; 
        for (auto i = moving.begin(); i != end; ++i)
        {
            start = i; 
            for (auto j = ++start; j != moving.end(); ++j)
            {
                if (Collision(*i, *j))
                    collisions.push_back(ObjectPair(*i, *j));
            }
        }
    } 
    for (auto i : moving)
    {
        for (auto j : statics)
        {
            if (Collision(i, j))
                collisions.push_back(ObjectPair(i, j));
        }
    }   

    if (!collisions.empty())
    {
        for (const auto & [a,b] : collisions)
        {
            a->OnCollision(b);
            b->OnCollision(a);
        }
    }

    ProcessDeleted();
} 
