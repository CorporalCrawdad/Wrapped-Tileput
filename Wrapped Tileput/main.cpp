#include "includes.h"
#include "classes.h"

#define SCRN_W 20
#define SCRN_H 20

HRESULT inputHandle(HWND, UINT, WPARAM, LPARAM);

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE ,
	LPSTR     ,
	int       )
{
	// Variables
	DXISPACE::DXIFACE* directX;

	{
		int screensize[2] = { SCRN_W,SCRN_H };
		int tilesize[2] = { 32,32 };
		directX = new DXISPACE::DXIFACE(screensize, tilesize);
	}

	// Initialize modules
	directX->Initialize(hInstance, inputHandle, L"First Window");

	// Run UI Loop

	directX->RunMessageLoop();

	delete directX;
}

HRESULT inputHandle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = S_OK;
	hr = DefWindowProcW(hWnd, message, wParam, lParam);
	return hr;
}