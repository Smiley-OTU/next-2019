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

static const int MAP_SIZE = 16;
static const int TUNNEL_LEN = 12;
static const float TUNNEL_FILL_PERCENT = 80;

//No "Game" class so everything will be global for the sake of communicating between GLUT render and update callbacks.
//(Use "static" to remove naming conflicts among translation units. Most likely no need so currently not worth extra keystrokes.
CSimpleTileMap g_map(MAP_SIZE);
//CRayCaster g_rayCaster{ 0.0f, APP_VIRTUAL_WIDTH };
CRayCaster g_rayCaster{ APP_VIRTUAL_WIDTH * 0.25f, APP_VIRTUAL_WIDTH * 0.75f, 4.0f };

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
    g_map.RandomMap(TUNNEL_FILL_PERCENT, TUNNEL_LEN);
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
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine or Print) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{	 
    g_map.Render();
	g_rayCaster.Render();
	CLine l1{ 100.0f, 100.0f, 100.0f, 500.0f };
	CLine l2{ 50.0f, 300.0f, 700.0f, 300.0f };
	CLine l3{ APP_VIRTUAL_WIDTH * 0.2f, 100, APP_VIRTUAL_WIDTH * 0.2f, 500 };
	//Intersection algorithm doesn't take thickness into account. That would need an OOBB check!
	glLineWidth(100.0f);
	App::DrawLine(l1);
	App::DrawLine(l2);
	App::DrawLine(l3);
	CPoint poi{ Math::intersect(l1, l2) };
	//printf("Intersection at %f %f.\n", poi[0], poi[1]);
	g_rayCaster.rayOriginY = APP_VIRTUAL_HEIGHT / 2.0f;
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{	//No leaks will be tolorated!!!
#if _DEBUG
	FreeConsole();
#endif
}