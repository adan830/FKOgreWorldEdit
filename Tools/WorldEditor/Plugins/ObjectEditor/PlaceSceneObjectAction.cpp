#include "StdAfx.h"
#include "PlaceSceneObjectAction.h"
#include "ISceneObjectVisible.h"

#include "ObjectEditorPlugin.h"
#include "ETSelection.h"
#include "ETTerrain.h"

namespace Ogre
{
	PlaceSceneObjectAction::PlaceSceneObjectAction(ISceneObject *obj)
		: m_object(obj)
	{
		// ���ص�������ʵ�壬��ʾ�ڷ�ʱ��ʵ��
		m_object->getSceneObjectVisible()->showEntity(false);
		m_object->getSceneObjectVisible()->showPlacingEntity(true);
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->addChildViewListener(this);
	}
	PlaceSceneObjectAction::~PlaceSceneObjectAction(void)
	{
		m_object->getScene()->destroySceneObject(m_object);
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->removeChildViewListener(this);
	}

	// ���ӿ���Ϣ
	bool PlaceSceneObjectAction::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		switch(message)
		{
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
			{
				short xPos = GET_X_LPARAM(lParam); 
				short yPos = GET_Y_LPARAM(lParam); 
				CRect rect;
				::GetClientRect(view->getWnd()->GetSafeHwnd() , &rect);
				Vector3 pos;
				Ray ray = view->getCamera()->getCameraToViewportRay(xPos / (float)rect.Width()
					, yPos / (float)rect.Height());

				Vector3 posIntersect;
				if(ObjectEditorPlugin::getSingleton().collide(ray , true , false , posIntersect))
				{
					m_object->setWorldPosition(posIntersect);
					// �������ģ��
					if(message == WM_LBUTTONDOWN)
					{
						// ��ʾ��������ʵ��
						m_object->getSceneObjectVisible()->showPlacingEntity(false);
						m_object->getSceneObjectVisible()->showEntity(true);
						// ��¡֮
						ISceneObject *obj = m_object->clone(ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getScene());
						// ��¡��Ķ�����ʾ������ʵ��
						obj->getSceneObjectVisible()->showPlacingEntity(false);
						obj->getSceneObjectVisible()->showEntity(true);
						//// ���ص�������ʵ�壬��ʾ�ڷ�ʱ��ʵ��
						m_object->getSceneObjectVisible()->showEntity(false);
						m_object->getSceneObjectVisible()->showPlacingEntity(true);
						ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->deselectAllObject();
						ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->selectObject(obj);
						// �����һ���켣�����������̽���༭ģʽ
						if(obj->getType() == AnimationTrackObjectFactory::FACTORY_TYPE)
						{
							ObjectEditorPlugin::getSingleton().getDlgTrackTools()->activate(static_cast<AnimationTrackObject*>(obj));
						}
					}
				}
			}
			break;
		}
		return true;
	}
}; 
