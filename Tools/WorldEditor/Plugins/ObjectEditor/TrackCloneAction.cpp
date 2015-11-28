#include "StdAfx.h"
#include "TrackCloneAction.h"
#include "ObjectEditorPlugin.h"

namespace Ogre
{

	TrackCloneAction::TrackCloneAction(AnimationTrackObject *obj)
		: m_object(obj)
	{
	}

	TrackCloneAction::~TrackCloneAction(void)
	{
	}
	// ����������󣬵�����falseʱ���ٱ�������Ķ���
	bool TrackCloneAction::visit(ISceneObject *obj)
	{
		// ֻ�б����Ŷ���ָ��Ĳſ���ѡ��
		KeyObjectData data = safe_cast<KeyObjectData>(obj->getUserAny());
		if(!data.isNull())
		{
			ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->deselectObject(obj);
			KeyObjectData newData = data.m_dlgTrackTools->cloneKeyFrame(data);
			ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->selectObject(newData.m_keyObject);
		}
		return true;
	}
};