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
		DXIFACE(int tiles_high, int tiles_wide);
		~DXIFACE();

		HRESULT Initialize(HINSTANCE);
		HRESULT Render();
		HRESULT SetCells(__in cell*, int tiles_wide, int tiles_high);
		void    GetCells(__out cell*);
	private:
		HRESULT CreateDeviceIndResources();
		HRESULT CreateDeviceResources();
		void    DiscardDeviceResources();

	private:
		HWND m_hwnd;
		ID2D1Factory* m_pDirect2dFactory;
		ID2D1RenderTarget* m_pRenderTarget;
		cell* cellBuffer;
		int tiles_h, tiles_w;
		std::mutex cellAccess;
	};

}