#pragma once
#include "ObjectRotateAction.h"

namespace Ogre
{
	// �켣��ת����
	class TrackRotateAction : public ObjectRotateAction
	{
	public:
		TrackRotateAction(AnimationTrackObject *obj);
		virtual ~TrackRotateAction(void);

		// ����������󣬵�����falseʱ���ٱ�������Ķ���
		virtual bool visit(ISceneObject *obj);
	protected:
		// �������ѡ�������ƽ�����ĵ�
		virtual bool getSelectedObjectsAvgCenter(Vector3 &center)const;

		// �����켣����
		AnimationTrackObject *m_object;
	};

};