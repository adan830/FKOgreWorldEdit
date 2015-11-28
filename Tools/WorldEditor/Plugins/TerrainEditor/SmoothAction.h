#pragma once
#include "SingleSelectionAction.h"
namespace Ogre
{
	// ����ƽ������
	class SmoothAction : public SingleSelectionAction
		, public FrameListener
	{
	public:
		SmoothAction(ETTerrain* pTerrain , SelectionRenderer *pSelectionRenderer , Selection *pSelection , TerrainBrush *pBrush);
		virtual ~SmoothAction(void);
		// ���ӿ���Ϣ
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
		virtual bool frameStarted(const FrameEvent& evt);
	protected:
		bool m_canAction;
	};
};