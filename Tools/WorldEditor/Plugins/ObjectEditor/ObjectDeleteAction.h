#pragma once
#include "ObjectAction.h"
namespace Ogre
{
	// ���ɾ������
	class ObjectDeleteAction : public ObjectAction
		, public IChildView::Listener
		, public IMapManager::ObjectVisitor
	{
	public:
		ObjectDeleteAction();
		virtual ~ObjectDeleteAction(void);
		// ���ӿ���Ϣ
		virtual bool onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
		// ����������󣬵�����falseʱ���ٱ�������Ķ���
		virtual bool visit(ISceneObject *obj);

		// ִ���������
		virtual void execute(){deleteObject();};

		// ɾ�����
		virtual void deleteObject();
	};
};