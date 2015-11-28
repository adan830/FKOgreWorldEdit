#include "StdAfx.h"
#include "ObjectCloneAction.h"
#include "ObjectEditorPlugin.h"
#include "ObjectSelectAction.h"
namespace Ogre
{
	ObjectCloneAction::ObjectCloneAction()
	{
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->addChildViewListener(this);
	}
	ObjectCloneAction::~ObjectCloneAction(void)
	{
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->removeChildViewListener(this);
	}

	// ���ӿ���Ϣ
	bool ObjectCloneAction::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		switch(message)
		{
		case WM_KEYUP:
			{
				if(wParam == VK_DELETE)
				{
				}
			}
			break;
		}
		return true;
	}

	// ����������󣬵�����falseʱ���ٱ�������Ķ���
	bool ObjectCloneAction::visit(ISceneObject *obj)
	{
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->deselectObject(obj);
		ISceneObject *clonedObject = obj->clone();
		clonedObject->translate(Vector3(0.5f , 0 , 0.5f));
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->selectObject(clonedObject);
		return true;
	}

	// ��¡���
	void ObjectCloneAction::cloneObject()
	{
		// �������б�ѡ��Ķ���
		IMapManager::SelectedObjectsIterator iter = ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getSelectedObjectsIterator();
		IMapManager::ListSceneObject listSceneObject;
		while(iter.hasMoreElements())
		{
			ISceneObject *obj = iter.getNext();
			listSceneObject.push_back(obj);
		}
		for(IMapManager::ListSceneObject::iterator iter = listSceneObject.begin() ; iter != listSceneObject.end() ; iter ++)
		{
			visit(*iter);
		}
	}

};