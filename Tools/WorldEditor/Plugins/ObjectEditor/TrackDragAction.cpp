#include "StdAfx.h"
#include "TrackDragAction.h"
#include "ObjectEditorPlugin.h"

namespace Ogre
{

	TrackDragAction::TrackDragAction(AnimationTrackObject *obj)
		: m_object(obj)
	{
	}

	TrackDragAction::~TrackDragAction(void)
	{
	}
	// ����������󣬵�����falseʱ���ٱ�������Ķ���
	bool TrackDragAction::visit(ISceneObject *obj)
	{
		// ֻ�б����Ŷ���ָ��Ĳſ���ѡ��
		KeyObjectData data = safe_cast<KeyObjectData>(obj->getUserAny());
		if(!data.isNull())
		{
			obj->translate(m_translate);
		}
		return true;
	}

	// �������ѡ�������ƽ�����ĵ�
	bool TrackDragAction::getSelectedObjectsAvgCenter(Vector3 &center)const
	{
		return ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getSelectedObjectsAvgCenter(center , m_object);
	}
};