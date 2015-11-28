#pragma once
#include "PropertyContainer.h"

namespace Ogre
{
	class IScene;
	class ISceneObjectVisible;
	// �������
	class ISceneObject : public PropertyContainer
	{
	public:

		ISceneObject(void)
		{
		}

		virtual ~ISceneObject(void)
		{
		}

		// ��������ʵ��
		virtual void create(IScene *scene , const NameValuePairList* params) = 0;
		// ���ٶ���ʵ��
		virtual void destroy() = 0;

		// ��ô������ĳ���
		virtual IScene *getScene()const = 0;

		// ����������
		virtual bool setName(const String &name) = 0;
		// ��ö�������
		virtual const String &getName()const = 0;
		// ��ö�������
		virtual const String &getType()const = 0;

		// ��ö���ȫ�ƣ���������+�������ƣ�
		virtual String getFullName()const = 0;

		// �����Ƿ���ʾ
		virtual void setVisible(bool visible) = 0;
		// �Ƿ���ʾ
		virtual bool isVisible()const = 0;

		// �����������
		virtual void setPosition(const Vector3 &pos) = 0;
		// ����������
		virtual const Vector3 &getPosition()const = 0;
		// ������������
		virtual void setWorldPosition(const Vector3 &pos) = 0;
		// �����������
		virtual const Vector3 &getWorldPosition()const = 0;

		// �����������
		virtual void setScale(const Vector3 &scale) = 0;
		// ����������
		virtual const Vector3 &getScale()const = 0;
		// ������������
		virtual void setWorldScale(const Vector3 &scale) = 0;
		// �����������
		virtual const Vector3 &getWorldScale()const = 0;

		// ������Է�λ
		virtual void setOrientation(const Quaternion &orientation) = 0;
		// ����������
		virtual const Quaternion &getOrientation()const = 0;
		// �������緽λ
		virtual void setWorldOrientation(const Quaternion &orientation) = 0;
		// ������緽λ
		virtual const Quaternion &getWorldOrientation()const = 0;

		// ����ĳ���������꣬������������ڲ��������緽λ
		virtual void lookAt(const Vector3& targetPoint) = 0;
		// ���ݵ�ǰ����ֵ����
		virtual void scale(const Vector3& scale) = 0;
		// ���ݵ�ǰλ���ƶ�
		virtual void translate(const Vector3& distance) = 0;
		// ���ݵ�ǰ��λ��ת
		virtual void rotate(const Quaternion& rotate) = 0;

		// ��һ����������ҽӵ�����
		virtual void addChildObject(ISceneObject *obj) = 0;
		// ��һ����������ӵ�����ȡ���ҽ�
		virtual void removeChildObject(ISceneObject *obj) = 0;
		// �������������Ӹ���������ȡ���ҽ�
		virtual void removeFromParentObject() = 0;

		// ���ø�����
		virtual void setParent(ISceneObject *parent) = 0;
		// ��ø�����
		virtual ISceneObject* getParent()const = 0;

		typedef std::map<String , ISceneObject*> MapChildObject;
		typedef MapIterator<MapChildObject> ChildObjectIterator;
		// ����Ѹ��ӵĳ������ö��
		virtual ChildObjectIterator getChildObjectIterator() = 0;

		// �������ͺ����ƴ���һ���˳���������ӳ�������
		virtual ISceneObject *createChildObject(const String &name , const String &type, const NameValuePairList* params = 0) = 0;
		// �������ͺ����ƴ���һ���˳���������ӳ�������
		virtual ISceneObject *createChildObject(const String &type, const NameValuePairList* params = 0) = 0;
		// ����һ���˳���������ӳ�������
		virtual void destroyChildObject(const String &name) = 0;
		virtual void destroyChildObject(ISceneObject *obj) = 0;
		// ���һ���˳���������ӳ�������
		virtual ISceneObject *getChildObject(const String &name) = 0;
		// �Ƿ����ָ�����ֵĴ˳���������ӳ�������
		virtual bool hasChildObject(const String &name)const = 0;

		// ���MovableObject
		virtual MovableObject *getOgreMovableObject()const = 0;

		// ���SceneNode
		virtual SceneNode *getOgreSceneNode()const = 0;

		// ��ö��󴴽�����
		virtual const NameValuePairList& getCreationParams()const = 0;

		// ��newName��Ϊ���ƿ�¡һ���µĶ���
		virtual ISceneObject *clone(const String &newName , IScene *newScene = 0) = 0;
		// ���Զ����ɵ�������Ϊ���ƿ�¡һ���µĶ���
		virtual ISceneObject *clone(IScene *newScene = 0) = 0;
		// �ѵ�ǰ�����е����ݿ�¡��target��
		virtual void cloneData(ISceneObject *target) = 0;

		// �����ײʵ��
		virtual ICollisionEntity *getCollisionEntity()const = 0;

		// �����Ƿ�μ���ײ
		virtual void setCanCollide(bool can) = 0;
		// �����Ƿ�μ���ײ
		virtual bool getCanCollide()const = 0;

		// ��ó�������Ŀɼ����������ڱ༭����ѡ��
		virtual ISceneObjectVisible *getSceneObjectVisible() = 0;

		// �����û�����
		virtual void setUserAny(const Any& anything) = 0;
		// ����û�����
		virtual const Any& getUserAny() const = 0;
	};

	// ��Any��ֵת����T��ת��ʧ���򷵻�0
	template<typename T>
	inline T safe_cast(const Any & anything)
	{
		T* obj = any_cast<T>((Any*)&anything);
		return obj ? *obj : T();
	};

	// ��Any��ֵת����ISceneObject *��ת��ʧ���򷵻�0
	inline ISceneObject *obj_cast(const Any &anything)
	{
		return safe_cast<ISceneObject*>(anything);
	};

	// ��������Ķ��󹤳�
	class ISceneObjectFactory
	{
	public:
		// ��øù�������������
		virtual const String &getType() const = 0;
		// ��������Ĵ�������
		virtual ISceneObject *create(const String &name , IScene *scene , const NameValuePairList* params) = 0;
		// ������������ٷ���
		virtual void destroy(ISceneObject *obj) = 0;
	};
}