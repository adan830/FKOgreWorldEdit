#pragma once
#include "ObjectCloneAction.h"

namespace Ogre
{
	// �켣��¡����
	class TrackCloneAction : public ObjectCloneAction
	{
	public:
		TrackCloneAction(AnimationTrackObject *obj);
		virtual ~TrackCloneAction(void);

		// ����������󣬵�����falseʱ���ٱ�������Ķ���
		virtual bool visit(ISceneObject *obj);

	protected:
		// �����켣����
		AnimationTrackObject *m_object;
	};

};