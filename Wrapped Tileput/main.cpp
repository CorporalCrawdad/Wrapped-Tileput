#include "includes.h"
#include "classes.h"

#define SCRN_SZ_W 1
#define SCRN_SZ_H 1
#define TILE_SZ_W 32
#define TILE_SZ_H 32

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
		DXISPACE::dxifaceInfo info;
		ZeroMemory(&info, sizeof(info));
		info.screensize.width = SCRN_SZ_W;
		info.screensize.height = SCRN_SZ_H;
		info.tilesize.width = TILE_SZ_W;
		info.tilesize.height = TILE_SZ_H;
		info.tilesetFilename = L"HELO.bmp";

		directX = new DXISPACE::DXIFACE(&info);
	}

	// Initialize modules
	CoInitialize(NULL);
	hr = directX->Initialize(hInstance, inputHandle, L"First Window");

	// Run UI Loop

	if(SUCCEEDED(hr)) directX->RunMessageLoop();

	delete directX;
	CoUninitialize();
}

LRESULT inputHandle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT hr = S_OK;
	hr = DefWindowProcW(hWnd, message, wParam, lParam);
	return hr;
}