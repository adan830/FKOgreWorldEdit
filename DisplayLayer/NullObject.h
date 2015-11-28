#pragma once
#include "ISceneObject.h"
#include "PropertyDef.h"
namespace Ogre
{
	// ��������Ķ��󹤳�
	class DISPLAYLAYER_API NullObjectFactory : public ISceneObjectFactory
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
	class DISPLAYLAYER_API NullObject : public ISceneObject
	{
	public:
		NullObject(const String &name);
		virtual ~NullObject(void);

		// ��������ʵ��
		virtual void create(IScene *scene , const NameValuePairList* params);
		// ���ٶ���ʵ��
		virtual void destroy();

		// ��ô������ĳ���
		virtual IScene *getScene()const;

		// �����Ogre�еĶ�������
		virtual String getFullName()const;

		// ����������
		virtual bool setName(const String &name);
		// ��ö�������
		virtual const String &getName()const;
		// ��ö�������
		virtual const String &getType()const
		{
			return NullObjectFactory::FACTORY_TYPE;
		}

		// �����Ƿ���ʾ
		virtual void setVisible(bool visible);
		// �Ƿ���ʾ
		virtual bool isVisible()const;


		// �����������
		virtual void setPosition(const Vector3 &pos);
		// ����������
		virtual const Vector3 &getPosition()const;
		// ������������
		virtual void setWorldPosition(const Vector3 &pos);
		// �����������
		virtual const Vector3 &getWorldPosition()const;

		// �����������
		virtual void setScale(const Vector3 &scale);
		// ����������
		virtual const Vector3 &getScale()const;
		// ������������
		virtual void setWorldScale(const Vector3 &scale);
		// �����������
		virtual const Vector3 &getWorldScale()const;

		// ������Է�λ
		virtual void setOrientation(const Quaternion &orientation);
		// ����������
		virtual const Quaternion &getOrientation()const;
		// �������緽λ
		virtual void setWorldOrientation(const Quaternion &orientation);
		// ������緽λ
		virtual const Quaternion &getWorldOrientation()const;

		// ���ø�����
		virtual void setParent(ISceneObject *parent);
		// ��ø�����
		virtual ISceneObject* getParent()const;

		// ����ĳ���������꣬������������ڲ��������緽λ
		virtual void lookAt(const Vector3& targetPoint);
		// ���ݵ�ǰ����ֵ����
		virtual void scale(const Vector3& scale);
		// ���ݵ�ǰλ���ƶ�
		virtual void translate(const Vector3& distance);
		// ���ݵ�ǰ��λ��ת
		virtual void rotate(const Quaternion& rotate);

		// ��һ����������ҽӵ�����
		virtual void addChildObject(ISceneObject *obj);
		// ��һ����������ӵ�����ȡ���ҽ�
		virtual void removeChildObject(ISceneObject *obj);
		// �������������Ӹ���������ȡ���ҽ�
		virtual void removeFromParentObject();

		// ����Ѹ��ӵĳ������ö��
		virtual ChildObjectIterator getChildObjectIterator();

		// �������ͺ����ƴ���һ���˳���������ӳ�������
		virtual ISceneObject *createChildObject(const String &name , const String &type, const NameValuePairList* params = 0);
		// �������ͺ����ƴ���һ���˳���������ӳ�������
		virtual ISceneObject *createChildObject(const String &type, const NameValuePairList* params = 0);
		// ����һ���˳���������ӳ�������
		virtual void destroyChildObject(const String &name);
		virtual void destroyChildObject(ISceneObject *obj);
		// ���һ���˳���������ӳ�������
		virtual ISceneObject *getChildObject(const String &name);
		// �Ƿ����ָ�����ֵĴ˳���������ӳ�������
		virtual bool hasChildObject(const String &name)const;

		// ���Ogre�ĳ����ڵ�
		virtual SceneNode* getOgreSceneNode()const
		{
			return m_node;
		}

		// ���MovableObject
		virtual MovableObject *getOgreMovableObject()const
		{
			return 0;
		}

		// ��ö��󴴽�����
		virtual const NameValuePairList& getCreationParams()const
		{
			return m_creationParams;
		}

		// ��newName��Ϊ���ƿ�¡һ���µĶ���
		virtual ISceneObject *clone(const String &newName , IScene *newScene = 0);
		// ���Զ����ɵ�������Ϊ���ƿ�¡һ���µĶ���
		virtual ISceneObject *clone(IScene *newScene = 0);
		// �ѵ�ǰ�����е����ݿ�¡��target��
		virtual void cloneData(ISceneObject *target);

		// ��������
		virtual bool setProperty(uint id , const Any &value);
		// ��ȡ����ֵ
		virtual Any getProperty(uint id)const;
		// ����������ԣ���������ʽ
		virtual const VTProperty* getPropertiesTree()const;
		// ����������ԣ����б����ʽ
		virtual const MapProperty* getPropertiesList()const;

		// �����ײʵ��
		virtual ICollisionEntity *getCollisionEntity()const
		{
			return 0;
		}

		// �����Ƿ�μ���ײ
		virtual void setCanCollide(bool can);
		// �����Ƿ�μ���ײ
		virtual bool getCanCollide()const;


		// �����û�����
		virtual void setUserAny(const Any& anything)
		{
			m_userAny = anything;
		}
		// ����û�����
		virtual const Any& getUserAny() const
		{
			return m_userAny;
		}
		// ��ó�������Ŀɼ����������ڱ༭����ѡ��
		virtual ISceneObjectVisible *getSceneObjectVisible();
	protected:
		// ö���������ԣ���������ʽ
		virtual void enumPropertiesTree(VTProperty &properties)const;

		// �û�����
		Any m_userAny;
		// ʵ������
		String m_name;
		// Ogre�ĳ����ڵ�
		SceneNode *m_node;
		// �Ƿ���Ҫ�Զ�ɾ��Ogre�ڵ�
		bool m_autoDeleteNode;
		// �����˶���ĳ���
		IScene *m_scene;
		// ���ڵ�
		ISceneObject *m_parent;
		// �ɼ�����
		ISceneObjectVisible *m_sceneObjectVisible;
		// �Ƿ����
		bool m_visible;
		// �Ƿ�μ���ײ����
		bool m_canCollide;
		// �������������������Ƕ���ʵ������������Ҫ�Ĳ������浽�����mesh����֮��
		NameValuePairList m_creationParams;
		// �Ӷ���
		MapChildObject m_mapChildObject;
	};


};