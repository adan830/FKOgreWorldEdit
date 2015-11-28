#pragma once
#include "ObjectDeleteAction.h"

namespace Ogre
{
	// �켣ɾ������
	class TrackDeleteAction : public ObjectDeleteAction
	{
	public:
		TrackDeleteAction(AnimationTrackObject *obj);
		virtual ~TrackDeleteAction(void);

		// ����������󣬵�����falseʱ���ٱ�������Ķ���
		virtual bool visit(ISceneObject *obj);

		// ɾ�����
		virtual void deleteObject();

	protected:
		// �����켣����
		AnimationTrackObject *m_object;
	};

};