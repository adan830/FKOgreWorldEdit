#pragma once
#include "ETSelection.h"
#include "ETTerrain.h"
#include "SelectionRenderer.h"
#include "IDecalSystem.h"

namespace Ogre
{

	// ������Ⱦ��ѡȡ
	class DecalSelectionRenderer : public SelectionRenderer
	{
	public:
		DecalSelectionRenderer(Selection *pSelection , TerrainBrush *brush);
		virtual ~DecalSelectionRenderer(void);

		// ���ѡ��
		virtual void addSelPre(ushort x, ushort z, const Brush &brush, float intensity){};
		virtual void addSelPost(ushort x, ushort z, const Brush &brush, float intensity){};
		// ����ˢ���ѡ��
		virtual void clearSelPre(ushort x, ushort z, const Brush &brush){};
		virtual void clearSelPost(ushort x, ushort z, const Brush &brush){};
		// add by ������ 2007-7-27
		// �������
		virtual void clearPre(){};
		virtual void clearPost(){};
		// add by ������ 2007-7-27
		// ����λ��
		virtual void setPositionPre(const GridVector2 &pos){};
		virtual void setPositionPost(const GridVector2 &pos);

		virtual void updateBrush();

	protected:
		// ����ʵ��
		IDecal *m_decal;
		MaterialPtr m_material;
		TexturePtr m_texture;
	};

};