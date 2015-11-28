#include "StdAfx.h"
#include "TrackMoveAction.h"
#include "ObjectEditorPlugin.h"

namespace Ogre
{

	TrackMoveAction::TrackMoveAction(AnimationTrackObject *obj)
		: m_object(obj)
	{
	}

	TrackMoveAction::~TrackMoveAction(void)
	{
	}
	// ����������󣬵�����falseʱ���ٱ�������Ķ���
	bool TrackMoveAction::visit(ISceneObject *obj)
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
	bool TrackMoveAction::getSelectedObjectsAvgCenter(Vector3 &center)const
	{
		return ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getSelectedObjectsAvgCenter(center , m_object);
	}
};