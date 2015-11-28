#include "StdAfx.h"
#include "PaintAction.h"
#include "TerrainEditorPlugin.h"

namespace Ogre
{
	PaintAction::PaintAction(size_t paintChannel , ETTerrain* pTerrain , SelectionRenderer *pSelectionRenderer , Selection *pSelection , TerrainBrush *pBrush)
		: SingleSelectionAction(pTerrain , pSelectionRenderer , pSelection , pBrush)
		, mPaintChannel(paintChannel)
		, m_canAction(false)
	{
		Root::getSingleton().addFrameListener(this);
	}

	PaintAction::~PaintAction(void)
	{
		Root::getSingleton().removeFrameListener(this);
	}

	// ���ӿ���Ϣ
	bool PaintAction::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		switch(message)
		{
		case WM_LBUTTONDOWN:
			m_canAction = true;
			break;
		case WM_LBUTTONUP:
			m_canAction = false;
			break;
		}
		return SingleSelectionAction::onWndMsg(view , message , wParam , lParam , pResult);
	}

	bool PaintAction::frameStarted(const FrameEvent& evt)
	{
		if(m_canAction)
		{
			// ������
			mTerrain->getSplattingManager()->paint(mPaintChannel, *mSelection , mBrush->getStrength());
		}
		return true;
	}
}; 