#pragma once
#include "ObjectAction.h"
#include "TranslationManipulator.h"
namespace Ogre
{
	// ����ƶ�����
	class ObjectMoveAction : public ObjectAction
		, public TranslationManipulator
		, public IMapManager::ObjectVisitor
		, public ManipulatorObserver
	{
	public:
		ObjectMoveAction();
		virtual ~ObjectMoveAction(void);
		// ����������󣬵�����falseʱ���ٱ�������Ķ���
		virtual bool visit(ISceneObject *obj);

		virtual void handleManipulatorTranslate(const Vector3& translation);

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
		Vector3 m_translate;

		// �������ѡ�������ƽ�����ĵ�
		virtual bool getSelectedObjectsAvgCenter(Vector3 &center)const;

	};
};