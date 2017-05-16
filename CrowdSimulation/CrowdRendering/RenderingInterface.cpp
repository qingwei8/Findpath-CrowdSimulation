//#include "RenderingInterface.h"
//
//using namespace hiveCrowdRendering;
//
////*******************************************************************
////FUNCTION:
//LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
//{
//	switch (uMsg)
//	{
//		case WM_CLOSE:
//			PostQuitMessage(0);
//			break;
//		default:
//			break;
//	}
//	return (DefWindowProc(hWnd, uMsg, wParam, lParam));
//}
//
////*******************************************************************
////FUNCTION:
//void createWindow(struct SWindow *vWindow)
//{
//	WNDCLASSEX WinClass;
//	HINSTANCE Instance = GetModuleHandle(0);
//	int LengthOfName = vWindow->Name.length();
//	wchar_t *pUnicode = new wchar_t(sizeof(wchar_t) * LengthOfName);
//	int LengthOfUnicode = MultiByteToWideChar(CP_UTF8, 0, vWindow->Name.c_str(), -1, NULL, 0);
//	MultiByteToWideChar(CP_UTF8, 0, vWindow->Name.c_str(), -1, (LPWSTR)pUnicode, LengthOfUnicode);
//	
//	WinClass.cbSize = sizeof(WNDCLASSEX);
//	WinClass.style = CS_HREDRAW | CS_VREDRAW;
//	WinClass.lpfnWndProc = WndProc;
//	WinClass.cbClsExtra = 0;
//	WinClass.cbWndExtra = 0;
//	WinClass.hInstance = Instance; 
//	WinClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
//	WinClass.hCursor = LoadCursor(NULL, IDC_ARROW);
//	WinClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
//	WinClass.lpszMenuName = NULL;
//	WinClass.lpszClassName = (LPCSTR)pUnicode;
//	WinClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
//	if (!RegisterClassEx(&WinClass))
//	{
//		hiveCommon::hiveOutputEvent("Unexpected error trying to start the application!\n", hiveCommon::WARNING_LEVEL);
//		exit(1);
//	}
//	RECT WindowRect = {0, 0, vWindow->Width, vWindow->Height};
//	AdjustWindowRect(&WindowRect, WS_OVERLAPPEDWINDOW, FALSE);
//	vWindow->Window = CreateWindowEx(0,
//		(LPCSTR)pUnicode,          
//		(LPCSTR)pUnicode,           
//		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_SYSMENU,
//		100, 100,           
//		WindowRect.right - WindowRect.left, 
//		WindowRect.bottom - WindowRect.top, 
//		NULL,               
//		NULL,             
//		Instance,  
//		NULL);              
//	if (!vWindow->Window) 
//	{
//		hiveCommon::hiveOutputEvent("Cannot create a window in which to draw!\n", hiveCommon::WARNING_LEVEL);
//		exit(1);
//	}
//}
//
//////*******************************************************************
//////FUNCTION:
////void hiveCrowdRendering::hiveCreateWindow(int vWidth, int vHeight)
////{
////	struct SWindow Window;
////	MSG Msg;   
////	Window.Width = vWidth;
////	Window.Height = vHeight;
////	Window.Name = APP_SHORT_NAME;
////	createWindow(&Window);
////
////	bool Done = false; 
////	while (!Done)
////	{
////		PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE);
////		if (Msg.message == WM_QUIT) 
////		{
////			Done = true; 
////		}
////		else
////		{
////			TranslateMessage(&Msg);
////			DispatchMessage(&Msg);
////		}
////		RedrawWindow(Window.Window, NULL, NULL, RDW_INTERNALPAINT);
////	}
////}
////
////// *******************************************************************
////// FUNCTION:
////CROWD_RENDERING_DLL_EXPORT void hiveCrowdRendering::loadModel(const std::string& vFileName)
////{
////	CLoadModel* pVulkanExample = new CLoadModel();
////
////	pVulkanExample->initSwapChain();
////	pVulkanExample->prepare(vFileName);
////	pVulkanExample->renderLoop();
////
////	delete(pVulkanExample);
////}
