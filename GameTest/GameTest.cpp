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
#include "Math/Point.h"

static const int MAP_SIZE = 16;
static const int TUNNEL_LEN = 12;
static const float TUNNEL_FILL_PERCENT = 80;

//No "Game" class so everything will be global for the sake of communicating between GLUT render and update callbacks.
//(Use "static" to remove naming conflicts among translation units. Most likely no need so currently not worth extra keystrokes.
CSimpleTileMap g_map(MAP_SIZE);
CPoint point{ 4, 5 };

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
    g_map.RandomMap(TUNNEL_FILL_PERCENT, TUNNEL_LEN);
	point.print();
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
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine or Print) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{	 
    g_map.Render();
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
}