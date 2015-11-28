#pragma once
#include "ObjectAction.h"
namespace Ogre
{
	// �����¡����
	class ObjectCloneAction : public ObjectAction
		, public IChildView::Listener
		, public IMapManager::ObjectVisitor
	{
	public:
		ObjectCloneAction();
		virtual ~ObjectCloneAction(void);
		// ���ӿ���Ϣ
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
		// ����������󣬵�����falseʱ���ٱ�������Ķ���
		virtual bool visit(ISceneObject *obj);

		// ִ���������
		virtual void execute(){cloneObject();};
		// ��¡���
		virtual void cloneObject();
	};
};