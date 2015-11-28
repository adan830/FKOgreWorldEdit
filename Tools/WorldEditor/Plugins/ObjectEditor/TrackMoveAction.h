#pragma once
#include "ObjectMoveAction.h"

namespace Ogre
{
	// �켣�ƶ�����
	class TrackMoveAction : public ObjectMoveAction
	{
	public:
		TrackMoveAction(AnimationTrackObject *obj);
		virtual ~TrackMoveAction(void);

		// ����������󣬵�����falseʱ���ٱ�������Ķ���
		virtual bool visit(ISceneObject *obj);
	protected:
		// �������ѡ�������ƽ�����ĵ�
		virtual bool getSelectedObjectsAvgCenter(Vector3 &center)const;

		// �����켣����
		AnimationTrackObject *m_object;
	};

};