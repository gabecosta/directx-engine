/**********************************************************************************        
**********************************************************************************/


#include "Background.h"
#include "Engine.h" 

Background::Background(string filename)
{ 
    image = new Image(filename);
    localImage = true; 
    spriteData.x = Engine::window->CenterX();
    spriteData.y = Engine::window->CenterY();
    spriteData.depth = Layer::BACK;
    spriteData.scale = 1.0f;    
    spriteData.rotation = 0.0f;
    spriteData.color = Color(1, 1, 1, 1);
    spriteData.texture = image->View();
} 

Background::Background(const Image* img)
{ 
    image = img;
    localImage = false; 
    spriteData.x = Engine::window->CenterX();
    spriteData.y = Engine::window->CenterY();
    spriteData.depth = Layer::BACK;
    spriteData.scale = 1.0f;
    spriteData.rotation = 0.0f;
    spriteData.color = Color(1, 1, 1, 1);
    spriteData.texture = image->View();
} 

Background::~Background()
{
    if (localImage)
        delete image;
} 

void Background::Draw(ViewPort & vp)
{ 
    spriteData.width = uint(vp.right - vp.left);
    spriteData.height = uint(vp.bottom - vp.top);
    spriteData.texCoord.x = vp.left / float(image->Width());
    spriteData.texCoord.y = vp.top / float(image->Height());
    spriteData.texSize.x = spriteData.width / float(image->Width());
    spriteData.texSize.y = spriteData.height / float(image->Height()); 
    Engine::renderer->Draw(spriteData);
} 

