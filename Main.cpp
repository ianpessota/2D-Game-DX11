/////////////////////////////////////////
//This is the main program entry point //
//
//

#include<Windows.h>
#include<memory>
#include"Game.h"
#include"GameTime.h"
#include"resource.h"

//On test

#include "DebugCon.h"
#include "Templates.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

//Main entry point
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	#ifdef _DEBUG

	//RedirectIOToConsole();

	#endif

	//Non used window paramenters
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);

	//Window 
	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.lpszMenuName = NULL;
	wndClass.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_ICON1);;
	wndClass.lpszClassName = "WindowClass";

	//if can`t register, return
	if (!RegisterClassEx(&wndClass))
		return -1;

	//Window creation
	RECT rc = { 0, 0, 1280, 720 };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hwnd = CreateWindowA("WindowClass", //Class name
								"Game Project", // Window name
								WS_OVERLAPPED,//WS_POPUP,//WS_OVERLAPPEDWINDOW, //Window Style
								CW_USEDEFAULT, 
								CW_USEDEFAULT, 
								rc.right - rc.left,
								rc.bottom - rc.top, 
								NULL, 
								NULL, 
								hInstance,
								NULL);

	//if can't create a window, return
	if (!hwnd)
		return -1;

	//DemoApp* demoApp = new DemoApp(hwnd);
	
	//Show Window
	ShowWindow(hwnd, cmdShow);

	//Smart pointer, game class instance
	std::auto_ptr<GameBase> game(new Game());

	// Game Initialization
	bool result = game->Initialize(hInstance, hwnd);

	//if can`t initialize, return
	if (result == false)
		return -1;

	//Variables
	MSG msg = { 0 };
	GameTime* gameTime = new GameTime(60,true);
	
	
	
	
	//Main game loop
	while (msg.message != WM_QUIT)
	{
		
		//capture windows msg`s
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//if there`s none then...
		else
		{
			
				//Game runs
				gameTime->frameCount++;
				

				if (gameTime->GetTime() > gameTime->timeThreshold)
				{
					//if (gameTime->frameTime>0.00001f)
					//printf_s("frames %lf\n", gameTime->getFrameRate());
					
					gameTime->frameCount = 0;
					gameTime->StartTimer();
					
					game->Render();
				}
				gameTime->frameTime = gameTime->GetFrameTime();

				game->Update(gameTime);
				//demoApp->Initialize();
				
		}

	}
	//Game shutdown
	game->Shutdown();
	return static_cast<int>(msg.wParam);
}

//Message callback
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;
	HDC hDC;
	switch (message)
	{
	case WM_PAINT:
		hDC = BeginPaint(hwnd, &paintStruct);
		EndPaint(hwnd, &paintStruct);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}