#include "StdAfx.h"
#include "TrackNewAction.h"
#include "ObjectEditorPlugin.h"

namespace Ogre
{
	TrackNewAction::TrackNewAction(DlgTrackTools *tools)
		: m_dlgTrackTools(tools)
	{
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->addChildViewListener(this , false);
	}
	TrackNewAction::~TrackNewAction(void)
	{
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->removeChildViewListener(this);
	}

	// ���ӿ���Ϣ
	bool TrackNewAction::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		switch(message)
		{
		case WM_KEYUP:
			{
				if(wParam == VK_DELETE)
				{
					m_dlgTrackTools->popBackKeyFrame();
					return false;
				} 
			}
			break;
		case WM_LBUTTONUP:
			{
				short xPos = GET_X_LPARAM(lParam); 
				short yPos = GET_Y_LPARAM(lParam); 
				CRect rect;
				::GetClientRect(view->getWnd()->GetSafeHwnd() , &rect);
				Vector3 pos;
				Ray ray = view->getCamera()->getCameraToViewportRay(xPos / (float)rect.Width()
					, yPos / (float)rect.Height());

				Vector3 posIntersect;
				// ������������
				if(ObjectEditorPlugin::getSingleton().collide(ray , true , false , posIntersect))
				{
					AnimationTrackObject::KeyFrame keyFrame;
					keyFrame.m_translate = posIntersect;
					keyFrame.m_length = 0.0f;
					// ����йؼ�֡���ͼ����µķ����ʱ��
					AnimationTrackObject *aniObject = m_dlgTrackTools->getAnimationTrackObject();
					size_t count = aniObject->getKeyFrameCount();
					if(count > 0)
					{
						size_t lastIndex = count - 1;
						AnimationTrackObject::KeyFrame lastKeyFrame = aniObject->getKeyFrame(lastIndex);
						Vector3 dir = keyFrame.m_translate - lastKeyFrame.m_translate;
						// �������
						float distance = dir.normalise();
						// ����ת��ķ���
						lastKeyFrame.m_rotate = MathEx::DirectionToQuaternion(dir);
						// �ս����Ķ����ٶȳ�ʼΪ10��/��
						keyFrame.m_length = distance / 10.0f;
						// �޸����һ���ڵ�ķ���
						aniObject->modifyKeyFrame(lastIndex , lastKeyFrame.m_length , lastKeyFrame.m_translate , lastKeyFrame.m_scale , lastKeyFrame.m_rotate);
					}
					// ��β������һ���ؼ�֡
					KeyObjectData newData = m_dlgTrackTools->pushBackKeyFrame(keyFrame);
					deselectAllObject();
					ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->selectObject(newData.m_keyObject);

				}
			}
			break;
		}
		return true;
	}


	// ��ѡ��֮ǰ�ȷ�ѡ�������
	void TrackNewAction::deselectAllObject()
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

};