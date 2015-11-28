#pragma once
#include "ObjectScaleAction.h"

namespace Ogre
{
	// �켣���Ų���
	class TrackScaleAction : public ObjectScaleAction
	{
	public:
		TrackScaleAction(AnimationTrackObject *obj);
		virtual ~TrackScaleAction(void);

		// ����������󣬵�����falseʱ���ٱ�������Ķ���
		virtual bool visit(ISceneObject *obj);
	protected:
		// �������ѡ�������ƽ�����ĵ�
		virtual bool getSelectedObjectsAvgCenter(Vector3 &center)const;

		// �����켣����
		AnimationTrackObject *m_object;
	};

};