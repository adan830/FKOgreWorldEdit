/******************************************************************
** �ļ���:	DecalSystem.h
** ��  Ȩ:	(C)  
** ������:	FreeKnight(������)
** ��  ��:	2008-1-10 9:53:30
** ��  ��:	����������ʵ�֣�����ָ����С������������Ȼ���Զ����������λ��
******************************************************************/
#pragma once
#include "SimpleDecal.h"
namespace Ogre
{
	class GridDecal : public SimpleDecal
	{
	public:
		GridDecal(SceneManager *mgr , const String &name);
		virtual ~GridDecal();

		// �������������
		virtual const DecalType getType()const
		{
			return DT_NORMAL;
		}

		// ���ú�����������
		void setWidthGridCount(uint gridCount);
		// ����������������
		void setHeightGridCount(uint gridCount);

		// ��ú�����������
		uint getWidthGridCount()const
		{
			return m_widthGridCount;
		}
		
		// ���������������
		uint getHeightGridCount()const
		{
			return m_heightGridCount;
		}

	protected:
		// �����Ըı��ˣ���Ҫ�ع�������ʱ��
		virtual void updateDecal();
		// ��������
		ManualObject *m_decal;
		// ����ƽ��
		Plane m_decalPlane;

		// �漰�����������Ա��ı���
		enum GRID_DECAL_DIRTY_FLAGS
		{
			GRID_DECAL_DIRTY_WIDTH_GRID_COUNT = 1 << (SimpleDecal::DIRTY_MAX_COUNT + 0),
			GRID_DECAL_DIRTY_HEIGHT_GRID_COUNT = 1 << (SimpleDecal::DIRTY_MAX_COUNT + 1),
			GRID_DECAL_DIRTY_MAX_COUNT		= 7,
		};
		// ������������
		uint m_widthGridCount;
		// ������������
		uint m_heightGridCount;

		// ��������������ĵ�����ײ����
		Vector3 *m_gridPositions;
	};

};