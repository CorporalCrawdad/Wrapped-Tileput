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
		DXIFACE(int tiles_wide, int tiles_high);
		~DXIFACE();

		HRESULT Initialize(HINSTANCE hInstance, HRESULT inputFunc(HWND, UINT, WPARAM, LPARAM), wchar_t* windowName);
		HRESULT Render();
		HRESULT SetCells(__in cell*, int tiles_wide, int tiles_high);
		void    GetCells(__out cell*);

		void	RunMessageLoop();
	private:
		HRESULT CreateDeviceIndResources();
		HRESULT CreateDeviceResources();
		void    DiscardDeviceResources();

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
		int tiles_h, tiles_w;
		std::mutex cellAccess;
		LRESULT(*unhandleFunc)(HWND, UINT, WPARAM, LPARAM);
	};
}