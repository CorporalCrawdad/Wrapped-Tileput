#pragma once
#include "includes.h"

namespace DXISPACE {
	enum INTMODE {};
	enum COLOR {};
	enum TileType {};
	
	struct cell
	{
		bool changed = false;
		TileType*		tileval;
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
	};

	class DXIFACE
	{
	public:
		DXIFACE(dxifaceInfo* info);
		~DXIFACE();

		HRESULT Initialize(HINSTANCE hInstance, LRESULT inputFunc(HWND, UINT, WPARAM, LPARAM), wchar_t* windowName);
		HRESULT Render();
		HRESULT SetCells(int loc_w, int loc_h, __in TileType* value);
		void    GetCells(int loc_w, int loc_h, __out TileType* retVal);

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
		ID2D1Factory* m_pDirect2dFactory;
		ID2D1HwndRenderTarget* m_pRenderTarget;
		cell* cellBuffer;
		int screensize[2];
		int tilesize[2];
		std::mutex cellAccess;
		LRESULT(*unhandleFunc)(HWND, UINT, WPARAM, LPARAM);
		ID2D1Bitmap* p_mBTileSet;

		// load bitmap from file WIC: https://msdn.microsoft.com/en-us/library/windows/desktop/dd756686(v=vs.85).aspx
		// draw text using DWrite: https://msdn.microsoft.com/en-us/library/windows/desktop/dd756692(v=vs.85).aspx
	};
}