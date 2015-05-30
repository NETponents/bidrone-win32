//Includes
#include <Windows.h>

//Global variables
LPCTSTR WndClassName = "firstwindow";
HWND hwnd = NULL;
const int Width = 800;
const int Height = 600;

//Function prototypes
bool InitializeWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed);
int messageloop();
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	if (!InitializeWindow(hInstance, nShowCmd, Width, Height, true))
	{
		MessageBox(0,  "Window Initialization - Failed",  "Error", MB_OK);
		return 0;
	}

	messageloop();

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