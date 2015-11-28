#include "StdAfx.h"
#include "ObjectPlaceFloorAction.h"

#include "ObjectEditorPlugin.h"
#include "ETSelection.h"
#include "ETTerrain.h"

namespace Ogre
{
	ObjectPlaceFloorAction::ObjectPlaceFloorAction()

	{
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->addChildViewListener(this);

	}
	ObjectPlaceFloorAction::~ObjectPlaceFloorAction(void)
	{
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->removeChildViewListener(this);
	}

	// ���ӿ���Ϣ
	bool ObjectPlaceFloorAction::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
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
	bool ObjectPlaceFloorAction::visit(ISceneObject *obj)
	{
		Vector3 pos = obj->getWorldPosition();
		pos.y = 9999;
		Vector3 intersection;
		Ray ray(pos , Vector3::NEGATIVE_UNIT_Y);
		if(obj->getScene()->collide(ray , true , false , intersection))
		{
			obj->setWorldPosition(intersection);
		}
		return true;
	}

	// �������
	void ObjectPlaceFloorAction::placeObjectToFloor()
	{
		// �������б�ѡ��Ķ���
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->visitSelectedObject(this);
	}

};