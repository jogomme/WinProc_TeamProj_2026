#include "stdafx.h"
#include "Scene.h"

//x, y ÀÌ¹ÌÁö
// ±ê ¾îÆ®¸®ºäÆ®
// git ignore
// git ignore 2
// git 3
CScene::CScene()
{

}

CScene::~CScene()
{
}

void CScene::BuildObjects()
{
	

#ifdef _WITH_DRAW_AXIS
	m_pWorldAxis = new CGameObject();
	CAxisMesh* pAxisMesh = new CAxisMesh(0.5f, 0.5f, 0.5f);
	m_pWorldAxis->SetMesh(pAxisMesh);
#endif
}

void CScene::ReleaseObjects()
{

#ifdef _WITH_DRAW_AXIS
	if (m_pWorldAxis) delete m_pWorldAxis;
#endif
}

void CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case 'A':
		default:
			break;
		}
		break;
	default:
		break;
	}
}

//CGameObject* CScene::PickObjectPointedByCursor(int xClient, int yClient)
//{
//	XMFLOAT3 xmf3PickPosition;
//	xmf3PickPosition.x = (((2.0f * xClient) / (float)pCamera->m_Viewport.m_nWidth) - 1) / pCamera->m_xmf4x4PerspectiveProject._11;
//	xmf3PickPosition.y = -(((2.0f * yClient) / (float)pCamera->m_Viewport.m_nHeight) - 1) / pCamera->m_xmf4x4PerspectiveProject._22;
//	xmf3PickPosition.z = 1.0f;
//
//	XMVECTOR xmvPickPosition = XMLoadFloat3(&xmf3PickPosition);
//	XMMATRIX xmmtxView = XMLoadFloat4x4(&pCamera->m_xmf4x4View);
//
//	int nIntersected = 0;
//	float fNearestHitDistance = FLT_MAX;
//	CGameObject* pNearestObject = NULL;
//	for (int i = 0; i < m_nObjects; i++)
//	{
//		float fHitDistance = FLT_MAX;
//		nIntersected = m_ppObjects[i]->PickObjectByRayIntersection(xmvPickPosition, xmmtxView, &fHitDistance);
//		if ((nIntersected > 0) && (fHitDistance < fNearestHitDistance))
//		{
//			fNearestHitDistance = fHitDistance;
//			pNearestObject = m_ppObjects[i];
//		}
//	}
//	return(pNearestObject);
//}


void CScene::Animate(float fElapsedTime)
{

}

void CScene::Render(HDC hDCFrameBuffer)
{

//UI
#ifdef _WITH_DRAW_AXIS
	CGraphicsPipeline::SetViewOrthographicProjectTransform(&pCamera->m_xmf4x4ViewOrthographicProject);
	m_pWorldAxis->SetRotationTransform(&m_pPlayer->m_xmf4x4World);
	m_pWorldAxis->Render(hDCFrameBuffer, pCamera);
#endif
}
