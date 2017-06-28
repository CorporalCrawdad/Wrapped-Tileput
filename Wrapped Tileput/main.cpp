#include "includes.h"
#include "classes.h"

#define SCRN_W 20
#define SCRN_H 20

HRESULT inputHandle();

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE ,
	LPSTR     ,
	int       )
{
	// Variables
	DXISPACE::DXIFACE directX(SCRN_W, SCRN_H);

	// Initialize modules
	directX.Initialize(hInstance, inputHandle, L"First Window");

	// Run UI Loop

	directX.RunMessageLoop();
}

HRESULT inputHandle()
{
	HRESULT hr = S_OK;
	return hr;
}