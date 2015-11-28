#include "StdAfx.h"
#include "ObjectDeleteAction.h"
#include "ObjectEditorPlugin.h"

namespace Ogre
{
	ObjectDeleteAction::ObjectDeleteAction()
	{
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->addChildViewListener(this);
	}
	ObjectDeleteAction::~ObjectDeleteAction(void)
	{
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->removeChildViewListener(this);
	}

	// ���ӿ���Ϣ
	bool ObjectDeleteAction::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
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
	bool ObjectDeleteAction::visit(ISceneObject *obj)
	{
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->deselectObject(obj);
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getScene()->destroySceneObject(obj);
		return true;
	}

	// ɾ�����
	void ObjectDeleteAction::deleteObject()
	{
		// �������б�ѡ��Ķ���
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->visitSelectedObject(this);
	}

};