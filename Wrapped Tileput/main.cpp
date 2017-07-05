#include "includes.h"
#include "classes.h"

#define SCRN_SZ_W 11
#define SCRN_SZ_H 11
#define TILE_SZ_W 16
#define TILE_SZ_H 16

LRESULT inputHandle(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI LogicThread(LPVOID);

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
		info.tilesize.width = 32;
		info.tilesize.height = 32;
		info.tilesetFilename = L"HelloWorld.bmp";
		info.backgrdColor = D2D1::ColorF(D2D1::ColorF::Black);

		directX = new DXISPACE::DXIFACE(&info);
	}

	// Initialize modules
	CoInitialize(NULL);
	hr = directX->Initialize(hInstance, inputHandle, L"First Window");
	if (SUCCEEDED(hr))
		hr = directX->SetTileset(L"HELO.bmp", 16,16);

	// Create thread to push cell information
	CreateThread(
		NULL,
		0,
		LogicThread,
		directX,
		0,
		NULL
	);
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

#define HELO_H 0.0f, 16.0f
#define HELO_E 0.0f, (16.0f*2)
#define HELO_L 0.0f, (16.0f*3)
#define HELO_O 0.0f, (16.0f*4)
#define HELO_W 0.0f, (16.0f*5)
#define HELO_R 0.0f, (16.0f*6)
#define HELO_D 0.0f, (16.0f*7)

DWORD WINAPI LogicThread(LPVOID lp)
{
	DXISPACE::DXIFACE *dxiface = reinterpret_cast<DXISPACE::DXIFACE*> (lp);

	Sleep(2500);

	dxiface->SetCell(0, 5, HELO_H);
	dxiface->SetCell(1, 5, HELO_E);
	dxiface->SetCell(2, 5, HELO_L);
	dxiface->SetCell(3, 5, HELO_L);
	dxiface->SetCell(4, 5, HELO_O);
	dxiface->SetCell(5, 5, 0.0f, 0.0f);
	dxiface->SetCell(6, 5, HELO_W);
	dxiface->SetCell(7, 5, HELO_O);
	dxiface->SetCell(8, 5, HELO_R);
	dxiface->SetCell(9, 5, HELO_L);
	dxiface->SetCell(10, 5, HELO_D);

	Sleep(2500);

	dxiface->ResetCells();
	dxiface->SetTileset(L"HelloWorld.bmp", 32, 32);
	dxiface->SetCell(6, 6, 0.0f, 0.0f);

	return S_OK;
}