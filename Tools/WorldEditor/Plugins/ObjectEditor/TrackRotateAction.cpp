#include "StdAfx.h"
#include "TrackRotateAction.h"
#include "ObjectEditorPlugin.h"

namespace Ogre
{

	TrackRotateAction::TrackRotateAction(AnimationTrackObject *obj)
		: m_object(obj)
	{
	}

	TrackRotateAction::~TrackRotateAction(void)
	{
	}
	// ����������󣬵�����falseʱ���ٱ�������Ķ���
	bool TrackRotateAction::visit(ISceneObject *obj)
	{
		// ֻ�б����Ŷ���ָ��Ĳſ���ѡ��
		KeyObjectData data = safe_cast<KeyObjectData>(obj->getUserAny());
		if(!data.isNull())
		{
			obj->rotate(m_rotate);
		}
		return true;
	}

	// �������ѡ�������ƽ�����ĵ�
	bool TrackRotateAction::getSelectedObjectsAvgCenter(Vector3 &center)const
	{
		return ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getSelectedObjectsAvgCenter(center , m_object);
	}
};