#pragma once

#include "ObjectSelectAction.h"
namespace Ogre
{

	// �켣ѡ�����
	class TrackSelectAction : public ObjectSelectAction
	{
	public:
		TrackSelectAction(AnimationTrackObject *obj);
		virtual ~TrackSelectAction(void);
	protected:
		// ѡ��һ��MO
		virtual bool selectObject(MovableObject *obj , IMapManager::ListSceneObject *listSceneObject);
		// ��ѡ��֮ǰ�ȷ�ѡ�������
		virtual void deselectAllObject();
		// û��ѡ��
		virtual void onNoSelect();

		// �����켣����
		AnimationTrackObject *m_object;

	};

};