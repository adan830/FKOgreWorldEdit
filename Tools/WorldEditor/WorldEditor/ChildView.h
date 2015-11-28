// View1.h : header file
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2007 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIEW1_H__DB5DD4BB_239B_4D1D_8478_3CA079B61C83__INCLUDED_)
#define AFX_VIEW1_H__DB5DD4BB_239B_4D1D_8478_3CA079B61C83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CChildView view
#include "WorldEditor.h"
#include "IChildView.h"
#include "TListener.h"
using namespace Ogre;
class CChildView : public CView
	, public IChildView
	, public RenderTargetListener
	, public TListener<IChildView::Listener>
{
protected:
	CChildView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChildView)

	// Attributes
public:

	// Operations
public:


	// ������ӿ�������
	virtual void addListener(Listener *listener , bool toBack = true)
	{
		TListener<IChildView::Listener>::addListener(listener , toBack);
	}
	// ɾ�����ӿ�������
	virtual void removeListener(Listener *listener)
	{
		TListener<IChildView::Listener>::removeListener(listener);
	}


	// ��ô���ָ��
	virtual CWnd* getWnd();
	// ��ô��ڴ�С
	virtual CRect getWndRect();

	// ��������ӿڵ������
	virtual void setCamera(Camera *camera);
	// �������ӿڵ������
	virtual Camera *getCamera()const;

	// ���������ҽӵĽڵ�
	virtual SceneNode *getMoveCameraSceneNode()const
	{
		return m_moveCameraNode;
	}
	// ���������ҽӵĽڵ�
	virtual SceneNode *getRotateCameraSceneNode()const
	{
		return m_rotateCameraNode;
	}


	// �����Ⱦ����
	virtual RenderWindow *getRenderWindow()const;

	// ����ӿ�
	virtual Viewport *getViewport()const;

	// �������������
	virtual void setCameraType(CAMERA_TYPE type);
	// ������������
	virtual CAMERA_TYPE getCameraType()const;


	// ������һ��WM_MOUSEMOVEʱ����ƶ��˶��پ���
	virtual const CPoint &getMouseMoveDistance()
	{
		return m_moveDistance;
	}
	virtual void preViewportUpdate(const RenderTargetViewportEvent& evt);
	virtual void postViewportUpdate(const RenderTargetViewportEvent& evt);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView)
protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

	// ��ʼ��
	void init();
	// ����
	void destroy();

	// Implementation
protected:
	virtual ~CChildView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// ��Ⱦ����
	RenderWindow *m_window;
	// ��Ⱦ�ӿ�
	Viewport *m_viewport;
	// �����
	Camera *m_camera;
	// ���������
	CAMERA_TYPE m_cameraType;

	// ������ҽӵĽڵ�
	SceneNode *m_moveCameraNode;
	// ������ҽӵĽڵ�
	SceneNode *m_rotateCameraNode;


	// �������λ��
	CPoint m_lastPosition;
	// ����ƶ��ľ���
	CPoint m_moveDistance;


	// Generated message map functions
protected:
	//{{AFX_MSG(CChildView)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEW1_H__DB5DD4BB_239B_4D1D_8478_3CA079B61C83__INCLUDED_)


