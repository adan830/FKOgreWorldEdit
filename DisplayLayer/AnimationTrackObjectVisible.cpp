#include "StdAfx.h"
#include "AnimationTrackObjectVisible.h"
#include "IScene.h"
#include "DisplaySystem.h"
#include "AnimationTrackObject.h"

namespace Ogre
{

	AnimationTrackObjectVisible::AnimationTrackObjectVisible(AnimationTrackObject *obj)
		: NullObjectVisible(obj)
		, m_trackLines(0)
		, m_controller(0)
		, m_trackNode(0)
		, m_needRebuild(true)
		, m_showKeyNode(true)
	{
		destroyVisibleSphere();
		m_sphereColor = ColourValue(0 , 1 , 1);
		createVisibleSphere();

		// ������������켣�ߵĿ�����
		m_controller = static_cast<AnimationTrackController*>(static_cast<AnimationTrackObject*>(m_object)->createTrack(m_object));

		// һ��ʼ���ؽ�
		rebuild();

		Root::getSingleton().addFrameListener(this);
	}

	AnimationTrackObjectVisible::~AnimationTrackObjectVisible(void)
	{
		Root::getSingleton().removeFrameListener(this);
		// ���ٹ켣��
		destroyTrackLines();
		// ���ٿ�����
		if(m_controller)
		{
			static_cast<AnimationTrackObject*>(m_object)->destroyTrack(m_controller);
			m_controller = 0;
		}
	}
	// ��ʾ�ؼ���
	void AnimationTrackObjectVisible::showKeyNode(bool show)
	{
		m_showKeyNode = show;
		if(m_trackKeyNode)
		{
			m_trackKeyNode->setVisible(m_showKeyNode);
		}
	}

	// ��ʾ�߿�
	void AnimationTrackObjectVisible::showBorder(bool show)
	{
		NullObjectVisible::showBorder(show);
		if(m_trackNode)
		{
			m_trackNode->setVisible(show);
		}
		if(m_trackKeyNode)
		{
			m_trackKeyNode->setVisible(m_showKeyNode && show);
		}
	}
	// �ؽ��ɼ�����
	void AnimationTrackObjectVisible::_notifyRebuild()
	{
		m_needRebuild = true;
	}
	// ���ٹ켣��
	void AnimationTrackObjectVisible::destroyTrackLines()
	{
		// ���ٹ켣��
		if(m_trackLines)
		{
			m_scene->getOgreSceneManager()->destroyManualObject(m_trackLines);
			m_trackLines = 0;
		}
		// ���ٹؼ���
		for(ListKeyNode::iterator iter = m_listKeyNode.begin() ; iter != m_listKeyNode.end() ; iter ++)
		{
			m_scene->getOgreSceneManager()->destroyEntity(*iter);
		}
		m_listKeyNode.clear();
		// �������нڵ�
		if(m_trackNode)
		{
			m_trackNode->getParentSceneNode()->removeAndDestroyChild(m_trackNode->getName());
			m_trackNode = 0;
			m_trackKeyNode = 0;
		}
	}

	// �ؽ��ɼ�����
	void AnimationTrackObjectVisible::rebuild()
	{
		if(!m_needRebuild)
			return;
		// �����ٹ켣��
		destroyTrackLines();
		// ���ؽ�����������
		m_controller->_rebuild();
		// �����켣��
		m_trackLines = m_scene->getOgreSceneManager()->createManualObject(m_object->getFullName() + "_TrackLines");

		// �����켣�ڵ�
		m_trackNode = m_scene->getOgreSceneManager()->getRootSceneNode()->createChildSceneNode();
		// �ѹ켣�߹ҽӵ��ڵ���ȥ
		m_trackNode->attachObject(m_trackLines);

		float timePos = 0.0f;
		NodeAnimationTrack *track = m_controller->getNodeAnimationTrack();
		Animation *animation = m_controller->getAnimation();
		// ����һ���ڵ��������ն�����Ϣ
		SceneNode *node = m_scene->getOgreSceneManager()->getRootSceneNode()->createChildSceneNode();
		m_trackLines->begin(DisplaySystem::getSingleton().getColorMaterial(ColourValue::Red) , RenderOperation::OT_LINE_STRIP);
		// һֱѭ�������
		while(timePos < m_controller->getLength())
		{
			node->resetToInitialState();
			// �Ѷ�����ϢӦ�õ�node��
			track->applyToNode(node , animation->_getTimeIndex(timePos));
			// ����ı���ߵ����һ���ڵ㣬��д��
			m_trackLines->position(node->getPosition());
			// ÿ0.1�����һ��
			timePos += 0.1f;
		}
		m_trackLines->end();

		// ��ԭʼ�Ĺ켣
		m_trackLines->begin(DisplaySystem::getSingleton().getColorMaterial(ColourValue::Blue) , RenderOperation::OT_LINE_STRIP);
		{
			AnimationTrackObject::ConstKeyFrameIterator iterator = m_controller->getAnimationTrackObject()->getConstKeyFrameIterator();
			while(iterator.hasMoreElements())
			{
				const AnimationTrackObject::KeyFrame &keyFrame = iterator.getNext();
				m_trackLines->position(keyFrame.m_translate);
			}
		}
		m_trackLines->end();

		// ���ؼ���
		{
			AnimationTrackObject::ConstKeyFrameIterator iterator = m_controller->getAnimationTrackObject()->getConstKeyFrameIterator();
			size_t index = 0;
			m_trackKeyNode = m_trackNode->createChildSceneNode();
			while(iterator.hasMoreElements())
			{
				const AnimationTrackObject::KeyFrame &keyFrame = iterator.getNext();
				SceneNode *child = m_trackKeyNode->createChildSceneNode(keyFrame.m_translate , keyFrame.m_rotate);
				// ����һ���ؼ���
				Entity *entity = m_scene->getOgreSceneManager()->createEntity(m_object->getFullName() + "_KeyFrame_" + StringConverter::toString(index++) , "axes.mesh");
				child->setScale(keyFrame.m_scale * (1.0f / entity->getBoundingRadius()));
				m_listKeyNode.push_back(entity);
				child->attachObject(entity);
			}
		}


		// ��������ڵ�
		m_scene->getOgreSceneManager()->destroySceneNode(node->getName());

		showBorder(isShowBorder());
		m_needRebuild = false;
	}

};
