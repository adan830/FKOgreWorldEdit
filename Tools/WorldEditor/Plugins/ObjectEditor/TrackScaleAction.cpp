#include "StdAfx.h"
#include "TrackScaleAction.h"
#include "ObjectEditorPlugin.h"

namespace Ogre
{

	TrackScaleAction::TrackScaleAction(AnimationTrackObject *obj)
		: m_object(obj)
	{
	}

	TrackScaleAction::~TrackScaleAction(void)
	{
	}
	// ����������󣬵�����falseʱ���ٱ�������Ķ���
	bool TrackScaleAction::visit(ISceneObject *obj)
	{
		// ֻ�б����Ŷ���ָ��Ĳſ���ѡ��
		KeyObjectData data = safe_cast<KeyObjectData>(obj->getUserAny());
		if(!data.isNull())
		{
			obj->scale(m_translate);
		}
		return true;
	}

	// �������ѡ�������ƽ�����ĵ�
	bool TrackScaleAction::getSelectedObjectsAvgCenter(Vector3 &center)const
	{
		return ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getSelectedObjectsAvgCenter(center , m_object);
	}
};