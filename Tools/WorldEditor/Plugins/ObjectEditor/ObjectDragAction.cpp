#include "StdAfx.h"
#include "ObjectDragAction.h"

#include "ObjectEditorPlugin.h"
#include "ETSelection.h"
#include "ETTerrain.h"

namespace Ogre
{
	ObjectDragAction::ObjectDragAction()

	{
		// ������������۲���Ϊ�Լ�
		setManipulatorObserver(this);

		DragManipulator::init(ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getScene() , "ObjectDragAction");
		activate();


		
	}
	ObjectDragAction::~ObjectDragAction(void)
	{
	}

	// ����������󣬵�����falseʱ���ٱ�������Ķ���
	bool ObjectDragAction::visit(ISceneObject *obj)
	{
		obj->translate(m_translate);
		return true;
	}


	void ObjectDragAction::handleManipulatorMove(const Vector3& worldPosition)
	{
		Vector3 center;
		if(!getSelectedObjectsAvgCenter(center))
			return;
		m_translate = worldPosition - center;
		// �������б�ѡ��Ķ���
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->visitSelectedObject(this);
	}

	/** This is called when the manipulator begins manipulating an object
	*/
	void ObjectDragAction::handleManipulatorStartManipulating()
	{
	}

	/** 
	*/
	void ObjectDragAction::handleManipulatorStopManipulating() 
	{
	}

	bool ObjectDragAction::manipulatorRayCollide(const Ogre::Ray& ray, Vector3& intersection)
	{
		return ObjectEditorPlugin::getSingleton().collide(ray , true , false , intersection);
	};


	// ׼�����´���
	bool ObjectDragAction::preViewportUpdate(IChildView *view , const RenderTargetViewportEvent& evt)
	{
		IMapManager *map = ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager();
		IMapManager::SelectedObjectsIterator iter = map->getSelectedObjectsIterator();
		if(iter.hasMoreElements())
		{
			ISceneObject *obj = iter.getNext();
			Vector3 center;
			if(!getSelectedObjectsAvgCenter(center))
				return true;
			update(view, center, Quaternion::IDENTITY);
		}
		return true;
	}


	// �������ѡ�������ƽ�����ĵ�
	bool ObjectDragAction::getSelectedObjectsAvgCenter(Vector3 &center)const
	{
		return ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getSelectedObjectsAvgCenter(center);
	}
};