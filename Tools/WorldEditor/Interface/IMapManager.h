// ============================================================
// �ļ���	:	IMapManager.h
// ������	:	������
// ˵��		:	��ͼ�������Ľӿ�
// ����ʱ��	:	2008-2-26 22:31:47
// ============================================================
#pragma once

namespace Ogre
{
	// ��ͼ�������Ľӿ�
	class IMapManager
	{
	public:

		IMapManager(void)
		{
		}

		virtual ~IMapManager(void)
		{
		}
		typedef std::list<ISceneObject*> ListSceneObject;

		// �¼�������
		struct Listener
		{
			// ѡ��һ����������
			virtual void onSelectObject(ISceneObject *obj){}
			// ѡ��һ����������
			virtual void onSelectObject(const ListSceneObject &listSceneObject){}
			// ��ѡһ����������
			virtual void onDeselectObject(ISceneObject *obj){}
			// ��ѡһ����������
			virtual void onDeselectObject(const ListSceneObject &listSceneObject){}

			// �½�����
			virtual void onNewScene(IScene *scene){};
			// ����һ������
			virtual void onPreLoadScene(const String &fileName ,IScene *scene){};
			// ����һ������
			virtual void onPostLoadScene(const String &fileName ,IScene *scene){};
			// ���泡��
			virtual void onPreSaveScene(const String &fileName ,IScene *scene){};
			// ���泡��
			virtual void onPostSaveScene(const String &fileName ,IScene *scene){};
			// ���ٳ���
			virtual void onDestroyScene(IScene *scene){};
		};

		// ���������
		virtual void addListener(Listener *listener , bool toBack = true) = 0;
		// ɾ��������
		virtual void removeListener(Listener *listener) = 0;


		// ѡ�����г�������
		virtual void selectAllObject() = 0;
		// ��ѡ���г�������
		virtual void deselectAllObject() = 0;


		// ѡ��һ����������
		virtual void selectObject(ISceneObject *obj) = 0;
		// ѡ��һ����������
		virtual void selectObject(const ListSceneObject &listSceneObject) = 0;
		// ��ѡһ����������
		virtual void deselectObject(ISceneObject *obj) = 0;
		// ��ѡһ����������
		virtual void deselectObject(const ListSceneObject &listSceneObject) = 0;

		// ���������������Ƿ��ѱ�ѡ��
		virtual bool isObjectSelected(ISceneObject *obj)const = 0;
		// ���������������Ƿ��ѱ�ѡ��
		virtual bool isObjectSelected(const String &name)const = 0;

		// ���������
		struct ObjectVisitor
		{
			// ����������󣬵�����falseʱ���ٱ�������Ķ���
			virtual bool visit(ISceneObject *obj) = 0;
		};
		// ���������Ѿ�ѡ������
		virtual void visitSelectedObject(ObjectVisitor *visitor) = 0;
		// ����Ѿ�ѡ������������
		virtual size_t getSelectedObjectCount()const = 0;

		typedef std::map<String , ISceneObject*> MapSelectedObjects;
		typedef MapIterator<MapSelectedObjects> SelectedObjectsIterator;
		typedef ConstMapIterator<MapSelectedObjects> ConstSelectedObjectsIterator;
		// ���ѡ�������ö��
		virtual SelectedObjectsIterator getSelectedObjectsIterator() = 0;
		virtual ConstSelectedObjectsIterator getConstSelectedObjectsIterator()const = 0;

		// �������ѡ�������ƽ�����ĵ�
		virtual bool getSelectedObjectsAvgCenter(Vector3 &center , const ISceneObject *exceptObject = 0)const = 0;

		// �½�����
		virtual void newScene() = 0;
		// ����һ������
		virtual bool loadScene(const String &fileName) = 0;
		// ���泡��
		virtual bool saveScene(const String &fileName) = 0;
		// ���ٳ���
		virtual void destroyScene() = 0;

		// ��õ�ǰ����
		virtual IScene *getScene() = 0;
	};
};