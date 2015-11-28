#include "StdAfx.h"
#include "MapManager.h"
#include "ISceneObjectVisible.h"

namespace Ogre
{

	MapManager::MapManager(void)
		: m_scene(0)
	{
		addListener(this);
	}

	MapManager::~MapManager(void)
	{
		removeListener(this);
	}

	// ѡ��һ����������
	void MapManager::selectObject(ISceneObject *obj)
	{
		if(m_selectedObject.find(obj->getFullName()) == m_selectedObject.end())
		{
			m_selectedObject[obj->getFullName()] = obj;

			// �ص���������
			ExecuteListener1(IMapManager::Listener , onSelectObject , obj);
		}
	}
	// ѡ��һ����������
	void MapManager::selectObject(const ListSceneObject &listSceneObject)
	{
		ListSceneObject listAdded;
		for(ListSceneObject::const_iterator iter = listSceneObject.begin() ; iter != listSceneObject.end() ; iter ++)
		{
			if(m_selectedObject.find((*iter)->getFullName()) == m_selectedObject.end())
			{
				m_selectedObject[(*iter)->getFullName()] = *iter;
				listAdded.push_back(*iter);
			}
		}

		if(!listAdded.empty())
		{
			if(listAdded.size() == 1)
			{
				// �ص���������
				ExecuteListener1(IMapManager::Listener , onSelectObject , listAdded.back());
			}
			else
			{
				// �ص���������
				ExecuteListener1(IMapManager::Listener , onSelectObject , listAdded);
			}
		}
	}

	// ��ѡһ����������
	void MapManager::deselectObject(ISceneObject *obj)
	{
		MapSelectedObjects::iterator iter = m_selectedObject.find(obj->getFullName());
		if(iter != m_selectedObject.end())
		{
			m_selectedObject.erase(iter);
			// �ص���������
			ExecuteListener1(IMapManager::Listener , onDeselectObject , obj);
		}
	}
	// ��ѡһ����������
	void MapManager::deselectObject(const ListSceneObject &listSceneObject)
	{
		ListSceneObject listRemoved;
		for(ListSceneObject::const_iterator iter = listSceneObject.begin() ; iter != listSceneObject.end() ; iter ++)
		{
			MapSelectedObjects::iterator iterFind = m_selectedObject.find((*iter)->getFullName());
			if(iterFind != m_selectedObject.end())
			{
				m_selectedObject.erase(iterFind);
				listRemoved.push_back(*iter);
			}
		}

		if(!listRemoved.empty())
		{
			if(listRemoved.size() == 1)
			{
				// �ص���������
				ExecuteListener1(IMapManager::Listener , onDeselectObject , listRemoved.back());
			}
			else
			{
				// �ص���������
				ExecuteListener1(IMapManager::Listener , onDeselectObject , listRemoved);
			}
		}
	}


	// ���������Ѿ�ѡ������
	void MapManager::visitSelectedObject(ObjectVisitor *visitor)
	{
		MapSelectedObjects selectedObject(m_selectedObject);
		for(MapSelectedObjects::iterator iter = selectedObject.begin() ; iter != selectedObject.end() ; iter ++)
		{
			visitor->visit(iter->second);
		}
	}


	// ѡ�����г�������
	void MapManager::selectAllObject()
	{
		if(!m_scene)
			return;
		IScene::SceneObjectIterator iterator = m_scene->getSceneObjectIterator();
		ListSceneObject listAdded;
		while(iterator.hasMoreElements())
		{
			ISceneObject *obj = iterator.getNext();
			listAdded.push_back(obj);
		}
		selectObject(listAdded);
	}

	// ���������������Ƿ��ѱ�ѡ��
	bool MapManager::isObjectSelected(ISceneObject *obj)const
	{
		return isObjectSelected(obj->getFullName());
	}

	// ���������������Ƿ��ѱ�ѡ��
	bool MapManager::isObjectSelected(const String &name)const
	{
		return m_selectedObject.find(name) != m_selectedObject.end();
	}

	// ��ѡ���г�������
	void MapManager::deselectAllObject()
	{
		ListSceneObject listRemoved;
		for(MapSelectedObjects::iterator iter = m_selectedObject.begin() ; iter != m_selectedObject.end() ; iter ++)
		{
			listRemoved.push_back(iter->second);
		}
		m_selectedObject.clear();

		if(!listRemoved.empty())
		{
			if(listRemoved.size() == 1)
			{
				// �ص���������
				ExecuteListener1(IMapManager::Listener , onDeselectObject , listRemoved.back());
			}
			else
			{
				// �ص���������
				ExecuteListener1(IMapManager::Listener , onDeselectObject , listRemoved);
			}
		}
	}


	// �½�����
	void MapManager::newScene()
	{
		destroyScene();
		m_scene = ISceneSystem::getSingleton().createScene();
		// �ص���������
		ExecuteListener1(IMapManager::Listener , onNewScene , m_scene);
	}
	// ����һ������
	bool MapManager::loadScene(const String &fileName)
	{
		destroyScene();
		m_scene = ISceneSystem::getSingleton().createScene();
		// �ص���������
		ExecuteListener2(IMapManager::Listener , onPreLoadScene , fileName , m_scene);
		// ���볡��
		ISceneSystem::getSingleton().loadScene(fileName , m_scene);
		// �ص���������
		ExecuteListener2(IMapManager::Listener , onPostLoadScene , fileName , m_scene);

		return m_scene != 0;
	}
	// ���泡��
	bool MapManager::saveScene(const String &fileName)
	{
		// �ص���������
		ExecuteListener2(IMapManager::Listener , onPreSaveScene , fileName , m_scene);

		bool ret = ISceneSystem::getSingleton().saveScene(m_scene , fileName);

		// �ص���������
		ExecuteListener2(IMapManager::Listener , onPostSaveScene , fileName , m_scene);

		return ret;
	}
	// ���ٳ���
	void MapManager::destroyScene()
	{
		// �ص���������
		ExecuteListener1(IMapManager::Listener , onDestroyScene , m_scene);

		deselectAllObject();
		if(m_scene)
		{
			ISceneSystem::getSingleton().destroyScene(m_scene);
			m_scene = 0;
		}
	}


	// ѡ��һ����������
	void MapManager::onSelectObject(Ogre::ISceneObject *obj)
	{
		obj->getSceneObjectVisible()->showBorder(true);
	}

	// ��ѡһ����������
	void MapManager::onDeselectObject(Ogre::ISceneObject *obj)
	{
		obj->getSceneObjectVisible()->showBorder(false);
	}

	// ѡ��һ����������
	void MapManager::onSelectObject(const Ogre::IMapManager::ListSceneObject &listSceneObject)
	{
		for(IMapManager::ListSceneObject::const_iterator iter = listSceneObject.begin() ; iter != listSceneObject.end() ; iter ++)
		{
			onSelectObject(*iter);
		}
	}

	// ��ѡһ����������
	void MapManager::onDeselectObject(const Ogre::IMapManager::ListSceneObject &listSceneObject)
	{
		for(IMapManager::ListSceneObject::const_iterator iter = listSceneObject.begin() ; iter != listSceneObject.end() ; iter ++)
		{
			onDeselectObject(*iter);
		}
	}


	// �������ѡ�������ƽ�����ĵ�
	bool MapManager::getSelectedObjectsAvgCenter(Vector3 &center , const ISceneObject *exceptObject)const
	{
		if(m_selectedObject.empty())
		{
			return false;
		}
		//get the min/max of x,y,z and choose the center of the bounding box
		Vector3 minVec = Vector3::ZERO;
		Vector3 maxVec = Vector3::ZERO;

		bool first = true;

		for(MapSelectedObjects::const_iterator iter = m_selectedObject.begin() ; iter != m_selectedObject.end() ; iter ++)
		{
			if(iter->second == exceptObject)
				continue;
			const Vector3& vec = iter->second->getWorldPosition();
			if(first)
			{
				minVec = vec;
				maxVec = vec;
				first = false;
			}
			else
			{
				if(vec.x < minVec.x)
					minVec.x = vec.x;

				if(vec.y < minVec.y)
					minVec.y = vec.y;

				if(vec.z < minVec.z)
					minVec.z = vec.z;

				if(vec.x > maxVec.x)
					maxVec.x = vec.x;

				if(vec.y > maxVec.y)
					maxVec.y = vec.y;

				if(vec.z > maxVec.z)
					maxVec.z = vec.z;
			}
		}

		//add and divide by 2
		//Vector3 avg = Vector3(maxVec.x + minVec.x / 2.0f, maxVec.y + minVec.y / 2.0f, maxVec.z + minVec.z / 2.0f);
		center = (maxVec + minVec) / 2.0f;
		return true;
	}
};