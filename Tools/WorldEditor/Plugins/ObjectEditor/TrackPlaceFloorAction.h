#pragma once
#include "ObjectPlaceFloorAction.h"

namespace Ogre
{
	// �켣���ز���
	class TrackPlaceFloorAction : public ObjectPlaceFloorAction
	{
	public:
		TrackPlaceFloorAction(AnimationTrackObject *obj);
		virtual ~TrackPlaceFloorAction(void);

		// ����������󣬵�����falseʱ���ٱ�������Ķ���
		virtual bool visit(ISceneObject *obj);

	protected:
		// �����켣����
		AnimationTrackObject *m_object;
	};

};