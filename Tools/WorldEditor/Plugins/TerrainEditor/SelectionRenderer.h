// ============================================================
// ������Ŀ	:	MMORPG
// �ļ���	:	ETSelectionRenderer.h
// ������	:	������
// ˵��		:	��ѡ������Ⱦ��
// ����ʱ��	:	2007-7-27 14:11:49
// ============================================================

#ifndef __ETSELECTIONRENDERER_H__
#define __ETSELECTIONRENDERER_H__

#include "TerrainEditorDll.h"
#include "ETSelection.h"
#include "ETTerrain.h"
#include "TerrainBrush.h"
namespace Ogre
{

	// add by ������ 2007-7-27
	// ��ѡ������Ⱦ��
	class TERRAINEDITOR_API SelectionRenderer : public SelectionListener
		, public TerrainBrushListener
	{
	public:
		SelectionRenderer(Selection *pSelection , TerrainBrush *brush)
			: m_selection(pSelection)
			, m_brush(brush)
			, m_needRedraw(true)
		{
			m_selection->addSelectionListener(this);
			m_brush->addListener(this);
		}

		virtual ~SelectionRenderer(void)
		{
			m_brush->removeListener(this);
			m_selection->removeSelectionListener(this);
		}

		// ���������ػ�
		virtual void redraw(){};
		// ��Ҫ�ػ�
		void needRedraw(){m_needRedraw = true;}

	protected:
		// ѡ��
		Selection *m_selection;

		TerrainBrush *m_brush;

		bool m_needRedraw;

	};	
}
#endif