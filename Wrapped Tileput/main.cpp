#include "includes.h"
#include "classes.h"
int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE ,
	LPSTR     ,
	int       )
{
	// Variables
	DXIFACE directX;

	// Initialize modules
	directX.Initialize();

	// Program Loop

	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}