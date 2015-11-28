#pragma once
#include "SingleSelectionAction.h"
namespace Ogre
{
	// �����򽵵͵��θ߶�
	class RaiseUpDownAction : public SingleSelectionAction
		, public FrameListener
	{
	public:
		RaiseUpDownAction(bool raiseUp , ETTerrain* pTerrain , SelectionRenderer *pSelectionRenderer , Selection *pSelection , TerrainBrush *pBrush);
		virtual ~RaiseUpDownAction(void);
		// ���ӿ���Ϣ
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
		virtual bool frameStarted(const FrameEvent& evt);

	protected:
		bool m_canAction;
		bool m_up;
	};
};