#include "StdAfx.h"
#include "ObjectSelectAction.h"

#include "ObjectEditorPlugin.h"
#include "ETSelection.h"
#include "ETTerrain.h"

namespace Ogre
{
	ObjectSelectAction::ObjectSelectAction()

	{
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->addChildViewListener(this);

		

		mSceneMgr = ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getScene()->getOgreSceneManager();

		// ������Ⱦ����
		mRect = new SelectionRectangle("Selection SelectionRectangle");
		mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(mRect);
		mRect->setVisible(false);

		// ���������ѯ
		mVolQuery = mSceneMgr->createPlaneBoundedVolumeQuery(PlaneBoundedVolumeList());
		// �������߲�ѯ
		mRayQuery = mSceneMgr->createRayQuery(Ray());


	}
	ObjectSelectAction::~ObjectSelectAction(void)
	{
		mSceneMgr->destroyQuery(mRayQuery);
		mSceneMgr->destroyQuery(mVolQuery);
		OGRE_DELETE(mRect);
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->removeChildViewListener(this);
	}

	// ���ӿ���Ϣ
	bool ObjectSelectAction::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		switch(message)
		{
		case WM_LBUTTONDOWN:
			{
				SHORT xPos = GET_X_LPARAM(lParam); 
				SHORT yPos = GET_Y_LPARAM(lParam); 
				CRect rect;
				view->getWnd()->GetClientRect(&rect);
				mStart.x = (float)xPos / (float)rect.Width();
				mStart.y = (float)yPos / (float)rect.Height();
				mStop = mStart;
				mRect->setCorners(mStart , mStop);
				mRect->setVisible(true);
			}
			break;
		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_MOUSEMOVE:
			{
				SHORT xPos = GET_X_LPARAM(lParam); 
				SHORT yPos = GET_Y_LPARAM(lParam); 
				CRect rect;
				view->getWnd()->GetClientRect(&rect);
				mStop.x = (float)xPos / (float)rect.Width();
				mStop.y = (float)yPos / (float)rect.Height();
				mRect->setCorners(mStart , mStop);
				// ������ѡ������
				if(message == WM_LBUTTONUP)
				{
					mRect->setVisible(false);
					
					Vector2 moved(mStop - mStart);
					if(std::min(abs(moved.x) , abs(moved.y)) < 0.01f)
					{
						raySelection(view->getCamera() , mStart , mStop , 0);
					}
					else
					{
						volSelection(view->getCamera() , mStart , mStop);
					}
				}
				// ˫���Ͳ����Ƿ��ǹ켣���壬����ǹ켣���壬�ͼ���켣�༭���������ֱ��ѡ������
				else if(message == WM_LBUTTONDBLCLK)
				{
					mRect->setVisible(false);
					Vector2 moved(mStop - mStart);
					if(std::min(abs(moved.x) , abs(moved.y)) < 0.01f)
					{
						ISceneObject *obj = 0;
						raySelection(view->getCamera() , mStart , mStop , &obj);
						if(obj)
						{
							// �������Ѱ����һ�������켣�����򼤻�༭����켣
							if(obj->getType() == AnimationTrackObjectFactory::FACTORY_TYPE)
							{
								ObjectEditorPlugin::getSingleton().getDlgTrackTools()->activate(static_cast<AnimationTrackObject*>(obj));
							}
							else
							{
								ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->selectObject(obj);
							}
						}
					}
				}
			}
			break;
		}
		return true;
	}

	// ִ�����߲�ѯ
	void ObjectSelectAction::raySelection(Camera *camera , const Vector2 &first, const Vector2 &second , ISceneObject **selectObject)
	{
		if(selectObject)
		{
			*selectObject = 0;
		}
		// �ȷ�ѡȫ��
		deselectAllObject();
		Ray ray = camera->getCameraToViewportRay(first.x, first.y);
		ListRayCheckInfo listInfo;
		// �����и���������ཻ�Ķ���ŵ�listInfo��
		if(ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getScene()->collide(ray , false , true , false , listInfo))
		{
			// �����ж���ִ�о�ȷ��ѯ
			for(ListRayCheckInfo::iterator iter = listInfo.begin() ; iter != listInfo.end() ; iter ++)
			{
				if(iter->obj->getSceneObjectVisible()->getCollisionEntity())
				{
					RayCollisionInfo objInfo;
					// ��ȷ��ѯ�������������ײ���ͱ����µ���ײ��Ϣ�������ɾ��֮
					if(iter->obj->getSceneObjectVisible()->getCollisionEntity()->collide(ray , objInfo))
					{
						iter->info = objInfo;
					}
					else
					{
						iter->info.distance = FLT_MAX;
					}
				}
			}
			// ���°����µ�λ������
			std::sort(listInfo.begin() , listInfo.end());

			// ѡ��֮
			if(!listInfo.empty())
			{
				if(listInfo.front().info.distance != FLT_MAX)
				{
					if(selectObject)
					{
						*selectObject = listInfo.front().obj;
					}
					else
					{
						ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->selectObject(listInfo.front().obj);
					}
				}
			}
		}
		else
		{
			onNoSelect();
		}
	}
	// ִ�������ѯ
	void ObjectSelectAction::volSelection(Camera *camera , const Vector2 &first, const Vector2 &second)
	{
		float left = first.x, right = second.x,
			top = first.y, bottom = second.y;

		if (left > right)
			std::swap(left, right);

		if (top > bottom)
			std::swap(top, bottom);

		if ((right - left) * (bottom - top) < 0.0001)
		{
			onNoSelect();
			return;
		}

		// Convert screen positions into rays
		Ray topLeft = camera->getCameraToViewportRay(left, top);
		Ray topRight = camera->getCameraToViewportRay(right, top);
		Ray bottomLeft = camera->getCameraToViewportRay(left, bottom);
		Ray bottomRight = camera->getCameraToViewportRay(right, bottom);

		// The plane faces the counter clockwise position.
		PlaneBoundedVolume vol;
		vol.planes.push_back(Plane(topLeft.getPoint(3), topRight.getPoint(3), bottomRight.getPoint(3)));         // front plane
		vol.planes.push_back(Plane(topLeft.getOrigin(), topLeft.getPoint(100), topRight.getPoint(100)));         // top plane
		vol.planes.push_back(Plane(topLeft.getOrigin(), bottomLeft.getPoint(100), topLeft.getPoint(100)));       // left plane
		vol.planes.push_back(Plane(topLeft.getOrigin(), bottomRight.getPoint(100), bottomLeft.getPoint(100)));   // bottom plane
		vol.planes.push_back(Plane(topLeft.getOrigin(), topRight.getPoint(100), bottomRight.getPoint(100)));     // right plane

		PlaneBoundedVolumeList volList;
		volList.push_back(vol);

		mVolQuery->setVolumes(volList);
		SceneQueryResult result = mVolQuery->execute();
		SceneQueryResultMovableList::iterator itr;
		// �ȷ�ѡȫ��
		deselectAllObject();

		IMapManager::ListSceneObject listSceneObject;
		for (itr = result.movables.begin(); itr != result.movables.end(); ++itr)
		{
			selectObject(*itr , &listSceneObject);
		}
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->selectObject(listSceneObject);
		if(listSceneObject.empty())
		{
			onNoSelect();
		}
	}

	// ѡ��һ��MO
	bool ObjectSelectAction::selectObject(MovableObject *obj , IMapManager::ListSceneObject *listSceneObject)
	{
		ISceneObject *sceneObject = ObjectEditorPlugin::getSingleton().getEditableSceneObject(obj);
		if(sceneObject)
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
		return false;
	}
	// ��ѡ��֮ǰ�ȷ�ѡ�������
	void ObjectSelectAction::deselectAllObject()
	{
		// �ȷ�ѡȫ��
		ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->deselectAllObject();
	}

};