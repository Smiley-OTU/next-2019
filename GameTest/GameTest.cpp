//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
//------------------------------------------------------------------------
#include "app\app.h"
#include "SimpleTileMap.h"

#include "Math.h"
#include "Point.h"
#include "Line.h"
#include "ColouredLine.h"
#include "RayCaster.h"
#include "Player.h"

//16x16 grid.
static const int MAP_SIZE = 16;
static const int TUNNEL_LEN = 12;
static const float TUNNEL_FILL_PERCENT = 80;

//No "Game" class so everything will be global for the sake of communicating between GLUT render and update callbacks.
//(Use "static" to remove naming conflicts among translation units. Most likely no need so currently not worth extra keystrokes.
CSimpleTileMap g_map(MAP_SIZE);
CRayCaster g_rayCaster{ 4.0f };
CPlayer g_player;

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
    g_map.RandomMap(TUNNEL_FILL_PERCENT, TUNNEL_LEN);
	g_player.setFov(60.0f);
	//g_player.setPosition(408.0f, 408.0f);
	g_player.setPosition(390.0f, 431.0f);
	g_player.setDirection(251.0f);
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
    static const float MIN_FRAMERATE = (1000 / 20);
    if (deltaTime > MIN_FRAMERATE)
        deltaTime = MIN_FRAMERATE;
	   
    if (App::GetController().CheckButton(VK_PAD_A, true))
    {
        g_map.RandomMap(TUNNEL_FILL_PERCENT, TUNNEL_LEN);
    }
	g_rayCaster.Update();
	g_player.Update(deltaTime);
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine or Print) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{
	g_map.Render();
	g_rayCaster.Render(g_map, g_player);
	for (float i = 0.0f; i < APP_VIRTUAL_HEIGHT; i += g_map.getTileHeight())
		App::DrawLine(0.0f, i, APP_VIRTUAL_WIDTH, i);
	for (float j = 0.0f; j < APP_VIRTUAL_WIDTH; j += g_map.getTileWidth())
		App::DrawLine(j, 0.0f, j, APP_VIRTUAL_HEIGHT);
	/*static float halfWidth = (float)APP_VIRTUAL_WIDTH / 2.0f;
	static float halfHeight = (float)APP_VIRTUAL_HEIGHT / 2.0f;
	//P1, top left.
	glViewport(0.0f, halfHeight, halfWidth, halfHeight);
	g_rayCaster.Render();
	//P2, top right.
	glViewport(halfWidth, halfHeight, halfWidth, halfHeight);
	g_rayCaster.Render();
	//P3, bottom left.
	glViewport(0.0f, 0.0f, halfWidth, halfHeight);
	g_rayCaster.Render();
	//P4, bottom right.
	glViewport(halfWidth, 0.0f, halfWidth, halfHeight);
	g_rayCaster.Render();*/

	//Intersection algorithm doesn't take thickness into account. That would need an OOBB check!
	//CPoint poi{ Math::intersect(l1, l2) };
	//printf("Intersection at %f %f.\n", poi[0], poi[1]);
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{	//No leaks will be tolorated!!!
}