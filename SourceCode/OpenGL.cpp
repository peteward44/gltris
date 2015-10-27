// OpenGL.cpp : Defines the entry point for the application.
//

#include "stdafx.h"


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	// this is the final exception catching field.
	// anything thrown this far back is a fatal exception.
	try {
		// initialise the CApp class.
		CApp *app;

		app = CApp::GetInstance();

		app->RegisterWindow("GLTris");
		app->CreateGLWindow(400, 500, 16, FALSE);

		app->MessageLoop();
	}
	catch (CException &e) {				// our exception class
		e.HandleError();
		return 1;
	}
	catch (...) {						// just in case
		MessageBox(NULL, "An unknown exception was caught", "Error", MB_OK);
		return 2;
	}

	return 0;
}



