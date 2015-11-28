#pragma once
#include "SingleSelectionAction.h"
namespace Ogre
{
	// �������θ߶�
	class RaiseHeightAction : public SingleSelectionAction
	{
	public:
		RaiseHeightAction(ETTerrain* pTerrain , SelectionRenderer *pSelectionRenderer , Selection *pSelection , TerrainBrush *pBrush);
		virtual ~RaiseHeightAction(void);
		// ���ӿ���Ϣ
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	};
};