#pragma once
#include "NullObject.h"
#include "EntityAnimationController.h"
namespace Ogre
{
	class EntityMaterialInstance;
	// ��������Ķ��󹤳�
	class DISPLAYLAYER_API EntityObjectFactory : public ISceneObjectFactory
	{
	public:
		static const String FACTORY_TYPE;
		// ��øù�������������
		virtual const String &getType() const;
		// ��������Ĵ�������
		virtual ISceneObject *create(const String &name , IScene *scene , const NameValuePairList* params);
		// ������������ٷ���
		virtual void destroy(ISceneObject *obj);
	};

	// ģ��ʵ�����
	class DISPLAYLAYER_API EntityObject : public NullObject
		, public FrameListener
	{
	public:
		EntityObject(const String &name);
		virtual ~EntityObject(void);
		// ��ö�������
		virtual const String &getType()const
		{
			return EntityObjectFactory::FACTORY_TYPE;
		}
		Entity *getOgreEntity()const
		{
			return m_entity;
		}
		// ��������ʵ��
		virtual void create(IScene *scene , const NameValuePairList* params);
		// ���ٶ���ʵ��
		virtual void destroy();

		// ��ö���������
		IEntityAnimationController &getAnimationController()
		{
			return m_animationController;
		}
		// ֡��������ÿִ֡��
		virtual bool frameStarted( const FrameEvent& evt );

		// ����͸���ȣ�0����ȫ͸����1����͸��
		void setAlpha(float alpha);
		// ���͸����
		float getAlpha()const;

		// �����Ƿ���ʾ
		virtual void setVisible(bool visible);

		// ��һ����������ҽӵ���ͷ��
		virtual void attachObjectToBone(const String &boneName,
			ISceneObject *obj,
			const Quaternion &offsetOrientation = Quaternion::IDENTITY,
			const Vector3 &offsetPosition = Vector3::ZERO);
		// ��һ����������ӹ�ͷȡ���ҽ�
		virtual void detachObjectFromBone(ISceneObject *obj);
		virtual void detachObjectFromBone(const String &objName);
		// �����г�������ӹ�ͷȡ���ҽ�
		virtual void detachAllObjectsFromBone();

		typedef std::map<String, ISceneObject*> ChildObjectList;
		typedef MapIterator<ChildObjectList> ChildObjectListIterator;
		// ����ѹҽӵ���ͷ�ĳ������
		ChildObjectListIterator getAttachedObjectIterator(void);

		// ���MovableObject
		virtual MovableObject *getOgreMovableObject()const
		{
			return m_entity;
		}

		// ʹ�����Mesh���ع�Entity
		bool setMeshFile(const String &meshFileName);
		// ���Mesh�ļ���
		const String &getMeshFile()const;


		// ��������
		virtual bool setProperty(uint id , const Any &value);
		// ��ȡ����ֵ
		virtual Any getProperty(uint id)const;
		// ����������ԣ���������ʽ
		virtual const VTProperty* getPropertiesTree()const;
		// ����������ԣ����б����ʽ
		virtual const MapProperty* getPropertiesList()const;

		// ������ײģ������
		void setCollisionShapeType(CollisionShapeType type);
		// �����ײģ������
		CollisionShapeType getCollisionShapeType()const
		{
			return m_collisionEntity ? m_collisionEntity->getCollisionShapeType() : CT_NONE;
		}
		// ������ײģ��
		void setCollisionShapeName(const String &name);
		// �����ײģ������
		const String &getCollisionShapeName()const
		{
			return m_collisionEntity ? m_collisionEntity->getCollisionShapeName() : StringUtil::BLANK;
		}

		// �����ײʵ��
		ICollisionEntity *getCollisionEntity()const
		{
			return m_collisionEntity;
		}

		// ��ó�������Ŀɼ����������ڱ༭����ѡ��
		virtual ISceneObjectVisible *getSceneObjectVisible();
	protected:
		// ������ײģ��
		void loadCollisionEntity();
		// ж����ײģ��
		void unloadCollisionEntity();
		// ö���������ԣ���������ʽ
		virtual void enumPropertiesTree(VTProperty &properties)const;
		// ����mesh
		void destroyMesh();
		// Mesh�ļ���
		String m_meshFileName;
		// Ogreʵ��
		Entity *m_entity;
		// ʵ�嶯��������
		EntityAnimationController m_animationController;
		// ͸����
		float m_alpha;
		// ͸��������
		EntityMaterialInstance *m_alphaController;
		// �ѹҽӵ���ͷ�ĳ�������б�
		ChildObjectList m_childObjectList;

		// ��ײ������
		CollisionShapeType m_collisionType;
		// ��ײģ������
		String m_collisionName;

		// ��ײʵ��
		ICollisionEntity *m_collisionEntity;
	};


};