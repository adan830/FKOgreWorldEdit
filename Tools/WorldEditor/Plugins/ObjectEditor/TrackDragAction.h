#pragma once
#include "ObjectDragAction.h"

namespace Ogre
{
	// �켣��ҷ����
	class TrackDragAction : public ObjectDragAction
	{
	public:
		TrackDragAction(AnimationTrackObject *obj);
		virtual ~TrackDragAction(void);

		// ����������󣬵�����falseʱ���ٱ�������Ķ���
		virtual bool visit(ISceneObject *obj);
	protected:
		// �������ѡ�������ƽ�����ĵ�
		virtual bool getSelectedObjectsAvgCenter(Vector3 &center)const;

		// �����켣����
		AnimationTrackObject *m_object;
	};

};