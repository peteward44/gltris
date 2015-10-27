// App.cpp: implementation of the CApp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <math.h>
#include <mmsystem.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CApp* CApp::_instance = NULL;
CSingletonDestroyer<CApp> CApp::m_destroyer;


CApp::CApp() {
	// set all our funky values
	hDC = NULL;
	hRC = NULL;
	m_hWnd = NULL;
	isActive = TRUE;
	text = NULL;
	frameCount = 0;
	fps = 0.0f;
	fps_flag = FALSE;
	refresh_rate = 60;
	time_difference = 0;

	memset(previous_frames, 0, sizeof(previous_frames));

	for (int i=0; i< NO_OF_BLOCKS ; i++)	brick_primitives[i] = NULL;

	// seed randomiser
	srand(timeGetTime());

	// set current state
	state = NEW_GAME;

	nextBlock = rand() % NO_OF_BLOCKS;

////////////////////////////////////////////////////////////////////
	// fill in all the brick offsets
	ZeroMemory(blocks, sizeof(blocks));

	// long block
	for (int j=0;j<NO_OF_ROTATIONS; j++) {
		for (int i=0;i<NO_OF_PRIMITIVES;i++)	blocks[0][j][i].y = i;
		j++;
		for (int i=0;i<NO_OF_PRIMITIVES; i++)	blocks[0][j][i].x = i;
	}

	// L shape 1
	for (int i=0;i<NO_OF_PRIMITIVES-1;i++)
		blocks[1][0][i].y = i;
	blocks[1][0][NO_OF_PRIMITIVES-1].y = NO_OF_PRIMITIVES-2;
	blocks[1][0][NO_OF_PRIMITIVES-1].x = 1;

	for (int i=0;i<NO_OF_PRIMITIVES-1;i++)
		blocks[1][1][i].x = i;
	blocks[1][1][NO_OF_PRIMITIVES-1].y = 1;

	for (int i=0;i<NO_OF_PRIMITIVES-1;i++) {
		blocks[1][2][i].y = i;
		blocks[1][2][i].x = 1;
	}

	for (int i=0;i<NO_OF_PRIMITIVES-1;i++) {
		blocks[1][3][i].x = i;
		blocks[1][3][i].y = 1;
	}
	blocks[1][3][NO_OF_PRIMITIVES-1].x = 2;


	// L shape 2
	for (int i=0;i<NO_OF_PRIMITIVES-1;i++) {
		blocks[2][0][i].y = i;
		blocks[2][0][i].x = 1;
	}
	blocks[2][0][NO_OF_PRIMITIVES-1].y = NO_OF_PRIMITIVES-2;
	blocks[2][0][NO_OF_PRIMITIVES-1].x = 0;

	for (int i=0;i<NO_OF_PRIMITIVES-1;i++) {
		blocks[2][1][i].x = i;
		blocks[2][1][i].y = 1;
	}

	for (int i=0;i<NO_OF_PRIMITIVES-1;i++)
		blocks[2][2][i].y = i;
	blocks[2][2][NO_OF_PRIMITIVES-1].x = 1;

	for (int i=0;i<NO_OF_PRIMITIVES-1;i++)
		blocks[2][3][i].x = i;
	blocks[2][3][NO_OF_PRIMITIVES-1].x = 2;
	blocks[2][3][NO_OF_PRIMITIVES-1].y = 1;


	// Z shape 1
	for (int i=0;i< NO_OF_ROTATIONS ; i++) {
		blocks[3][i][1].y = 1;
		blocks[3][i][2].x = 1;
		blocks[3][i][2].y = 1;
		blocks[3][i][3].x = 1;
		blocks[3][i][3].y = 2;
		i++;
		blocks[3][i][0].x = 1;
		blocks[3][i][1].x = 2;
		blocks[3][i][2].y = 1;
		blocks[3][i][2].x = 1;
		blocks[3][i][3].y = 1;
	}

	// Z shape 2
	for (int i=0;i< NO_OF_ROTATIONS ; i++) {
		blocks[4][i][0].x = 1;
		blocks[4][i][1].x = 1;
		blocks[4][i][1].y = 1;
		blocks[4][i][2].y = 1;
		blocks[4][i][3].y = 2;
		i++;
		blocks[4][i][1].x = 1;
		blocks[4][i][2].x = 1;
		blocks[4][i][2].y = 1;
		blocks[4][i][3].x = 2;
		blocks[4][i][3].y = 1;
	}

	// square block
	for (int i=0;i< NO_OF_ROTATIONS; i++) {
		blocks[5][i][1].x = 1;
		blocks[5][i][2].y = 1;
		blocks[5][i][3].x = 1;
		blocks[5][i][3].y = 1;
	}

	// the T block
	blocks[6][0][0].x = 1;
	blocks[6][0][1].y = 1;
	blocks[6][0][2].x = 1;
	blocks[6][0][2].y = 1;
	blocks[6][0][3].x = 2;
	blocks[6][0][3].y = 1;

	blocks[6][1][1].y = 1;
	blocks[6][1][2].y = 2;
	blocks[6][1][3].x = 1;
	blocks[6][1][3].y = 1;

	blocks[6][2][1].x = 1;
	blocks[6][2][2].x = 2;
	blocks[6][2][3].x = 1;
	blocks[6][2][3].y = 1;

	blocks[6][3][0].x = 1;
	blocks[6][3][1].x = 1;
	blocks[6][3][1].y = 1;
	blocks[6][3][2].x = 1;
	blocks[6][3][2].y = 2;
	blocks[6][3][3].y = 1;

////////////////////////////////////////////////////////////////////

	Reset();
}

CApp::~CApp(void)
{
	if (text) delete text;

	for (int i=0; i< NO_OF_BLOCKS ; i++)
		if (brick_primitives[i]) delete brick_primitives[i];

	if (fullscreen)
	{
		ChangeDisplaySettings(NULL,0);
		ShowCursor(TRUE);
	}

	if (hRC)
	{
		wglMakeCurrent(NULL,NULL);
		if (hRC) wglDeleteContext(hRC);
	}

	if (hDC)	ReleaseDC(m_hWnd,hDC);

	if (m_hWnd) {
		RemoveProp(m_hWnd,"WPROC");
		GlobalDeleteAtom(m_aWndProc);
		DestroyWindow(m_hWnd);
		UnregisterClass(className.c_str(),hInstance);
	}
}


CApp* CApp::GetInstance () {
	if (!_instance) {
		_instance = new CApp;
		m_destroyer.SetSingleton(_instance);
	}
	return _instance;
}

void CApp::Destroy() {
	if (_instance) {
		delete _instance;
		_instance = NULL;
		m_destroyer.SetSingleton(NULL);
	}
}

////////////////////////////////////////////////////////////////////////////

void CApp::RegisterWindow(string name)
{
	WNDCLASS wc;
	className = name;
	hInstance = GetModuleHandle(NULL);

	// first register the window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= (WNDPROC) CApp::StaticWndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= NULL;
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= className.c_str();

	if (!RegisterClass(&wc))
		throw(CFatalException("RegisterClass() failed"));
}

void CApp::CreateGLWindow(int w, int h, int b, BOOL fs)
{
	width = w; height = h; bits = b;
	fullscreen = fs;

	unsigned int		PixelFormat;
	DWORD				dwExStyle;
	DWORD				dwStyle;
	RECT				WindowRect;
	int					posX, posY;

	WindowRect.left=(long)0;
	WindowRect.right=(long)width;
	WindowRect.top=(long)0;
	WindowRect.bottom=(long)height;

	pixels_width = GetSystemMetrics(SM_CXSCREEN);
	pixels_height = GetSystemMetrics(SM_CYSCREEN);

	if (fullscreen)
	{
		// change the resolution if its fullscreen
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize =		sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth	= width;
		dmScreenSettings.dmPelsHeight	= height;
		dmScreenSettings.dmBitsPerPel	= bits;
		dmScreenSettings.dmFields		= DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// if it fails, backout to windowed mode.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
			fullscreen=FALSE;
	}

	// set appropriate styles
	if (fullscreen)
	{
		dwExStyle=WS_EX_APPWINDOW;
		dwStyle=WS_POPUP;
		ShowCursor(FALSE);
		posX = 0; posY = 0;
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle=WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

		// put the window in the centre of the screen.
		posX = (pixels_width-width) / 2;
		posY = (pixels_height-height) / 2;
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);

	// Create The Window
	if (!(m_hWnd=CreateWindowEx(	dwExStyle,
								className.c_str(),
								className.c_str(),
								dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
								posX, posY,
								WindowRect.right-WindowRect.left,
								WindowRect.bottom-WindowRect.top,
								NULL,
								NULL,
								hInstance,
								NULL)))
	{
		throw(CFatalException("CreateWindowEx() failed"));
	}

	SetProp(m_hWnd,"WPROC",this);

	// pfd Tells Windows How We Want Things To Be
	static	PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		bits,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};
	
	// grab display context
	if (!(hDC=GetDC(m_hWnd)))
		throw(CFatalException("GetDC() failed"));

	// Did Windows Find A Matching Pixel Format?
	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))
		throw(CFatalException("ChoosePixelFormat() failed"));

	// Are We Able To Set The Pixel Format?
	if(!SetPixelFormat(hDC,PixelFormat,&pfd))
		throw(CFatalException("SetPixelFormat() failed"));

	// Are We Able To Get A Rendering Context?
	if (!(hRC=wglCreateContext(hDC)))
		throw(CFatalException("wglCreateContext() failed"));

	// Try To Activate The Rendering Context
	if(!wglMakeCurrent(hDC,hRC))
		throw(CFatalException("wglMakeCurrent() failed"));

	ShowWindow(m_hWnd,SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	Resize(width, height);

	// initialise GL
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	Init();
}

// add extra initialisation here

void CApp::Init()
{
//	glEnable(GL_TEXTURE_2D);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
//	glDisable(GL_TEXTURE_2D);

	// load brick textures
	brick_primitives[0] = new CSprite("images\\brick_red.bmp", P_BMP, BRICK_SIZE_X, BRICK_SIZE_Y);
	brick_primitives[1] = new CSprite("images\\brick_blue.bmp", P_BMP, BRICK_SIZE_X, BRICK_SIZE_Y);
	brick_primitives[2] = new CSprite("images\\brick_green.bmp", P_BMP, BRICK_SIZE_X, BRICK_SIZE_Y);
	brick_primitives[3] = new CSprite("images\\brick_yellow.bmp", P_BMP, BRICK_SIZE_X, BRICK_SIZE_Y);
	brick_primitives[4] = new CSprite("images\\brick_torq.bmp", P_BMP, BRICK_SIZE_X, BRICK_SIZE_Y);
	brick_primitives[5] = new CSprite("images\\brick_purple.bmp", P_BMP, BRICK_SIZE_X, BRICK_SIZE_Y);
	brick_primitives[6] = new CSprite("images\\brick_grey.bmp", P_BMP, BRICK_SIZE_X, BRICK_SIZE_Y);

	text = new CText("Arial");
}


LRESULT CALLBACK CApp::StaticWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CApp *pThis = (CApp *)GetProp(hWnd,"WPROC");
	if(pThis) return pThis->WndProc(hWnd,uMsg,wParam,lParam);
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

LRESULT CALLBACK CApp::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int i;
	int offset = 0;
	BOOL flag = FALSE;

	switch (uMsg)
	{
		// change activity flag if we become inactive (ie minimised window)
		case WM_ACTIVATE:
		{
			if (!HIWORD(wParam))	isActive=TRUE;
			else					isActive=FALSE;
		}

		// disable screensaver and monitor power saving kicking in
		case WM_SYSCOMMAND:
			switch (wParam)
			{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
				return 0;
			}
			break;

		case WM_KEYDOWN:
			switch (wParam)
			{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			case VK_UP:

				if (currentRotation == NO_OF_ROTATIONS-1)
					currentRotation = 0;
				else
					currentRotation++;

				// this stops the bricks rotating off the screen
				for (i=0; i< NO_OF_PRIMITIVES; i++) {
					if (blocks[ currentBlock ][ currentRotation ][ i ].x + ConvertX() >= GRID_SIZE_X)
						offset++;
				}
				currentX -= BRICK_SIZE_X*offset;

				break;
			case VK_DOWN:
				currentSpeed = FALLING_BLOCK_SPEED;
				break;
			case VK_LEFT:
				// check boundaries against wall
				if (currentX <= GRID_LEFT) 
					flag = TRUE;
				
				if (!flag) {
					// then see if theres a brick in the way
					for (i=0; i< NO_OF_PRIMITIVES; i++) {
						if (grid[ blocks[currentBlock][currentRotation][i].x + ConvertX() -1 ][ blocks[currentBlock][currentRotation][i].y + ConvertY() ] > -1)
							flag = TRUE;
					}
				}

				if (!flag)
					currentX -= BRICK_SIZE_X;

				break;
			case VK_RIGHT:
				// check boundaries against wall
				for (i=0; i< NO_OF_PRIMITIVES; i++) {
					if (((blocks[ currentBlock ][ currentRotation ][ i ].x+1) * BRICK_SIZE_X) + currentX >= GRID_RIGHT)
						flag = TRUE;
				}

				if (!flag) {
					// then see if theres a brick in the way
					for (i=0; i< NO_OF_PRIMITIVES; i++) {
						if (grid[ blocks[currentBlock][currentRotation][i].x + ConvertX() +1 ][ blocks[currentBlock][currentRotation][i].y + ConvertY() ] > -1)
							flag = TRUE;
					}
				}

				if (!flag)
					currentX += BRICK_SIZE_X;
				break;
			case 'F':
				if(!fps_flag)
					fps_flag = TRUE;
				else
					fps_flag = FALSE;
				break;
			}
			break;
		case WM_QUIT:
		case WM_CLOSE:
			PostQuitMessage(0);
			break;

		case WM_SIZE:
			Resize(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			break;
	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int CApp::GetHeight()
{
	return height;
}

int CApp::GetWidth()
{
	return width;
}

HWND CApp::GetHwnd()
{
	return m_hWnd;
}

HDC CApp::GetHDC()
{
	return hDC;
}

HGLRC CApp::GetHRC()
{
	return hRC;
}

void CApp::Resize(int w, int h)
{
	if (w == 0 && h == 0) {
		w = width;
		h = height;
	}

	if (h == 0)
		h = 1;

	glViewport(0,0,w,h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0f, (GLdouble) w, 0.0f, (GLdouble) h);

	// Calculate The Aspect Ratio Of The Window
//	gluPerspective(45.0f,(GLfloat)w/(GLfloat)h,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);								// Select The Modelview Matrix
	glLoadIdentity();
}

void CApp::MessageLoop()
{
	BOOL done = FALSE;
	MSG msg;
	DWORD time_count = (1000/refresh_rate);

	// check for the existance of a performance counter
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&perf_frequency)) {
		perf_flag = TRUE;
	} else {
		// use timeGetTime instead
		perf_flag = FALSE;
	}

	time_next = 0;
	time_cur = 1;

	while(!done)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if (msg.message==WM_QUIT)	done=TRUE;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// Draw The Scene
			if (isActive)		// are we active?
			{
				// frame timing code. calculate time between
				// each frame, and move blocks as appropriate
				// both leave the time taken for the frame to happen in milliseconds
				if (perf_flag) {
					QueryPerformanceCounter((LARGE_INTEGER*) &time_cur);
					time_cur = (time_cur*1000)/perf_frequency;
				} else {
					time_cur = timeGetTime();
				}

#ifdef CAP_FRAME_RATE
				if (time_cur > time_next) {
#endif

					time_difference = time_cur-time_last;

					time_last = time_cur;

					time_next = time_cur + time_count;

					Render();
					SwapBuffers(hDC);

#ifdef CAP_FRAME_RATE
				}
#endif

			}
		}
	}
}

void CApp::Render()
{
////////////////////////////////////////////////////////////////////////////////////////
// initialisation
	int i,j, lines_count = 0;

	// colour of background - make it change
	currentBackColour_r = 0;
	currentBackColour_g = 0;
	currentBackColour_b = 0;


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer;
	glLoadIdentity();

	// draw background rectangle
	glBegin(GL_QUADS);
		glColor3f(currentBackColour_r, currentBackColour_g, currentBackColour_b);
		glVertex2i( GRID_LEFT, GRID_BOTTOM );
		glColor3f(currentBackColour_r+0.2f, currentBackColour_g+0.2f, currentBackColour_b+0.2f);
		glVertex2i( GRID_RIGHT, GRID_BOTTOM );
		glColor3f(currentBackColour_r+0.4f, currentBackColour_g+0.4f, currentBackColour_b+0.4f);
		glVertex2i( GRID_RIGHT, GRID_TOP );
		glColor3f(currentBackColour_r+0.2f, currentBackColour_g+0.2f, currentBackColour_b+0.2f);
		glVertex2i( GRID_LEFT, GRID_TOP );
	glEnd();

	// blit the already landed blocks
	for (int a=0; a< NO_OF_BLOCKS; a++) {

		brick_primitives[ a ]->Begin();

		for (i=0; i< GRID_SIZE_X; i++) {
			for (j=0; j< GRID_SIZE_Y; j++) {
				if (grid[i][j] == a) {
					brick_primitives[ a ]->Blit((i*BRICK_SIZE_X)+GRID_LEFT,
						(j*BRICK_SIZE_Y)+GRID_BOTTOM);
				}
			}
		}

		brick_primitives[ a ]->End();

	}


////////////////////////////////////////////////////////////////////////////////////////
// new game

	if (state == NEW_GAME) {

		if (CollisionDetect()) {

			int x = ConvertX();
			int y = ConvertY();

			// fill in the grid
			for (i=0; i<NO_OF_PRIMITIVES; i++) {
				// if we are landing on a block thats already been filled in that means
				// we've lost
				int tempx, tempy;
				tempx = x + blocks[currentBlock][currentRotation][i].x;
				tempy = y + blocks[currentBlock][currentRotation][i].y;

				if (grid[ tempx ][ tempy ] > 0) {
					// lost
					state = LOST;
				} else
					grid[ tempx ][ tempy ] = currentBlock;
			}

			BOOL line_flag = FALSE;
			
			// see if a line of bricks have been made
			for (i=0; i< GRID_SIZE_Y; i++) {
				for (j=0; j< GRID_SIZE_X; j++) {
					if (grid[j][i] == -1) {
						line_flag = TRUE;
						break;
					}
				}
				
				if (!line_flag) {
					for (int a=i; a<GRID_SIZE_Y-1; a++) {
						for (int b=0; b<GRID_SIZE_X; b++) {
							grid[b][a] = grid[b][a+1];
						}
					}
					totalLines++;
					lines_count++;
					i--;
				} else {
					line_flag = FALSE;
				}
			}
			
			// increment score as appropriate
			switch (lines_count) {
			case 1:
				score += SCORE_INCREMENT_ONE;
				break;
			case 2:
				score += SCORE_INCREMENT_TWO;
				break;
			case 3:
				score += SCORE_INCREMENT_THREE;
				break;
			case 4:
				score += SCORE_INCREMENT_FOUR;
				break;
			}

			// see if we have to change the level
			for (i=1; i<=NO_OF_LEVELS; i++) {
				if (i * SCORE_INCREMENT > score) {
					currentLevel = i;
					if (currentLevel != 1)
						currentSpeed = SPEED_INCREMENT*i;
					else
						currentSpeed = 1;

					break;
				}
			}

			NewBrick();
		}

///////////////////////////////////////////////////////////////////////////

		for (i=0; i<NO_OF_PRIMITIVES; i++) {
			// this blits the currently falling block
			brick_primitives[ currentBlock ]->Begin();
			brick_primitives[ currentBlock ]->Blit(currentX + (blocks[ currentBlock ][ currentRotation ][ i ].x * BRICK_SIZE_X),
													currentY + (blocks[ currentBlock ][ currentRotation ][ i ].y * BRICK_SIZE_Y));
			brick_primitives[ currentBlock ]->End();

			// blit the preview for the next block
			brick_primitives[ nextBlock ]->Begin();
			brick_primitives[ nextBlock ]->Blit(PREVIEW_BLOCK_X + (blocks[ nextBlock ][ 0 ][ i ].x * BRICK_SIZE_X),
													PREVIEW_BLOCK_Y + (blocks[ nextBlock ][ 0 ][ i ].y * BRICK_SIZE_Y));
			brick_primitives[ nextBlock ]->End();
		}

		glColor3f(1.0f, 1.0f, 1.0f);
		text->Print(10, GRID_TOP-120, "Lines: %d", totalLines);
		text->Print(10, GRID_TOP-160, "Score: %d", score);
		text->Print(10, GRID_TOP-200, "Level: %d", currentLevel);


		// frames per second - only update every FRAME_UPDATE_INTERVAL frames
		if (frameCount == FRAME_UPDATE_INTERVAL) {
			double sum=0.0f;
			
			// find average of previous frame counts
			for (i=0; i< FRAME_UPDATE_INTERVAL; i++) {
				sum += previous_frames[i];
			}

			sum /= 1000;
			
			fps = 1.0f/(sum / FRAME_UPDATE_INTERVAL);
			frameCount = -1;
		} else {
			previous_frames[ frameCount ] = time_difference;
		}

		if (fps_flag) {	
			text->Print(width-200, 10, "FPS: %3.2f", fps);
		}
///////////////////////////////////////////////////////////////////////////

		// move block according to time difference between frames
		// time difference is measured in milliseconds, time the last frame to execute
		if ((time_difference/TIME_DIVIDER_CONSTANT) * currentSpeed >= BRICK_SIZE_Y) {
			currentY -= BRICK_SIZE_Y-0.1f;
		} else {
			currentY -= (time_difference/TIME_DIVIDER_CONSTANT) * currentSpeed;
		}
	}

	else if (state == LOST) {
		glColor3f(1.0f, 1.0f, 1.0f);
		text->Print(50, 300, "You suck, loser!");
	}

	frameCount++;
}

void CApp::Reset()
{
	// zero out the grid which sez whether theres a brick there or not
	memset(grid, -1, sizeof(grid));

	currentSpeed = 1;
	score = 0;
	totalLines = 0;
	currentLevel = INITIAL_LEVEL;

	NewBrick();
}

void CApp::NewBrick()
{
	// set current coordinates
	currentY = START_POSITION_Y;
	currentX = START_POSITION_X;

	// choose block shape
	currentBlock = nextBlock;

	nextBlock = rand() % NO_OF_BLOCKS;

	currentRotation = 0;

	// reset speed to level
	if (currentLevel != 1)
		currentSpeed = currentLevel * SPEED_INCREMENT;
	else
		currentSpeed = 1;
}

BOOL CApp::CollisionDetect()
{
	int x,y;

	x = ConvertX();
	y = ConvertY();

	for (int i=0;i<NO_OF_PRIMITIVES;i++) {
		// see if brick hit the bottom
		if (y + blocks[ currentBlock ][ currentRotation ][i].y == 0)
			return TRUE;

		// see if it hits another already landed brick
		if (grid[ x + blocks[currentBlock][currentRotation][i].x ][ y + blocks[currentBlock][currentRotation][i].y -1 ] > -1)
			return TRUE;
	}

	return FALSE;
}

int CApp::ConvertX()
{
	return floorf((currentX - GRID_LEFT)/BRICK_SIZE_X);
}

int CApp::ConvertY()
{
	return floorf((currentY - GRID_BOTTOM+((BRICK_SIZE_Y)/2))/BRICK_SIZE_Y);

}
