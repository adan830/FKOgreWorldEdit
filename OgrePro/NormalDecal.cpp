/******************************************************************
** �ļ���:	DecalSystem.cpp
** ��  Ȩ:	(C)  
** ������:	FreeKnight(������)
** ��  ��:	2008-1-10 9:53:29
** ��  ��:
******************************************************************/
#include "stdafx.h"
#include "NormalDecal.h"


namespace Ogre
{
	static const Vector3 Vector3Max(FLT_MAX , FLT_MAX , FLT_MAX);

	NormalDecal::NormalDecal(SceneManager *mgr , const String &name)
		: SimpleDecal(mgr , name)
		, m_decal(0)
	{
	}
	
	NormalDecal::~NormalDecal()
	{
		if(m_decal)
		{
			m_sceneMgr->destroyManualObject(m_decal);
			m_decal = 0;
		}
	}

	// �����Ըı��ˣ���Ҫ�ع�������ʱ��
	void NormalDecal::updateDecal()
	{
		// ��ʾ�����ڵ�
		m_node->setVisible(m_visible);
		// �����ӾͲ�������
		if(!m_visible)
		{
			return;
		}

		// λ�á���ȡ���С�ᵼ��ƽ�����¶���
		if((m_dirty & DIRTY_POSITION)
			|| (m_dirty & DIRTY_DEPTH)
			|| (m_dirty & DIRTY_SIZE)
			)
		{
			Vector3 centerTop(m_position + Vector3(0 , m_depth , 0));
			Vector3 bottomTop(m_position + Vector3(0 , -m_depth , m_size.y));
			Vector3 bottomLeft(m_position + Vector3(-m_size.x , -m_depth , -m_size.y));
			Vector3 bottomRight(m_position + Vector3(m_size.x , -m_depth , -m_size.y));

			// �����߼����������㣬�������������㷨��
			Vector3 pos1(Vector3Max) , pos2(Vector3Max) , pos3(Vector3Max);

			// �������һ�����Ҳ������Ͳ�������
			if(!IDecalSystem::getSingleton().worldRayQuery(pos1 , centerTop , bottomTop))return;
			if(!IDecalSystem::getSingleton().worldRayQuery(pos2 , centerTop , bottomLeft))return;
			if(!IDecalSystem::getSingleton().worldRayQuery(pos3 , centerTop , bottomRight))return;

			// �����㶨������ƽ��
			m_decalPlane.redefine(pos1 , pos2 , pos3);
		}
		
		// �ı�ڵ�λ��
		if(m_dirty & DIRTY_POSITION)
		{
			m_node->setPosition(m_position);
		}

		// �������ʹ�С��Ӱ������������ع�
		if((m_dirty & DIRTY_MATERIAL_NAME)
			|| (m_dirty & DIRTY_SIZE)
			)
		{
			// ��������
			if(!m_materialName.empty())
			{
				// ������������
				if(m_decal == 0)
				{
					m_decal = m_sceneMgr->createManualObject(m_name + "_NormalDecal");
					m_decal->setDynamic(true);
					m_decal->begin(m_materialName);
					m_decal->position(Vector3::ZERO);
					m_decal->textureCoord(0,0);
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
				if(m_dirty & DIRTY_SIZE)
				{
					m_decal->beginUpdate(0);
					m_decal->position(-m_size.x , 0 , m_size.y);
					m_decal->textureCoord(0 , 0);
					m_decal->position(m_size.x , 0 , -m_size.y);
					m_decal->textureCoord(1 , 1);
					m_decal->position(-m_size.x , 0 , -m_size.y);
					m_decal->textureCoord(0 , 1);

					m_decal->position(-m_size.x , 0 , m_size.y);
					m_decal->textureCoord(0 , 0);
					m_decal->position(m_size.x , 0 , m_size.y);
					m_decal->textureCoord(1 , 0);
					m_decal->position(m_size.x , 0 , -m_size.y);
					m_decal->textureCoord(1 , 1);
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
			if((m_dirty & DIRTY_POSITION)
				|| (m_dirty & DIRTY_DEPTH)
				|| (m_dirty & DIRTY_SIZE)
				|| (m_dirty & DIRTY_OFFSET)
				)
			{
				m_nodeAttachObject->setOrientation(Vector3::UNIT_Y.getRotationTo(-m_decalPlane.normal) * m_offsetOrientation);
				m_nodeAttachObject->setPosition(m_offsetPosition);
				m_nodeAttachObject->setScale(m_offsetScale);
			}
		}
	}


};