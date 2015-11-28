#pragma once

#include "ETSelection.h"
#include "ETTerrain.h"
#include "MovableText.h"
#include "SelectionRenderer.h"
namespace Ogre
{
	// add by ������ 2007-7-27
	// ʵ���߿���Ⱦ��ѡ����Ⱦ��
	class LineDecalSelectionRenderer : public SelectionRenderer
		, public SimpleRenderable
	{
	public:
		LineDecalSelectionRenderer(Selection *pSelection , TerrainBrush *brush);
		virtual ~LineDecalSelectionRenderer(void);

		// ���ѡ��
		virtual void addSelPre(Ogre::ushort x, Ogre::ushort z, const Brush &brush, float intensity){};
		virtual void addSelPost(Ogre::ushort x, Ogre::ushort z, const Brush &brush, float intensity)
		{
			updateVertexBuffer();
			updateIndexBuffer();
		};
		// ����ˢ���ѡ��
		virtual void clearSelPre(Ogre::ushort x, Ogre::ushort z, const Brush &brush){};
		virtual void clearSelPost(Ogre::ushort x, Ogre::ushort z, const Brush &brush)
		{
			updateVertexBuffer();
			updateIndexBuffer();
		};
		// �������
		virtual void clearPre(){};
		virtual void clearPost()
		{
			updateVertexBuffer();
			updateIndexBuffer();
		};
		// ����λ��
		virtual void setPositionPre(const GridVector2 &pos){};
		virtual void setPositionPost(const GridVector2 &pos)
		{
			updateVertexBuffer();
			updateIndexBuffer();
		};
		// �������ݣ�һ����TerrainAction�ص�
		virtual void redraw()
		{
			updateVertexBuffer();
			updateIndexBuffer();
		};
		virtual void getRenderOperation(RenderOperation& op)
		{
			// �ػ�
			if(m_needRedraw)
			{
				redraw();
				m_needRedraw = false;
			}
			SimpleRenderable::getRenderOperation(op);
		}
		virtual void updateBrush()
		{
			m_needRedraw = true;
		}


		Real getSquaredViewDepth(const Camera *cam) const;
		Real getBoundingRadius() const;
	protected:
		void updateVertexBuffer();
		void updateIndexBuffer();

		const Quaternion &getWorldOrientation() const;
		const Vector3 &getWorldPosition() const;

		// add by ������ 2007-8-10
		// ���汻ѡ�е���������������
		typedef std::map<GridVector2 , ushort> MapGridVector2;
		MapGridVector2 mGridPos;

	};
};