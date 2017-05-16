//#include "CrowdRender.h"
//
//
//CCrowdRender::CCrowdRender()
//{
//	m_pLoadModel = new CLoadModel();
//}
//
//CCrowdRender::~CCrowdRender()
//{
//
//}
//
////*******************************************************************
////FUNCTION:
//void CCrowdRender::renderScene(HINSTANCE vHInstance)
//{
//	
//	m_pLoadModel->setupWindow(vHInstance, __WndProc);
//	m_pLoadModel->initSwapChain();
//	m_pLoadModel->prepare("data/ColorScene.obj");
//	m_pLoadModel->renderLoop();
//}
//
////*******************************************************************
////FUNCTION:
//LRESULT CALLBACK CCrowdRender::__WndProc(HWND vHWnd, UINT vMsg, WPARAM vWParam, LPARAM vLParam)
//{
//	if (m_pLoadModel != NULL)
//	{
//		m_pLoadModel->handleMessages(vHWnd, vMsg, vWParam, vLParam);
//	}
//
//	return (DefWindowProc(vHWnd, vMsg, vWParam, vLParam));
//}
