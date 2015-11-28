#pragma once
#include "TerrainAction.h"
namespace Ogre
{
	// ѡ�����
	class SelectTerrainAction : public TerrainAction
		, public IChildView::Listener
	{
	public:
		SelectTerrainAction(ETTerrain* pTerrain , SelectionRenderer *pSelectionRenderer , Selection *pSelection , TerrainBrush *pBrush);
		virtual ~SelectTerrainAction(void);
		// ���ӿ���Ϣ
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	};
};