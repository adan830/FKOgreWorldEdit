// ============================================================
// �ļ���	:	IMapManager.h
// ������	:	������
// ˵��		:	��ͼ��������ʵ��
// ����ʱ��	:	2008-2-26 22:31:47
// ============================================================

#pragma once
#include "IMapManager.h"
#include "TListener.h"
namespace Ogre
{
	// ��ͼ��������ʵ��
	class MapManager : public IMapManager
		, public IMapManager::Listener
		, public TListener<IMapManager::Listener>
	{
	public:
		MapManager(void);
		virtual ~MapManager(void);


		// ���������������
		virtual void addListener(IMapManager::Listener *listener , bool toBack = true)
		{
			TListener<IMapManager::Listener>::addListener(listener , toBack);
		}
		// ɾ��������������
		virtual void removeListener(IMapManager::Listener *listener)
		{
			TListener<IMapManager::Listener>::removeListener(listener);
		}

		// ѡ�����г�������
		virtual void selectAllObject();
		// ��ѡ���г�������
		virtual void deselectAllObject();

		// ѡ��һ����������
		virtual void selectObject(ISceneObject *obj);
		// ѡ��һ����������
		virtual void selectObject(const ListSceneObject &listSceneObject);
		// ��ѡһ����������
		virtual void deselectObject(ISceneObject *obj);
		// ��ѡһ����������
		virtual void deselectObject(const ListSceneObject &listSceneObject);

		// ���������������Ƿ��ѱ�ѡ��
		virtual bool isObjectSelected(ISceneObject *obj)const;
		// ���������������Ƿ��ѱ�ѡ��
		virtual bool isObjectSelected(const String &name)const;

		// ���������Ѿ�ѡ������
		virtual void visitSelectedObject(ObjectVisitor *visitor);
		// ����Ѿ�ѡ������������
		virtual size_t getSelectedObjectCount()const
		{
			return m_selectedObject.size();
		}

		// �½�����
		virtual void newScene();
		// ����һ������
		virtual bool loadScene(const String &fileName);
		// ���泡��
		virtual bool saveScene(const String &fileName);
		// ���ٳ���
		virtual void destroyScene();
		// ��õ�ǰ����
		virtual IScene *getScene()
		{
			return m_scene;
		}


		// ѡ��һ����������
		virtual void onSelectObject(Ogre::ISceneObject *obj);
		// ѡ��һ����������
		virtual void onSelectObject(const Ogre::IMapManager::ListSceneObject &listSceneObject);
		// ��ѡһ����������
		virtual void onDeselectObject(Ogre::ISceneObject *obj);
		// ��ѡһ����������
		virtual void onDeselectObject(const Ogre::IMapManager::ListSceneObject &listSceneObject);

		// ���ѡ�������ö��
		virtual SelectedObjectsIterator getSelectedObjectsIterator()
		{
			return SelectedObjectsIterator(m_selectedObject.begin() , m_selectedObject.end());
		}
		// ���ѡ�������ö��
		virtual ConstSelectedObjectsIterator getConstSelectedObjectsIterator()const
		{
			return ConstSelectedObjectsIterator(m_selectedObject.begin() , m_selectedObject.end());
		}

		// �������ѡ�������ƽ�����ĵ�
		virtual bool getSelectedObjectsAvgCenter(Vector3 &center , const ISceneObject *exceptObject = 0)const;


	protected:
		// ��ѡ�еĶ����б�
		MapSelectedObjects m_selectedObject;

		// ��ǰ����ĳ���
		IScene *m_scene;

	};
};