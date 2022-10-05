/**********************************************************************************          
**********************************************************************************/

#include <cmath>
#include "Geometry.h"   

Geometry::Geometry()
{ 
    x = y = 0.0f;
    scale = 1.0f;
    rotation = 0.0f;
    type = UNKNOWN_T; 
} 
    
Geometry::~Geometry()
{
}   

Point::Point()
{
    type = POINT_T;
} 

Point::Point(float posX, float posY)
{
    MoveTo(posX, posY);
    type = POINT_T;
} 

Point::Point(int posX, int posY)
{
    MoveTo(float(posX), float(posY)); 
    type = POINT_T;
} 

float Point::Distance(const Point& pa, const Point& pb)
{  
    float deltaX = abs(pb.x - pa.x);
    float deltaY = abs(pb.y - pa.y); 
    return sqrt(deltaX*deltaX + deltaY*deltaY);
}   

Line::Line()
{ 
    a = Point();
    b = Point();
    type = LINE_T;
} 

Line::Line(float x1, float y1, float x2, float y2)
{ 
    a = Point(x1,y1);    
    b = Point(x2,y2);
    type = LINE_T;
} 
    
Line::Line(Point& pa, Point& pb)
{ 
    a = pa;
    b = pb;
    type = LINE_T;
} 

void Line::Rotate(float angle)
{  

    float xr, yr, theta;
    const double PIdiv180 = 0.0174532925194444; 
    theta = float(angle * PIdiv180); 
    xr = a.X() * cos(theta) - a.Y() * sin(theta);
    yr = a.X() * sin(theta) + a.Y() * cos(theta);
    a.MoveTo(xr, yr);

    xr = b.X() * cos(theta) - b.Y() * sin(theta);
    yr = b.X() * sin(theta) + b.Y() * cos(theta);
    b.MoveTo(xr, yr);
} 

float Line::Angle(const Point& pa, const Point& pb)
{
    const double PI = 3.1415926535; 
    float dx, dy;
    float ang;  
    dx = pb.X() - pa.X();
    dy = pa.Y() - pb.Y(); 
    if (dx > 0)
    { 
        if (dy >= 0)
        { 
            ang = atan(dy / dx); 
            ang = float((180.0 * ang) / PI);
        } 
        else 
        { 
            ang = atan(dy / dx); 
            ang = float((180.0 * ang) / PI) + 360.0f;
        }
    } 
    else if (dx < 0)
    { 
        ang = atan(dy / dx); 
        ang = float((180.0 * ang) / PI) + 180.0f;
    }
    else 
    {
        if (dy > 0)
            ang = 90.0f;
        else if (dy < 0)
            ang = 270.0f;
        else
            ang = 0.0f;
    }

    return ang;
}   

Rect::Rect()
{ 
    left = right  = 0.0f;
    top  = bottom = 0.0f;
    type = RECTANGLE_T;
} 

Rect::Rect(float x1, float y1, float x2, float y2)
{ 
    left   = x1;
    top    = y1;
    right  = x2; 
    bottom = y2;
    type   = RECTANGLE_T;
} 
Rect::Rect(Point& a, Point& b)
{ 
    left   = a.X();
    top    = a.Y();
    right  = b.X(); 
    bottom = b.Y();
    type   = RECTANGLE_T;
}   

Circle::Circle()
{ 
    radius   = 0;
    type     = CIRCLE_T;
} 

Circle::Circle(float r)
{ 
    radius   = r;
    type     = CIRCLE_T;
}   

Poly::Poly()
{
    vertexCount = 0; 
    vertexList  = nullptr; 
    bbox = new Circle(); 
    type = POLYGON_T; 
} 

Poly::Poly(Point * vList, uint vCount)
{ 
    vertexCount = vCount; 
    vertexList = new Point[vCount]; 
    for (uint i=0; i < vCount; ++i)        
        vertexList[i].MoveTo(vList[i].X(), vList[i].Y()); 
    BuildBBox(); 
    type = POLYGON_T;
} 

Poly::Poly(const Poly& p)
{ 
    vertexCount = p.vertexCount; 
    vertexList = new Point[vertexCount]; 
    for (uint i=0; i < vertexCount; ++i)
        vertexList[i].MoveTo(p.vertexList[i].X(), p.vertexList[i].Y()); 
    BuildBBox(); 
    type = POLYGON_T;
} 

const Poly& Poly::operator=(const Poly& p)
{ 
    if (vertexList)
        delete [] vertexList; 
    vertexCount = p.vertexCount; 
    vertexList = new Point[vertexCount]; 
    for (uint i=0; i < vertexCount; ++i)
        vertexList[i].MoveTo(p.vertexList[i].X(), p.vertexList[i].Y()); 
    BuildBBox(); 
    type = POLYGON_T;

    return *this;
} 

void Poly::Translate(float dx, float dy)
{
    x += dx; 
    y += dy;
    bbox->Translate(dx, dy);
} 

void Poly::MoveTo(float px, float py)
{
    x = px;
    y = py;
    bbox->MoveTo(px, py);
} 

float Poly::Scale() const
{
    return scale;
} 

void Poly::Scale(float factor)
{
    scale *= factor;
    bbox->Scale(factor);
} 

void Poly::ScaleTo(float value)
{
    scale = value;
    bbox->ScaleTo(value);
} 

void Poly::BuildBBox()
{
    float curRadius;
    float maxRadius = 0.0f; 
    for (uint i = 0; i < vertexCount; ++i)
    { 
        float pX = vertexList[i].X() * scale;
        float pY = vertexList[i].Y() * scale; 
        curRadius = Point::Distance(Point(pX,pY), Point(0,0));
        if (maxRadius < curRadius)
            maxRadius = curRadius;
    } 
    if (bbox) delete bbox;
    bbox = new Circle(maxRadius);
    bbox->MoveTo(x, y);
} 

Poly::~Poly()
{ 
    if (vertexList)
        delete [] vertexList;
}   

Mixed::Mixed()
{
    type = MIXED_T;
} 

Mixed::~Mixed()
{ 
    for (auto i : shapes)
        delete i;
} 

void Mixed::Insert(Geometry * s)
{
    shapes.push_back(s);
} 

void Mixed::Remove(Geometry * s)
{
    auto i = find(shapes.begin(), shapes.end(), s);
    shapes.erase(i);
} 

void Mixed::Translate(float dx, float dy)
{
    x += dx;
    y += dy;

    for (auto i : shapes)
        i->Translate(dx, dy);
} 

void Mixed::MoveTo(float px, float py)
{
    for (auto i : shapes)
    {
        float deltaX = i->X() - x;
        float deltaY = i->Y() - y;
        i->MoveTo(px + deltaX, py + deltaY);
    }

    x = px;
    y = py;
} 

float Mixed::Scale() const
{
    return scale;
} 

void Mixed::Scale(float factor)
{
    scale *= factor;

    for (auto i : shapes)
        i->Scale(factor);
} 

void Mixed::ScaleTo(float value)
{
    scale = value;
    for (auto i : shapes)
        i->ScaleTo(value);
} 
