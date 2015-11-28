/******************************************************************
** �ļ���:	DecalSystem.cpp
** ��  Ȩ:	(C)  
** ������:	FreeKnight(������)
** ��  ��:	2008-1-10 9:53:29
** ��  ��:
******************************************************************/
#include "stdafx.h"
#include "GridDecal.h"


namespace Ogre
{
	static const Vector3 Vector3Max(FLT_MAX , FLT_MAX , FLT_MAX);

	GridDecal::GridDecal(SceneManager *mgr , const String &name)
		: SimpleDecal(mgr , name)
		, m_decal(0)
		, m_widthGridCount(8)
		, m_heightGridCount(8)
		, m_gridPositions(0)
	{
	}

	GridDecal::~GridDecal()
	{
		OGRE_DELETE_ARRAY(m_gridPositions);
		if(m_decal)
		{
			m_sceneMgr->destroyManualObject(m_decal);
			m_decal = 0;
		}
	}
	// �����Ըı��ˣ���Ҫ�ع�������ʱ��
	void GridDecal::updateDecal()
	{
#define WIDTH_POS_COUNT (m_widthGridCount + 1)
#define HEIGHT_POS_COUNT (m_heightGridCount + 1)
		// ��ʾ�����ڵ�
		m_node->setVisible(m_visible);
		// �����ӾͲ�������
		if(!m_visible)
		{
			return;
		}
		// �������ݼ��
		if(m_heightGridCount == 0 || m_widthGridCount == 0)
			return;
		// ��������С�ı��ˣ����ع�
		if((m_dirty & (GRID_DECAL_DIRTY_WIDTH_GRID_COUNT
			| GRID_DECAL_DIRTY_HEIGHT_GRID_COUNT))
			|| m_gridPositions == 0
			)
		{
			OGRE_DELETE_ARRAY(m_gridPositions);
			m_gridPositions = new Vector3[WIDTH_POS_COUNT * HEIGHT_POS_COUNT];
			memset(m_gridPositions , 0 , sizeof(Vector3) * WIDTH_POS_COUNT * HEIGHT_POS_COUNT);
		}

		// λ�á���ȡ���С�ᵼ��ƽ�����¶���
		if(m_dirty & (DIRTY_POSITION
			| DIRTY_DEPTH
			| DIRTY_SIZE
			| GRID_DECAL_DIRTY_WIDTH_GRID_COUNT
			| GRID_DECAL_DIRTY_HEIGHT_GRID_COUNT)
			)
		{
			for(uint y = 0 ; y < WIDTH_POS_COUNT ; y ++)
			{
				for(uint x = 0 ; x < HEIGHT_POS_COUNT ; x ++)
				{
					// �ҵ�������ཻ�ĵ�
					if(!IDecalSystem::getSingleton().worldRayQuery(m_gridPositions[y * WIDTH_POS_COUNT + x]
						, m_position + Vector3(m_size.x * ((float)x / (float)WIDTH_POS_COUNT) - m_size.x / 2.0f , m_depth , m_size.y * ((float)y / (float)HEIGHT_POS_COUNT) - m_size.y / 2.0f)
						, m_position + Vector3(m_size.x * ((float)x / (float)WIDTH_POS_COUNT) - m_size.x / 2.0f , -m_depth , m_size.y * ((float)y / (float)HEIGHT_POS_COUNT) - m_size.y / 2.0f)
						)
						)
					{
						return;
					}
					// �ƶ������λ��
					m_gridPositions[y * WIDTH_POS_COUNT + x] -= m_position;
				}
			}
			// �����㶨������ƽ��
			m_decalPlane.redefine(m_gridPositions[WIDTH_POS_COUNT / 2]
								, m_gridPositions[WIDTH_POS_COUNT * (HEIGHT_POS_COUNT - 1)]
								, m_gridPositions[WIDTH_POS_COUNT * HEIGHT_POS_COUNT - 1]
								);
		}

		// �ı�ڵ�λ��
		if(m_dirty & DIRTY_POSITION)
		{
			m_node->setPosition(m_position);
		}

		// �������ʹ�С��Ӱ������������ع�
		if(m_dirty & (DIRTY_POSITION
			| DIRTY_DEPTH
			| DIRTY_MATERIAL_NAME
			| DIRTY_SIZE
			| GRID_DECAL_DIRTY_WIDTH_GRID_COUNT
			| GRID_DECAL_DIRTY_HEIGHT_GRID_COUNT)
			)
		{
			// ��������
			if(!m_materialName.empty())
			{
				// ������������
				if(m_decal == 0)
				{
					m_decal = m_sceneMgr->createManualObject(m_name + "_GridDecal");
					m_decal->setDynamic(true);
					m_decal->begin(m_materialName);
					m_decal->position(Vector3::ZERO);
					m_decal->textureCoord(0,0);
					m_decal->index(0);
					m_decal->end();
					// �����ڵ���֮�����֮ǰ
					m_decal->setRenderQueueGroup(RENDER_QUEUE_DECAL);
					attachObject(m_decal);
				}

				// ���ò���
				if(m_dirty & DIRTY_MATERIAL_NAME)
				{
					m_decal->getSection(0)->setMaterialName(m_materialName);
				}
				// ���¶���
				if(m_dirty & (DIRTY_POSITION
					| DIRTY_DEPTH
					| DIRTY_SIZE
					| GRID_DECAL_DIRTY_WIDTH_GRID_COUNT
					| GRID_DECAL_DIRTY_HEIGHT_GRID_COUNT)
					)
				{
					m_decal->beginUpdate(0);
					// �������ж���
					for(uint y = 0 ; y < HEIGHT_POS_COUNT ; y ++)
					{
						for(uint x = 0 ; x < WIDTH_POS_COUNT ; x ++)
						{
							m_decal->position(m_gridPositions[y * WIDTH_POS_COUNT + x]);
							m_decal->textureCoord((float)x / (float)WIDTH_POS_COUNT
								, (float)y / (float)HEIGHT_POS_COUNT);
						}
					}
					// ���ݶ�����������������
					for(uint y = 0 ; y < m_heightGridCount ; y ++)
					{
						for(uint x = 0 ; x < m_widthGridCount ; x ++)
						{
							uint tl = y * WIDTH_POS_COUNT + x;
							uint tr = y * WIDTH_POS_COUNT + x + 1;
							uint bl = (y + 1) * WIDTH_POS_COUNT + x;
							uint br = (y + 1) * WIDTH_POS_COUNT + x + 1;
							m_decal->quad(tl
								, bl
								, br
								, tr
								);
						}
					}
					
					m_decal->end();
				}
			}
			else
			{
				if(m_decal)
				{
					m_sceneMgr->destroyManualObject(m_decal);
					m_decal = 0;
				}
			}
		}

		// �ı丽�Ӷ���Ľڵ���ת
		if(m_nodeAttachObject)
		{
			// λ����Ϣ�ı����Ҫ��������
			if(m_dirty & (DIRTY_POSITION
				| DIRTY_DEPTH
				| DIRTY_SIZE
				| DIRTY_OFFSET)
				)
			{
				m_nodeAttachObject->setPosition(m_offsetPosition);
			}
		}
	}
	// ���ú�����������
	void GridDecal::setWidthGridCount(uint gridCount)
	{
		if(m_widthGridCount != gridCount)
		{
			m_dirty |= GRID_DECAL_DIRTY_WIDTH_GRID_COUNT;
		}
		m_widthGridCount = gridCount;
	}
	// ����������������
	void GridDecal::setHeightGridCount(uint gridCount)
	{
		if(m_heightGridCount != gridCount)
		{
			m_dirty |= GRID_DECAL_DIRTY_HEIGHT_GRID_COUNT;
		}
		m_heightGridCount = gridCount;
	}


};