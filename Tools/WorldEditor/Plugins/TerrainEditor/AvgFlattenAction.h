#pragma once
#include "SingleSelectionAction.h"
namespace Ogre
{
	// Ĩƽ����
	class AvgFlattenAction : public SingleSelectionAction
		, public FrameListener
	{
	public:
		AvgFlattenAction(ETTerrain* pTerrain , SelectionRenderer *pSelectionRenderer , Selection *pSelection , TerrainBrush *pBrush);
		virtual ~AvgFlattenAction(void);
		// ���ӿ���Ϣ
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
		virtual bool frameStarted(const FrameEvent& evt);
	protected:
		bool m_canAction;
	};
};