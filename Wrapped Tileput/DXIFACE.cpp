#include "DXIFACE.h"
using namespace DXISPACE;

DXIFACE::DXIFACE(dxifaceInfo* info) :
	m_hWnd(NULL),
	m_pDirect2dFactory(NULL),
	m_pRenderTarget(NULL)
{
	screensize[0] = info->screensize.width;
	screensize[1] = info->screensize.height;
	cells = new cell[info->screensize.width*info->screensize.height];

	cells[(0*1)+1].render = true;[]
	cells[(0*1)+1].TileLocation.y = 16.0f;

	tilesize[0] = info->tilesize.width;
	tilesize[1] = info->tilesize.height;
	tsFileName = info->tilesetFilename;
}


DXIFACE::~DXIFACE()
{
	SafeRelease(&m_pDirect2dFactory);
	SafeRelease(&m_pRenderTarget);
	delete [] cells;
}

HRESULT DXIFACE::Initialize(HINSTANCE hInstance, LRESULT inputFunc(HWND,UINT,WPARAM,LPARAM), wchar_t* windowName)
{
	HRESULT hr = S_OK;

	unhandleFunc = inputFunc;

	// Initialize device independent resources, such
	// as the Direct2d Factory
	hr = CreateDeviceIndResources();

	if (SUCCEEDED(hr))
	{
		// Register the window class
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = DXIFACE::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = hInstance;
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
		wcex.lpszClassName = L"WrappedDXIput";

		RegisterClassEx(&wcex);

		// Because the CreateWindow function takes its size in pixels,
		// obtain the system DPI and use it to scale the window size.
//		FLOAT dpiX, dpiY;

		// The factory returns the current system DPI. This is also 
		// the value it will use to create its own windows.
//		m_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);

		// Create the window.
		m_hWnd = CreateWindow(
			L"WrappedDXIput",
			windowName,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
//			static_cast<UINT>(ceil(640.f * dpiX / 96.f)),
//			static_cast<UINT>(ceil(480.f * dpiY / 96.f)),
			640, 480,
			NULL,
			NULL,
			hInstance,
			this
		);
		hr = m_hWnd ? S_OK : E_FAIL;
		if (SUCCEEDED(hr))
		{
			ShowWindow(m_hWnd, SW_SHOWNORMAL);
			UpdateWindow(m_hWnd);
		}
	}

	return hr;
}

void DXISPACE::DXIFACE::RunMessageLoop()
{
	MSG msg;
	bool done = false;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
			done = true;
		else 
			Render();
	}
}

HRESULT DXISPACE::DXIFACE::CreateDeviceIndResources()
{
	HRESULT hr = S_OK;

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

	return hr;
}

HRESULT DXISPACE::DXIFACE::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	if (!m_pRenderTarget)
	{
		RECT rc;

		GetClientRect(m_hWnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top);

		// Create the D2D render target
		hr = m_pDirect2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hWnd, size),
			&m_pRenderTarget);

		if(SUCCEEDED(hr))
			hr = fillTileset();
	}

	return hr;
}

void DXISPACE::DXIFACE::DiscardDeviceResources()
{
	SafeRelease(&m_pDirect2dFactory);
	SafeRelease(&m_pBTileSet);
	//SafeRelease(&m_pBScreenBuffer);
}

LRESULT CALLBACK DXISPACE::DXIFACE::WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	LRESULT result = 0;



	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		DXIFACE* pThis = (DXIFACE*)pcs->lpCreateParams;

		if (FAILED(::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis))))
		{
			wchar_t buf[40];
			swprintf_s(buf, L"Error code: %d", int(GetLastError()));
			MessageBox(hWnd, buf, L"Error", MB_OK);
		}

		result = 1;
	}
	else
	{
		DXIFACE* pThis = reinterpret_cast<DXIFACE*>(static_cast<LONG_PTR>(
			::GetWindowLongPtr(
				hWnd,
				GWLP_USERDATA
			)));
		bool wasHandled = false;

		if (pThis != 0)
		{
			switch (message)
			{
			case WM_DESTROY:
			{
				PostQuitMessage(0);
				result = 1;
				wasHandled = true;
				break;
			}
			
			case WM_SIZE:
				{
					UINT width = LOWORD(lParam);
					UINT height = HIWORD(lParam);
					pThis->OnResize(width, height);
				}
				result = 0;
				wasHandled = true;
				break;
			

			case WM_DISPLAYCHANGE:
				{
					InvalidateRect(hWnd, NULL, FALSE);
				}
				result = 0;
				wasHandled = true;
				break;

			case WM_PAINT:
				{
					HRESULT hr = pThis->Render();
					ValidateRect(hWnd, NULL);
					if (FAILED(hr))
						PostQuitMessage(-1);
				}
				result = 0;
				wasHandled = true;
				break;
			}

			if (!wasHandled)
				result = pThis->unhandleFunc(hWnd, message, wParam, lParam);
			return result;
		}

		if (!wasHandled)
		{
			result = DefWindowProc(hWnd, message, wParam, lParam);
		}
	}

	return result;
}

void DXISPACE::DXIFACE::OnResize(UINT width, UINT height)
{
	if (m_pRenderTarget)
	{
		// Note: This method can fail, but it's okay to ignore the
		// error here, because the error will be returned again
		// the next time EndDraw is called.
		m_pRenderTarget->Resize(D2D1::SizeU(width, height));
	}
}

HRESULT DXISPACE::DXIFACE::Render()
{
	HRESULT hr = S_OK;

	hr = CreateDeviceResources();

	if (SUCCEEDED(hr))
	{
		m_pRenderTarget->BeginDraw();
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pRenderTarget->Clear(m_bkgrdColor);

		// Retrieve the size of the render target
		D2D1_SIZE_F renderTargetSize = m_pRenderTarget->GetSize();

		// square tiles delimited by smallest side
		float actDim = 0.0F;
		if (renderTargetSize.width < renderTargetSize.height)
			actDim = renderTargetSize.width / screensize[0];
		else actDim = renderTargetSize.height / screensize[1];

		// tiles proportional to screensize
//		float actWidth =  floorf(renderTargetSize.width / screensize[0]);
//		float actHeight = floorf(renderTargetSize.height / screensize[1]);


		// iterate through cells and draw each
		/*for (int iw = 0; iw < screensize[0]; iw++)
		{
			for (int ih = 0; ih < screensize[1]; ih++)
			{
				if (cells[iw, ih].render)
				{
					// location of tile on tileset bitmap
					D2D1_RECT_F srcLoc = D2D1::RectF(cells[ih, iw].TileLocation.y, cells[ih, iw].TileLocation.x, cells[ih, iw].TileLocation.y + tilesize[0], cells[ih, iw].TileLocation.x + tilesize[1]);
					// translation of cell[w,h] to rect on screen
//					D2D1_RECT_F dest = D2D1::RectF(iw*actWidth, ih*actHeight, (iw+1)*actWidth, (ih+1)*actHeight);
					D2D1_RECT_F dest = D2D1::RectF(iw*actDim, ih*actDim, (iw + 1)*actDim, (ih + 1)*actDim);
					// draw the bitmap from its src in tilest to dest on screen
					m_pRenderTarget->DrawBitmap(
						m_pBTileSet,
						dest,
						1.0f,
						D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
						srcLoc
					);
				}
			}
		}*/
	}

	hr = m_pRenderTarget->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}

	return hr;
}

HRESULT DXISPACE::DXIFACE::SetCell(int loc_w, int loc_h, float tileLocX, float tileLocY, bool vis)
{
	cells[loc_w, loc_h].TileLocation.x = tileLocX;
	cells[loc_w, loc_h].TileLocation.y = tileLocY;
	cells[loc_w, loc_h].render = vis;
	
	return S_OK;
}

HRESULT DXISPACE::DXIFACE::fillTileset()
{
	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapFrameDecode *pSource = NULL;
	IWICStream *pStream = NULL;
	IWICFormatConverter *pConverter = NULL;
	IWICBitmapScaler *pScaler = NULL;
	IWICImagingFactory *pIWICFactory = NULL;


	HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		(LPVOID*)&pIWICFactory
	);

	hr = pIWICFactory->CreateDecoderFromFilename(
		tsFileName,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);

	if (SUCCEEDED(hr))
	{
		// Create the initial frame.
		hr = pDecoder->GetFrame(0, &pSource);
	}

	if (SUCCEEDED(hr))
	{

		// Convert the image format to 32bppPBGRA
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		hr = pIWICFactory->CreateFormatConverter(&pConverter);

	}

	if (SUCCEEDED(hr))
	{
		hr = pConverter->Initialize(
			pSource,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);
	}

	if (SUCCEEDED(hr))
	{

		// Create a Direct2D bitmap from the WIC bitmap.
		hr = m_pRenderTarget->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			&m_pBTileSet
		);
	}

	SafeRelease(&pDecoder);
	SafeRelease(&pSource);
	SafeRelease(&pStream);
	SafeRelease(&pConverter);
	SafeRelease(&pScaler);
	SafeRelease(&pIWICFactory);

	return hr;
}
