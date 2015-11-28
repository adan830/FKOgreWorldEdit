#pragma once
#include "SingleSelectionAction.h"
namespace Ogre
{
	// ���ϰ�����
	class BlockerAction : public SingleSelectionAction
		, public FrameListener
	{
	public:
		BlockerAction(bool isBlock , ETTerrain* pTerrain , SelectionRenderer *pSelectionRenderer , Selection *pSelection , TerrainBrush *pBrush);
		virtual ~BlockerAction(void);
		// ���ӿ���Ϣ
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
		virtual bool frameStarted(const FrameEvent& evt);
	protected:
		bool m_isBlock;
		bool m_canAction;
	};
};