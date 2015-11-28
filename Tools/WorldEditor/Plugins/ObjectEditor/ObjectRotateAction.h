#pragma once
#include "ObjectAction.h"
#include "RotationManipulator.h"
namespace Ogre
{
	// ����ƶ�����
	class ObjectRotateAction : public ObjectAction
		, public RotationManipulator
		, public IMapManager::ObjectVisitor
		, public ManipulatorObserver
	{
	public:
		ObjectRotateAction();
		virtual ~ObjectRotateAction(void);
		// ����������󣬵�����falseʱ���ٱ�������Ķ���
		virtual bool visit(ISceneObject *obj);

		virtual void handleManipulatorRotate(const Quaternion& rotation);

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
		Quaternion m_rotate;

		// �������ѡ�������ƽ�����ĵ�
		virtual bool getSelectedObjectsAvgCenter(Vector3 &center)const;

	};
};