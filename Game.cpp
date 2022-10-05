/**********************************************************************************             
**********************************************************************************/

#include "Game.h"
#include "Engine.h"  

Canvas    Game::world    = { 0, 0, 0, 0 }; 
Window* & Game::window   = Engine::window; 
Game*   & Game::game     = Engine::game; 
float   & Game::gameTime = Engine::frameTime;  

Game::Game()
{ 
    Size(window->Width(), window->Height()); 
    viewport = { 0, 0, window->Width(), window->Height() };
} 

Game::~Game()
{
} 

void Game::OnPause()
{
    Sleep(10);
} 

void Game::Size(float width, float height)
{
    world.width = width;
    world.height = height;
    world.centerX = width / 2;
    world.centerY = height / 2;
} 