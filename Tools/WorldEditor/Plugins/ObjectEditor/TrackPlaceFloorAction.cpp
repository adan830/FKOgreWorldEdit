#include "StdAfx.h"
#include "TrackPlaceFloorAction.h"
#include "ObjectEditorPlugin.h"

namespace Ogre
{

	TrackPlaceFloorAction::TrackPlaceFloorAction(AnimationTrackObject *obj)
		: m_object(obj)
	{
	}

	TrackPlaceFloorAction::~TrackPlaceFloorAction(void)
	{
	}

	// ����������󣬵�����falseʱ���ٱ�������Ķ���
	bool TrackPlaceFloorAction::visit(ISceneObject *obj)
	{
		// ֻ�б����Ŷ���ָ��Ĳſ���ѡ��
		KeyObjectData data = safe_cast<KeyObjectData>(obj->getUserAny());
		if(!data.isNull())
		{
			Vector3 pos = obj->getWorldPosition();
			pos.y = 9999;
			Vector3 intersection;
			Ray ray(pos , Vector3::NEGATIVE_UNIT_Y);
			if(obj->getScene()->collide(ray , true , false , intersection))
			{
				obj->setWorldPosition(intersection);
			}
		}
		return true;
	}


};