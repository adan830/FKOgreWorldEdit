#pragma once
#include "TerrainAction.h"
#include "ETSelection.h"
#include "ETTerrain.h"
#include "SelectionRenderer.h"
#include "TerrainBrush.h"

namespace Ogre
{

	// ��ѡ��
	class SingleSelectionAction : public TerrainAction
		, public IChildView::Listener
	{
	public:
		SingleSelectionAction(ETTerrain* pTerrain
			, SelectionRenderer *pSelectionRenderer
			, Selection *pSelection 
			, TerrainBrush *pBrush
			);
		virtual ~SingleSelectionAction(void);
		// ���ӿ���Ϣ
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
		// �ӿڸ���
		virtual void preViewportUpdate(const RenderTargetViewportEvent& evt);

		virtual void updateBrush();

	};
};