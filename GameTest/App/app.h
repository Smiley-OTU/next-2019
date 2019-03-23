//---------------------------------------------------------------------------------
// App.h
//---------------------------------------------------------------------------------
#ifndef _APP_H
#define _APP_H
//---------------------------------------------------------------------------------
#include <windows.h> 
//---------------------------------------------------------------------------------
#include "../glut/include/GL/freeglut.h"
#include "AppSettings.h"
#include "SimpleController.h"
struct CPoint;
struct CLine;

//---------------------------------------------------------------------------------
// App namespace: These are the IO calls you can use for you game.
//---------------------------------------------------------------------------------
namespace App
{
	//*******************************************************************************************
	// Display Calls.
	//*******************************************************************************************
	//-------------------------------------------------------------------------------------------
	// void DrawLine( float sx, float sy, float ex, float ey, float r = 1.0f, float g = 1.0f, float b = 1.0f );
	//-------------------------------------------------------------------------------------------
	// Draw a 2D Line from sx,sy to ex, ey using color r = red, g = green, b = blue.
	//-------------------------------------------------------------------------------------------
	void DrawPoint(const CPoint& point, float border = 1.0f, float r = 1.0f, float g = 1.0f, float b = 1.0f);
	void DrawPoint(float x, float y, float border = 1.0f, float r = 1.0f, float g = 1.0f, float b = 1.0f);
	void DrawLine(const CPoint& start, const CPoint& end, float r = 1.0f, float g = 1.0f, float b = 1.0f);
	void DrawLine(const CLine& line, float r = 1.0f, float g = 1.0f, float b = 1.0f);
	void DrawLine(float sx, float sy, float ex, float ey, float r = 1.0f, float g = 1.0f, float b = 1.0f);
    void DrawQuad(float sx, float sy, float ex, float ey, float r = 1.0f, float g = 1.0f, float b = 1.0f);
	//-------------------------------------------------------------------------------------------
	// void Print(float x, float y, const char *text, float r = 1.0f, float g = 1.0f, float b = 1.0f, void *font = GLUT_BITMAP_HELVETICA_18);
	//-------------------------------------------------------------------------------------------
	// Print text to screen at x,y coordinates using color r = red, g = green, b=blue using the passed GLUT font. 
	// Color values are in the range 0.0f to 1.0f.
	// Available fonts...
	// GLUT_BITMAP_9_BY_15, GLUT_BITMAP_8_BY_13, GLUT_BITMAP_TIMES_ROMAN_10, GLUT_BITMAP_TIMES_ROMAN_24
	// GLUT_BITMAP_HELVETICA_10, GLUT_BITMAP_HELVETICA_12, GLUT_BITMAP_HELVETICA_18	
	//-------------------------------------------------------------------------------------------
	void Print(float x, float y, const char *text, float r = 1.0f, float g = 1.0f, float b = 1.0f, void *font = GLUT_BITMAP_HELVETICA_18);

	//*******************************************************************************************
	// Sound handling.	
	//*******************************************************************************************
	//-------------------------------------------------------------------------------------------
	// void PlaySound(const char *fileName, bool looping = false);
	//-------------------------------------------------------------------------------------------
	// Plays the passed .wav file. Set looping to true if you want the sound to loop.
	// If the sound is already playing it will restart.
	//-------------------------------------------------------------------------------------------
	void PlaySound(const char *fileName, bool looping = false);

	//-------------------------------------------------------------------------------------------
	// void StopSound(const char *fileName);
	//-------------------------------------------------------------------------------------------	
	// Stops the given .wav file from playing.
	//-------------------------------------------------------------------------------------------	
	void StopSound(const char *fileName);

	//-------------------------------------------------------------------------------------------
	// bool IsSoundPlaying(const char *filename);
	//-------------------------------------------------------------------------------------------	
	// Returns true if the given .wav file is currently playing.
	//-------------------------------------------------------------------------------------------	
	bool IsSoundPlaying(const char *filename);
	
	//*******************************************************************************************
	// Input handling.	
	//*******************************************************************************************
	//-------------------------------------------------------------------------------------------
	// bool IsKeyPressed(int key);
	//-------------------------------------------------------------------------------------------
	// Returns true if the given key is currently being pressed. Uses windows keys, see WinUser.h 
	// e.g.
	// IsKeyPressed(VK_F1); // Is the F1 key pressed.
	// IsKeyPressed('A'); // Is the 'A' key pressed.
	//-------------------------------------------------------------------------------------------
	bool IsKeyPressed(int key);

	//-------------------------------------------------------------------------------------------
	// void GetMousePos(float &x, float &y);
	//-------------------------------------------------------------------------------------------
	// Sets the value of the passed in float references to the current position of the mouse pointer.	
	//-------------------------------------------------------------------------------------------
	void GetMousePos(float &x, float &y);

	//-------------------------------------------------------------------------------------------
	// const CController &GetController(int pad = 0);
	//-------------------------------------------------------------------------------------------
	// Returns a reference to a game controller. 
	// You can use this object to query gamepad buttons and analog stick positions etc.
	// e.g.
	// GetController(0).CheckButton(XINPUT_GAMEPAD_A, false); Returns true if the A button of pad 0 was pressed and repeat if held.
	// GetController(0).CheckButton(XINPUT_GAMEPAD_A, true); Returns true if the A button of pad 0 was pressed but DO NOT repeat if held.	
	// GetController(0).GetLeftThumbStickX(); Returns a float between -1.0f to 1.0f representing the position of the left thumb sticks X Axis.
	// See SimpleController.h for more info.
	//-------------------------------------------------------------------------------------------
	const CController &GetController( int pad = 0 );
};

namespace Keys {
	enum {
		Mouse_Left = 0x01,
		Mouse_Right = 0x02,
		Backspace = 0x08,
		Tab = 0x09,
		Clear = 0x0C,
		Enter = 0x0D,
		Shift = 0x10,
		Control = 0x11,
		Alt = 0x12,
		Pause = 0x13,
		CapsLock = 0x14,
		Escape = 0x1B,
		Space = 0x20,
		PageUp = 0x21,
		PageDown = 0x22,
		End = 0x23,
		Home = 0x24,
		Left = 0x25,
		Up = 0x26,
		Right = 0x27,
		Down = 0x28,
		Select = 0x29,
		Print = 0x2A,
		Execute = 0x2B,
		PrintScreen = 0x2C,
		Insert = 0x2D,
		Delete = 0x2E,
		Help = 0x2F,
		Zero = 0x30,
		One = 0x31,
		Two = 0x32,
		Three = 0x33,
		Four = 0x34,
		Five = 0x35,
		Six = 0x36,
		Seven = 0x37,
		Eight = 0x38,
		Nine = 0x39,
		A = 0x41,
		B = 0x42,
		C = 0x43,
		D = 0x44,
		E = 0x45,
		F = 0x46,
		G = 0x47,
		H = 0x48,
		I = 0x49,
		J = 0x4A,
		K = 0x4B,
		L = 0x4C,
		M = 0x4D,
		N = 0x4E,
		O = 0x4F,
		P = 0x50,
		Q = 0x51,
		R = 0x52,
		S = 0x53,
		T = 0x54,
		U = 0x55,
		V = 0x56,
		W = 0x57,
		X = 0x58,
		Y = 0x59,
		Z = 0x5A,
		LeftWindowsKey = 0x5B,
		RightWindowsKey = 0x5C,
		ApplicationsKey = 0x5D,
		Sleep = 0x5F,
		NumPad0 = 0x60,
		NumPad1 = 0x61,
		NumPad2 = 0x62,
		NumPad3 = 0x63,
		NumPad4 = 0x64,
		NumPad5 = 0x65,
		NumPad6 = 0x66,
		NumPad7 = 0x67,
		NumPad8 = 0x68,
		NumPad9 = 0x69,
		Multiply = 0x6A,
		Add = 0x6B,
		Seperator = 0x6C,
		Subtract = 0x6D,
		Decimal = 0x6E,
		Divide = 0x6F,
		F1 = 0x70,
		F2 = 0x71,
		F3 = 0x72,
		F4 = 0x73,
		F5 = 0x74,
		F6 = 0x75,
		F7 = 0x76,
		F8 = 0x77,
		F9 = 0x78,
		F10 = 0x79,
		F11 = 0x7A,
		F12 = 0x7B,
		F13 = 0x7C,
		F14 = 0x7D,
		F15 = 0x7E,
		F16 = 0x7F,
		F17 = 0x80,
		F18 = 0x81,
		F19 = 0x82,
		F20 = 0x83,
		F21 = 0x84,
		F22 = 0x85,
		F23 = 0x86,
		F24 = 0x87,
		Numlock = 0x90,
		ScrollLock = 0x91,
		LeftShift = 0xA0,
		RightShift = 0xA1,
		LeftControl = 0xA2,
		RightContol = 0xA3,
		LeftMenu = 0xA4,
		RightMenu = 0xA5,
		BrowserBack = 0xA6,
		BrowserForward = 0xA7,
		BrowserRefresh = 0xA8,
		BrowserStop = 0xA9,
		BrowserSearch = 0xAA,
		BrowserFavorites = 0xAB,
		BrowserHome = 0xAC,
		VolumeMute = 0xAD,
		VolumeDown = 0xAE,
		VolumeUp = 0xAF,
		NextTrack = 0xB0,
		PreviousTrack = 0xB1,
		StopMedia = 0xB2,
		PlayPause = 0xB3,
		LaunchMail = 0xB4,
		SelectMedia = 0xB5,
		LaunchApp1 = 0xB6,
		LaunchApp2 = 0xB7,
		OEM1 = 0xBA,
		OEMPlus = 0xBB,
		OEMComma = 0xBC,
		OEMMinus = 0xBD,
		OEMPeriod = 0xBE,
		OEM2 = 0xBF,
		OEM3 = 0xC0,
		OEM4 = 0xDB,
		OEM5 = 0xDC,
		OEM6 = 0xDD,
		OEM7 = 0xDE,
		OEM8 = 0xDF,
		OEM102 = 0xE2,
		Process = 0xE5,
		Packet = 0xE7,
		Attn = 0xF6,
		CrSel = 0xF7,
		ExSel = 0xF8,
		EraseEOF = 0xF9,
		Play = 0xFA,
		Zoom = 0xFB,
		PA1 = 0xFD,
		OEMClear = 0xFE
	};
}
#endif //_APP_H