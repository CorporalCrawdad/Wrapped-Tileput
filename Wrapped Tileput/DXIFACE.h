#pragma once
#include "includes.h"

namespace DXISPACE {
	enum INTMODE {};
	enum COLOR {};
	
	struct cell
	{
		bool render = false;
		struct	
		{
			float x = 0.0f;
			float y = 0.0f;
		}TileLocation;
		INTMODE interp;
		COLOR   color;
	};

	struct dxifaceInfo
	{
		struct
		{
			int height;
			int width;
		} screensize, tilesize;
		PCWSTR tilesetFilename;
		D2D1_COLOR_F backgrdColor;
	};

	class DXIFACE
	{
	public:
		DXIFACE(dxifaceInfo* info);
		~DXIFACE();

		HRESULT Initialize(HINSTANCE hInstance, LRESULT inputFunc(HWND, UINT, WPARAM, LPARAM), wchar_t* windowName);
		HRESULT Render();
		HRESULT SetCell(int loc_w, int loc_h, __in float tileLocX, float tileLocY, bool vis = true);
		void	ResetCells(int start_loc_w = 0, int start_loc_h = 0, int end_loc_w = 0, int end_loc_h = 0);
		HRESULT SetTileset(PCWSTR uri, int tile_w = 0, int tile_h = 0);
		void    GetCell(int loc_w, int loc_h, __out float* retVal);
		HRESULT ChangeBkgrdColor(D2D1_COLOR_F color);

		void	RunMessageLoop();
	private:
		HRESULT CreateDeviceIndResources();
		HRESULT CreateDeviceResources();
		void    DiscardDeviceResources();
		void	OnResize(UINT width, UINT height);
		HRESULT fillTileset();

		static LRESULT CALLBACK WndProc(
			HWND hWnd,
			UINT message,
			WPARAM wParam,
			LPARAM lParam
		);

		 
	private:
		HWND m_hWnd;
		PCWSTR tsFileName;
		D2D1_COLOR_F m_bkgrdColor;
		ID2D1Factory* m_pDirect2dFactory;
		ID2D1HwndRenderTarget* m_pRenderTarget;
		cell** cells;
		int screensize[2];
		int tilesize[2];
		bool tilesetChanged = false;
		std::mutex cellAccess;
		LRESULT(*unhandleFunc)(HWND, UINT, WPARAM, LPARAM);
		ID2D1Bitmap* m_pBTileSet;

		// load bitmap from file WIC: https://msdn.microsoft.com/en-us/library/windows/desktop/dd756686(v=vs.85).aspx
		// draw text using DWrite: https://msdn.microsoft.com/en-us/library/windows/desktop/dd756692(v=vs.85).aspx
	};
}