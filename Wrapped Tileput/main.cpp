#include "includes.h"
#include "classes.h"

#define SCRN_W 20
#define SCRN_H 20

LRESULT inputHandle(HWND, UINT, WPARAM, LPARAM);

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE ,
	LPSTR     ,
	int       )
{
	// Variables
	DXISPACE::DXIFACE* directX;
	HRESULT hr;

	{
		int screensize[2] = { SCRN_W,SCRN_H };
		int tilesize[2] = { 32,32 };
		directX = new DXISPACE::DXIFACE(screensize, tilesize);
	}

	// Initialize modules
	hr = directX->Initialize(hInstance, inputHandle, L"First Window");

	// Run UI Loop

	if(SUCCEEDED(hr)) directX->RunMessageLoop();

	delete directX;
}

LRESULT inputHandle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT hr = S_OK;
	hr = DefWindowProcW(hWnd, message, wParam, lParam);
	return hr;
}