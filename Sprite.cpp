/**********************************************************************************        
**********************************************************************************/

#include "Sprite.h"
#include "Engine.h"      

const float Layer::FRONT  = 0.02f;
const float Layer::UPPER  = 0.25f;
const float Layer::MIDDLE = 0.50f;
const float Layer::LOWER  = 0.75f;
const float Layer::BACK   = 0.99f; 

Sprite::Sprite(string filename)
{ 
    image = new Image(filename);
    localImage = true; 
    sprite.texture = image->View();
} 

Sprite::Sprite(const Image * img)
{ 
    image = img;
    localImage = false; 
    sprite.texture = image->View();
} 

Sprite::~Sprite()
{
    if (localImage)
        delete image;
} 

void Sprite::Draw(float x, float y, float z, float scale, float rotation, Color color)
{ 
    x -= Engine::game->viewport.left;
    y -= Engine::game->viewport.top; 
    sprite.x = x;
    sprite.y = y;
    sprite.scale = scale;
    sprite.depth = z;
    sprite.rotation = rotation * XM_PI / 180.0f;
    sprite.width = image->Width();
    sprite.height = image->Height();
    sprite.texCoord.x = 0;
    sprite.texCoord.y = 0;
    sprite.texSize.x = 1;
    sprite.texSize.y = 1;
    sprite.color = color; 
    Engine::renderer->Draw(sprite);
} 

void Sprite::Draw(float x, float y, float z, float scale, float rotation, const Color & color, const RECT & region)
{ 
    x -= Engine::game->viewport.left;
    y -= Engine::game->viewport.top;

    sprite.x = x;
    sprite.y = y;
    sprite.scale = scale;
    sprite.depth = z;
    sprite.rotation = rotation * XM_PI / 180.0f;
    sprite.width = region.right - region.left;
    sprite.height = region.bottom - region.top;
    sprite.texCoord.x = float(region.left) / image->Width();
    sprite.texCoord.y = float(region.top) / image->Height();
    sprite.texSize.x = float(sprite.width) / image->Width();
    sprite.texSize.y = float(sprite.height) / image->Height();
    sprite.color = color; 
    Engine::renderer->Draw(sprite);
} 