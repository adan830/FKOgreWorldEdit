#pragma once
#include "ObjectAction.h"
#include "DragManipulator.h"
namespace Ogre
{
	// ����ӵ�����ҷ����
	class ObjectDragAction : public ObjectAction
		, public DragManipulator
		, public IMapManager::ObjectVisitor
		, public ManipulatorObserver
	{
	public:
		ObjectDragAction();
		virtual ~ObjectDragAction(void);
		// ����������󣬵�����falseʱ���ٱ�������Ķ���
		virtual bool visit(ISceneObject *obj);


		virtual void handleManipulatorMove(const Vector3& worldPosition);

		/** This is called when the manipulator begins manipulating an object
		*/
		virtual void handleManipulatorStartManipulating();

		/** 
		*/
		virtual void handleManipulatorStopManipulating();

		virtual bool manipulatorRayCollide(const Ogre::Ray& ray, Vector3& intersection);

		// ׼�����´���
		virtual bool preViewportUpdate(IChildView *view , const RenderTargetViewportEvent& evt);

	protected:

		// �������ѡ�������ƽ�����ĵ�
		virtual bool getSelectedObjectsAvgCenter(Vector3 &center)const;

		// ��Ҫ�ƶ��ľ���
		Vector3 m_translate;
	};
};