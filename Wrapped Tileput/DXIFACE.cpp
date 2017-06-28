#include "DXIFACE.h"
using namespace DXISPACE;

DXIFACE::DXIFACE(int tiles_high, int tiles_wide) :
	m_hwnd(NULL),
	m_pDirect2dFactory(NULL),
	m_pRenderTarget(NULL)
{
	cellBuffer = new cell[tiles_wide, tiles_high];
	tiles_w = tiles_wide;
	tiles_h = tiles_high;
}


DXIFACE::~DXIFACE()
{
	SafeRelease(&m_pDirect2dFactory);
	SafeRelease(&m_pRenderTarget);
	delete [] cellBuffer;
}

HRESULT DXIFACE::Initialize(HINSTANCE hInstance)
{
	
}
