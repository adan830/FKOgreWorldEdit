// ============================================================
// ������Ŀ	:	MMORPG
// �ļ���	:	ETTerrainAction.h
// ������	:	������
// ˵��		:	������Ϊ
// ����ʱ��	:	2007-8-20 15:31:25
// ============================================================

#ifndef __ETTERRAINACTION_H__
#define __ETTERRAINACTION_H__

#include "TerrainEditorDll.h"
#include "TerrainBrush.h"

namespace Ogre
{
	class SelectionRenderer;
	class Selection;

	class TERRAINEDITOR_API TerrainAction : public TerrainBrushListener
	{
	public:
		TerrainAction(ETTerrain* pTerrain , SelectionRenderer *pSelectionRenderer , Selection *pSelection , TerrainBrush *pBrush)
			: mSelectionRenderer(pSelectionRenderer)
			, mSelection(pSelection)
			, mBrush(pBrush)
			, mTerrain(pTerrain)
		{
			if(mBrush)
			{
				mBrush->addListener(this);
			}
		}
		virtual ~TerrainAction(void)
		{
			if(mBrush)
			{
				mBrush->removeListener(this);
			}
		}
		// add by ������ 2007-7-23
		// ��̬���ı��������
		void setSelection(Selection *pSelection)
		{
			mSelection = pSelection;
		}
		// add by ������ 2007-7-23
		// ��̬���ı��������
		void setSelectionRenderer(SelectionRenderer *pSelectionRenderer)
		{
			mSelectionRenderer = pSelectionRenderer;
		}
		// �ӿڸ���
		virtual void preViewportUpdate(const RenderTargetViewportEvent& evt){};

		
	protected:
		// ѡ����Ⱦ��
		SelectionRenderer *mSelectionRenderer;
		// ѡ���˵����ݱ��浽����
		Selection *mSelection;
		// ��ǰѡ��ı�ˢ
		TerrainBrush *mBrush;
		// ����ϵͳ
		ETTerrain* mTerrain;
	};
}
#endif