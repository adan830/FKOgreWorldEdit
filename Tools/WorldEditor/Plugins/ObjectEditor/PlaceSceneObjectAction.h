#pragma once
#include "ObjectAction.h"
namespace Ogre
{
	// ����ģ��ʵ��
	class PlaceSceneObjectAction : public ObjectAction
		, public IChildView::Listener
	{
	public:
		PlaceSceneObjectAction(ISceneObject *obj);
		virtual ~PlaceSceneObjectAction(void);
		// ���ӿ���Ϣ
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	protected:
		// �������Ķ���
		ISceneObject *m_object;
	};
};