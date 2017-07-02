#include "DXIFACE.h"
using namespace DXISPACE;

DXIFACE::DXIFACE(int* screensizeWH, int* tilesizeWH) :
	m_hWnd(NULL),
	m_pDirect2dFactory(NULL),
	m_pRenderTarget(NULL)
{
	cellBuffer = new cell[screensizeWH[0], screensizeWH[1]];
	screensize[0] = screensizeWH[0];
	screensize[1] = screensizeWH[1];
}


DXIFACE::~DXIFACE()
{
	SafeRelease(&m_pDirect2dFactory);
	SafeRelease(&m_pRenderTarget);
	delete [] cellBuffer;
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

	while (GetMessage(&msg, NULL, NULL, NULL)>0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

HRESULT DXISPACE::DXIFACE::CreateDeviceIndResources()
{
	return S_OK;
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
				PostQuitMessage(0);
				result = 1;
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