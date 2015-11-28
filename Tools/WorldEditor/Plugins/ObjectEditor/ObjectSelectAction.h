#pragma once
#include "ObjectAction.h"
#include "SelectionRectangle.h"
namespace Ogre
{
	// ѡ���������
	class ObjectSelectAction : public ObjectAction
		, public IChildView::Listener
	{
	public:
		ObjectSelectAction();
		virtual ~ObjectSelectAction(void);
		// ���ӿ���Ϣ
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	protected:
		// ִ�������ѯ
		virtual void volSelection(Camera *camera , const Vector2 &first, const Vector2 &second);
		// ִ�����߲�ѯ
		virtual void raySelection(Camera *camera , const Vector2 &first, const Vector2 &second , ISceneObject **selectObject);
		// ѡ��һ��MO
		virtual bool selectObject(MovableObject *obj , IMapManager::ListSceneObject *listSceneObject);

		// ��ѡ��֮ǰ�ȷ�ѡ�������
		virtual void deselectAllObject();

		// û��ѡ��
		virtual void onNoSelect(){};

		Vector2 mStart;
		Vector2 mStop;
		SceneManager *mSceneMgr;
		PlaneBoundedVolumeListSceneQuery *mVolQuery;
		RaySceneQuery *mRayQuery;
		SelectionRectangle *mRect;
	};
};