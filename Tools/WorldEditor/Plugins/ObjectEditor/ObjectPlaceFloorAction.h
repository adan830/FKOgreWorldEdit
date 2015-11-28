#pragma once
#include "ObjectAction.h"
namespace Ogre
{
	// ������ز���
	class ObjectPlaceFloorAction : public ObjectAction
		, public IChildView::Listener
		, public IMapManager::ObjectVisitor
	{
	public:
		ObjectPlaceFloorAction();
		virtual ~ObjectPlaceFloorAction(void);
		// ���ӿ���Ϣ
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
		// ����������󣬵�����falseʱ���ٱ�������Ķ���
		virtual bool visit(ISceneObject *obj);

		// ִ���������
		virtual void execute(){placeObjectToFloor();};
		// �������
		void placeObjectToFloor();
	};
};