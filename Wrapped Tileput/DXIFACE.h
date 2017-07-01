#pragma once
#include "includes.h"
namespace DXISPACE {
	enum TILEVAL {};
	enum INTMODE {};
	enum COLOR {};
	
	struct cell
	{
		bool changed = false;
		TILEVAL value;
		INTMODE interp;
		COLOR   color;
	};

	class DXIFACE
	{
	public:
		DXIFACE(int* screensizeWH, int* tilesizeWH);
		~DXIFACE();

		HRESULT Initialize(HINSTANCE hInstance, LRESULT inputFunc(HWND, UINT, WPARAM, LPARAM), wchar_t* windowName);
		HRESULT Render();
		HRESULT SetCells(__in cell*, int[2] );
		void    GetCells(__out cell*);

		void	RunMessageLoop();
	private:
		HRESULT CreateDeviceIndResources();
		HRESULT CreateDeviceResources();
		void    DiscardDeviceResources();
		void	OnResize(UINT width, UINT height);

		static LRESULT CALLBACK WndProc(
			HWND hWnd,
			UINT message,
			WPARAM wParam,
			LPARAM lParam
		);

	private:
		HWND m_hWnd;
		ID2D1Factory* m_pDirect2dFactory;
		ID2D1RenderTarget* m_pRenderTarget;
		cell* cellBuffer;
		int screensize[2];
		int tilesize[2];
		std::mutex cellAccess;
		LRESULT(*unhandleFunc)(HWND, UINT, WPARAM, LPARAM);
	};
}