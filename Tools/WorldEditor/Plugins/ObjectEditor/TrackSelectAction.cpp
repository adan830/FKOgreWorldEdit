#include "StdAfx.h"
#include "TrackSelectAction.h"
#include "ObjectEditorPlugin.h"
#include "DlgTrackTools.h"

namespace Ogre
{
	TrackSelectAction::TrackSelectAction(AnimationTrackObject *obj)
		: m_object(obj)
	{
	}

	TrackSelectAction::~TrackSelectAction(void)
	{
	}
	// ѡ��һ��MO
	bool TrackSelectAction::selectObject(MovableObject *obj , IMapManager::ListSceneObject *listSceneObject)
	{
		ISceneObject *sceneObject = obj_cast(obj->getUserAny());
		if(sceneObject)
		{
			// ֻ�б����Ŷ���ָ��Ĳſ���ѡ��
			KeyObjectData data = safe_cast<KeyObjectData>(sceneObject->getUserAny());
			if(!data.isNull())
			{
				if(!listSceneObject)
				{
					ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->selectObject(sceneObject);
				}
				else
				{
					listSceneObject->push_back(sceneObject);
				}
				return true;
			}
		}
		return false;
	}

	// ��ѡ��֮ǰ�ȷ�ѡ�������
	void TrackSelectAction::deselectAllObject()
	{
		// �������б�ѡ��Ķ���
		IMapManager::SelectedObjectsIterator iter = ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getSelectedObjectsIterator();
		IMapManager::ListSceneObject listSceneObject;
		while(iter.hasMoreElements())
		{
			ISceneObject *obj = iter.getNext();
			KeyObjectData data = safe_cast<KeyObjectData>(obj->getUserAny());
			if(!data.isNull())
			{
				listSceneObject.push_back(obj);
			}
		}
		// ��ѡȫ��
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->deselectObject(listSceneObject);
	}

	// û��ѡ��
	void TrackSelectAction::onNoSelect()
	{
		// ��ѡ
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->deselectObject(m_object);
		// ѡ��
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->selectObject(m_object);

	}

};