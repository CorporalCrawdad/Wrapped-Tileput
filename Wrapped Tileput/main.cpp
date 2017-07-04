#include "includes.h"
#include "classes.h"

#define SCRN_SZ_W 10
#define SCRN_SZ_H 10
#define TILE_SZ_W 16
#define TILE_SZ_H 16

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
		info.backgrdColor = D2D1::ColorF(D2D1::ColorF::Black);

		directX = new DXISPACE::DXIFACE(&info);
	}

	// Initialize modules
	CoInitialize(NULL);
	hr = directX->Initialize(hInstance, inputHandle, L"First Window");

	// Create thread to push cell information
	//directX->SetCell(0, 1, 0.0f, 16.0f);
	//directX->SetCell(1, 1, 0.0f, 32.0f);
	//directX->SetCell(2, 1, 0.0f, 48.0f);
	//directX->SetCell(3, 1, 0.0f, 48.0f);
	//directX->SetCell(4, 1, 0.0f, 64.0f);

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