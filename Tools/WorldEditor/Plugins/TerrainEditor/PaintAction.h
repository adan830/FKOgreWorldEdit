#pragma once
#include "SingleSelectionAction.h"
namespace Ogre
{
	// ��������
	class PaintAction : public SingleSelectionAction
		, public FrameListener
	{
	public:
		PaintAction(size_t paintChannel , ETTerrain* pTerrain , SelectionRenderer *pSelectionRenderer , Selection *pSelection , TerrainBrush *pBrush);
		virtual ~PaintAction(void);
		// ���ӿ���Ϣ
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
		virtual bool frameStarted(const FrameEvent& evt);
	protected:
		size_t mPaintChannel;
		bool m_canAction;
	};
};