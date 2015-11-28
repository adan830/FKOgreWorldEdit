#include "StdAfx.h"
#include "ObjectRotateAction.h"

#include "ObjectEditorPlugin.h"
#include "ETSelection.h"
#include "ETTerrain.h"

namespace Ogre
{
	ObjectRotateAction::ObjectRotateAction()

	{
		// ������������۲���Ϊ�Լ�
		setManipulatorObserver(this);

		RotationManipulator::init(ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getScene() , "ObjectRotateAction");
		activate();

		
	}
	ObjectRotateAction::~ObjectRotateAction(void)
	{
	}

	// ����������󣬵�����falseʱ���ٱ�������Ķ���
	bool ObjectRotateAction::visit(ISceneObject *obj)
	{
		obj->rotate(m_rotate);
		return true;
	}


	void ObjectRotateAction::handleManipulatorRotate(const Quaternion& rotation)
	{
		IMapManager *map = ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager();
		m_rotate = rotation;

		// �������б�ѡ��Ķ���
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->visitSelectedObject(this);
	}

	/** This is called when the manipulator begins manipulating an object
	*/
	void ObjectRotateAction::handleManipulatorStartManipulating()
	{

	}

	/** 
	*/
	void ObjectRotateAction::handleManipulatorStopManipulating() 
	{

	}


	bool ObjectRotateAction::manipulatorRayCollide(const Ogre::Ray& ray, Vector3& intersection)
	{
		return mScene->collide(ray , true , false , intersection);
	};


	// ׼�����´���
	bool ObjectRotateAction::preViewportUpdate(IChildView *view , const RenderTargetViewportEvent& evt)
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
	bool ObjectRotateAction::getSelectedObjectsAvgCenter(Vector3 &center)const
	{
		return ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getSelectedObjectsAvgCenter(center);
	}
};