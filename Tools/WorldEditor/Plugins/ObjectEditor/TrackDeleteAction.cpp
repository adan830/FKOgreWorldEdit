#include "StdAfx.h"
#include "TrackDeleteAction.h"
#include "ObjectEditorPlugin.h"

namespace Ogre
{

	TrackDeleteAction::TrackDeleteAction(AnimationTrackObject *obj)
		: m_object(obj)
	{
	}

	TrackDeleteAction::~TrackDeleteAction(void)
	{
	}
	// ����������󣬵�����falseʱ���ٱ�������Ķ���
	bool TrackDeleteAction::visit(ISceneObject *obj)
	{
		// ֻ�б����Ŷ���ָ��Ĳſ���ѡ��
		KeyObjectData data = safe_cast<KeyObjectData>(obj->getUserAny());
		if(!data.isNull())
		{
			ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->deselectObject(obj);
			data.m_dlgTrackTools->deleteKeyFrame(data);
		}
		return true;
	}


	// ɾ�����
	void TrackDeleteAction::deleteObject()
	{
		ObjectDeleteAction::deleteObject();
		// ѡ��켣��������
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->selectObject(m_object);
	}

};