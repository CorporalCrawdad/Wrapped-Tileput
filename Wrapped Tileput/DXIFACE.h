#pragma once
#include "includes.h"
namespace DXISPACE {
	enum INTMODE {};
	enum COLOR {};
	
	template <typename TileType> struct cell
	{
		bool changed = false;
		TileType*		tileval;
		INTMODE interp;
		COLOR   color;
	};

	template struct cell<ID2D1Bitmap>;
	template struct cell<wchar_t>;

	class DXIFACE
	{
	public:
		DXIFACE(int* screensizeWH, int* tilesizeWH);
		~DXIFACE();

		HRESULT Initialize(HINSTANCE hInstance, LRESULT inputFunc(HWND, UINT, WPARAM, LPARAM), wchar_t* windowName);
		HRESULT Render();
		template <typename TileType>
		HRESULT SetCells(int[2] loc_wh, __in TileType* value);
		template <typename TileType>
		void    GetCells(int[2] loc_wh, __out TileType* retVal);

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
		ID2D1HwndRenderTarget* m_pRenderTarget;
		cell* cellBuffer;
		int screensize[2];
		int tilesize[2];
		std::mutex cellAccess;
		LRESULT(*unhandleFunc)(HWND, UINT, WPARAM, LPARAM);
	};
}