//Libraries
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

//Includes
#include <Windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>

////////////////////////////////////
// Global variables

//Wnd
LPCTSTR WndClassName = "firstwindow";
HWND hwnd = NULL;
const int Width = 800;
const int Height = 600;

//D3D10
IDXGISwapChain* SwapChain;
ID3D11Device* d3d11Device;
ID3D11DeviceContext* d3d11DevCon;
ID3D11RenderTargetView* renderTargetView;

//temp vars
float red = 0.0f;
float green = 0.0f;
float blue = 0.0f;
int colormodr = 1;
int colormodg = 1;
int colormodb = 1;
////////////////////////////////////

////////////////////////////////////
//Function prototypes

//Wnd-Framework
bool InitializeWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed);
int messageloop();
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//D3D-game loop
bool InitializeDirect3d11App(HINSTANCE hInstance);
void ReleaseObjects();
bool InitScene();
void UpdateScene();
void DrawScene();
////////////////////////////////////

// Entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//Initialize Wnd and bind hwnd
	if (!InitializeWindow(hInstance, nShowCmd, Width, Height, true))
	{
		MessageBox(0,  "Window Initialization - Failed",  "Error", MB_OK);
		return 0;
	}
	//Initialize DirectX objects
	if (!InitializeDirect3d11App(hInstance))
	{
		MessageBox(0, "Direct3D Initialization - Failed", "Error", MB_OK);
		return 0;
	}
	//Initialize scene
	if (!InitScene())
	{
		MessageBox(0, "Scene Initialization - Failed", "Error", MB_OK);
		return 0;
	}

	messageloop();

	ReleaseObjects();

	return 0;
}
//Create window
bool InitializeWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed)
{
	WNDCLASSEX wc; //Wnd class
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW; //Redraw method
	wc.lpfnWndProc = WndProc; //Wnd stuff method
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hInstance = hInstance; //hInstance to bind to
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); //Title bar icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); //Cursor
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2); //Background brush to use
	wc.lpszMenuName = NULL; //Wnd menu
	wc.lpszClassName = WndClassName; //Class name of window
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION); //Taskbar icon
	//Check for registration errors
	if (!RegisterClassEx(&wc)) 
	{
		MessageBox(NULL,  "Error registering class",  "Error", MB_OK | MB_ICONERROR);
		return 1;
	}
	//Create wnd handle
	hwnd = CreateWindowEx(NULL, WndClassName,  "Window Title", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hInstance, NULL);
	//verify hwnd was bound to an instance of wnd
	if (!hwnd)
	{
		MessageBox(NULL,  "Error creating window",  "Error", MB_OK | MB_ICONERROR);
		return 1;
	}
	ShowWindow(hwnd, ShowWnd); //Show the window
	UpdateWindow(hwnd); //Run the update loop once
	return true;
}
//Infinite message loop
int messageloop()
{
	MSG msg; //Create message memory structure
	ZeroMemory(&msg, sizeof(MSG)); //Clear it
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) //Check to see if a message is waiting
		{
			if (msg.message == WM_QUIT) //Check to see if something ordered an application shutdown
			{
				break;
			}
			//If not quit, translate and dispatch message to rest of system
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else //No message was found
		{
			//run game code
			UpdateScene();
			DrawScene();
		}
	}
	return msg.wParam;
}
//Wnd stuff
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			if (MessageBox(0,  "Are you sure you want to exit?",  "Really?", MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				DestroyWindow(hwnd);
			}
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
bool InitializeDirect3d11App(HINSTANCE hInstance)
{
	HRESULT hr;

	//Describe our Buffer
	DXGI_MODE_DESC bufferDesc;

	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

	bufferDesc.Width = Width;
	bufferDesc.Height = Height;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//Describe our SwapChain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


	//Create our SwapChain
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &d3d11Device, NULL, &d3d11DevCon);

	//Create our BackBuffer
	ID3D11Texture2D* BackBuffer;
	hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

	//Create our Render Target
	hr = d3d11Device->CreateRenderTargetView(BackBuffer, NULL, &renderTargetView);
	BackBuffer->Release();

	//Set our Render Target
	d3d11DevCon->OMSetRenderTargets(1, &renderTargetView, NULL);

	return true;
}
void ReleaseObjects()
{
	SwapChain->Release();
	d3d11Device->Release();
	d3d11DevCon->Release();
}
bool InitScene()
{
	return true;
}
void UpdateScene()
{
	//Update the colors of our scene
	red += colormodr * 0.00005f;
	green += colormodg * 0.00002f;
	blue += colormodb * 0.00001f;

	if (red >= 1.0f || red <= 0.0f)
	{
		colormodr *= -1;
	}
	if (green >= 1.0f || green <= 0.0f)
	{
		colormodg *= -1;
	}
	if (blue >= 1.0f || blue <= 0.0f)
	{
		colormodb *= -1;
	}
}
void DrawScene()
{
	D3DXCOLOR bgColor(red, green, blue, 1.0f);
	d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor);
	SwapChain->Present(0, 0);
}