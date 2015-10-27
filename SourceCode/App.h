// App.h: interface for the CApp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APP_H__8E0C149A_9092_4F0F_87F3_5A958925D533__INCLUDED_)
#define AFX_APP_H__8E0C149A_9092_4F0F_87F3_5A958925D533__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SingletonDestroyer.h"
#include "Sprite.h"
#include "Text.h"

//#define CAP_FRAME_RATE


#define GRID_SIZE_X			13
#define GRID_SIZE_Y			25

#define BRICK_SIZE_X		15
#define BRICK_SIZE_Y		15

#define GRID_BOTTOM			105
#define GRID_LEFT			180
#define GRID_TOP			(GRID_SIZE_Y*BRICK_SIZE_Y)+GRID_BOTTOM
#define GRID_RIGHT			(GRID_SIZE_X*BRICK_SIZE_X)+GRID_LEFT

#define START_POSITION_Y	GRID_TOP-(BRICK_SIZE_Y*4)
#define START_POSITION_X	255

#define NO_OF_PRIMITIVES	4
#define NO_OF_BLOCKS		7

#define NO_OF_ROTATIONS		4

#define TIME_DIVIDER_CONSTANT	150.0f

#define SCORE_INCREMENT_ONE		1
#define SCORE_INCREMENT_TWO		5
#define SCORE_INCREMENT_THREE	10
#define SCORE_INCREMENT_FOUR	20

#define PREVIEW_BLOCK_X			50
#define PREVIEW_BLOCK_Y			GRID_TOP-60

#define NO_OF_LEVELS			50
#define SCORE_INCREMENT			50
#define SPEED_INCREMENT			1.5f

#define FRAME_UPDATE_INTERVAL	20

#define INITIAL_LEVEL			1

#define FALLING_BLOCK_SPEED		100.0f


enum FINITE_MACHINE_STATUS {
	NEW_GAME, IN_GAME, LOST
};

struct BLOCK {
	int x, y;
};

// CApp. Singleton class
// Contains all the rubbish like windows initialisation
// and message handling

class CApp  
{
	friend class CSingletonDestroyer<CApp>;

///////////////////////////////////////////////////////////////
	// Singleton stuff

public:
	static CApp* GetInstance();
	static void Destroy();

protected:

private:
	static CApp* _instance;
	static CSingletonDestroyer<CApp> m_destroyer;

////////////////////////////////////////////////////////////
	// etc
public:
	void Render();
	void MessageLoop();
	void Resize(int, int);
	HGLRC GetHRC();
	HDC GetHDC();
	void RegisterWindow(string name);
	void CreateGLWindow(int w, int h, int b, BOOL fullscreen);
	HWND GetHwnd();
	int GetWidth();
	int GetHeight();

protected:
	int pixels_height;
	int pixels_width;

	float currentBackColour_r;
	float currentBackColour_b;
	float currentBackColour_g;
	double fps;
	int frameCount;
	int currentLevel;
	int score;
	int totalLines;
	__inline int ConvertY();
	__inline int ConvertX();
	BOOL CollisionDetect();
	void NewBrick();
	LONGLONG time_cur;
	LONGLONG time_last;
	LONGLONG time_difference;
	LONGLONG perf_frequency;
	LONGLONG time_next;
	BOOL perf_flag;
	int currentBlock;
	int nextBlock;
	int currentRotation;
	void Reset();
	int currentX;
	double currentY;
	void Init();
	ATOM m_aWndProc;
	BOOL fullscreen;
	string className;
	BOOL isActive;
	HGLRC hRC;
	HDC hDC;
	int bits;
	int height;
	int width;
	HINSTANCE hInstance;
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK StaticWndProc(HWND, UINT, WPARAM, LPARAM);
	HWND m_hWnd;

	CApp();
	virtual ~CApp(void);

	FINITE_MACHINE_STATUS state;

	CSprite *brick_primitives[ NO_OF_BLOCKS ];

	BLOCK blocks[ NO_OF_BLOCKS ][ NO_OF_ROTATIONS ][ NO_OF_PRIMITIVES ];
	int grid[ GRID_SIZE_X ][ GRID_SIZE_Y ];

	float currentSpeed;

	CText *text;

	int previous_frames[ FRAME_UPDATE_INTERVAL ];
	int refresh_rate;
	BOOL fps_flag;
	BOOL move_fast_flag;
};

#endif // !defined(AFX_APP_H__8E0C149A_9092_4F0F_87F3_5A958925D533__INCLUDED_)
