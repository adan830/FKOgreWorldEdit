#include "StdAfx.h"
#include "Scene.h"
#include "NullObject.h"
#include "ISceneSystem.h"
#include "AnimationStateController.h"
#include "TerrainObject.h"
#include "CameraObject.h"
#include "ETTerrain.h"
#include "ETTile.h"

namespace Ogre
{

	Scene::Scene(SceneManager *mgr , const String &name , ICollisionWorld *collisionWorld , ITriggerWorld *triggerWorld)
		: m_name(name)
		, m_sceneMgr(mgr)
		, m_index(0)
		, m_rootObject(0)
		, m_collisionWorld(collisionWorld)
		, m_triggerWorld(triggerWorld)
		, m_rayQuery(0)
	{
		Root::getSingleton().addFrameListener(this);
		m_rootObject = new NullObject("root_object");
		NameValuePairList params;
		params["isRootNode"] = "";
		m_rootObject->create(this , &params);

		m_rayQuery = m_sceneMgr->createRayQuery(Ray());
	}

	Scene::~Scene(void)
	{
		clear();
		m_sceneMgr->destroyQuery(m_rayQuery);
		m_rayQuery = 0;
		OGRE_DELETE(m_rootObject);
		Root::getSingleton().removeFrameListener(this);
	}

	// ֪ͨ������������ĸ��ļ������
	void Scene::_notifyOrigin(const String &origin)
	{
		m_origin = origin;
	}
	// ������ڴ�������������ļ���
	const String &Scene::getOrigin()const
	{
		return m_origin;
	}

	// �����������
	void Scene::clear()
	{
		// ɾ�����ж���������
		for(MapAnimationStateController::iterator iter = m_mapAnimationStateController.begin() ; iter != m_mapAnimationStateController.end() ; iter ++)
		{
			m_sceneMgr->destroyAnimation(iter->first);
			delete iter->second;
		}
		m_mapAnimationStateController.clear();
		// ɾ���������
		for(MapSceneObject::iterator iter = m_mapSceneObject.begin() ; iter != m_mapSceneObject.end() ; iter ++)
		{
			ISceneObjectFactory* factory = ISceneSystem::getSingleton().getFactory(iter->second->getType());
			if(!factory)
			{
				OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
					"�Ҳ�������Ϊ '" + iter->second->getType() + "' �Ķ��󹤳�", 
					"Scene::destroySceneObject");
				return;
			}
			// �ص���������
			ExecuteListener1(IScene::Listener , onDestroySceneObject , iter->second);
			// ɾ������
			factory->destroy(iter->second);
		}
		m_mapSceneObject.clear();
	}

	// ���һ��������ΨһID
	uint32 Scene::getUniqueID()
	{
		return ++m_index;
	}

	// ���һ��������Ψһ����
	String Scene::getUniqueName()
	{
		String name;
		do 
		{
			name = "_object_" + StringConverter::toString(getUniqueID());
		}while(hasSceneObject(name));
		return name;	
	}

	// �������ͺ����ƴ���һ����������
	ISceneObject *Scene::createSceneObject(const String &type, const NameValuePairList* params)
	{
		return createSceneObject(getUniqueName() , type , params);
	}

	// �������ͺ����ƴ���һ����������
	ISceneObject *Scene::createSceneObject(const String &name , const String &type , const NameValuePairList* params)
	{
		ISceneObjectFactory* factory = ISceneSystem::getSingleton().getFactory(type);
		if(!factory)
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"�Ҳ�������Ϊ '" + type + "' �Ķ��󹤳�", 
				"Scene::createSceneObject");
			return 0;
		}

		if(hasSceneObject(name))
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
				"������������Ϊ '" + name + "' �ĳ��������Ѿ�����", 
				"Scene::createSceneObject");
			return 0;
		}
		ISceneObject *obj = factory->create(name , this , params);
		m_mapSceneObject[name] = obj;
		// �ص���������
		ExecuteListener1(IScene::Listener , onCreateSceneObject , obj);
		return obj;		
	}


	// ���ٳ�������
	void Scene::destroySceneObject(const String &name)
	{
		MapSceneObject::iterator iter = m_mapSceneObject.find(name);
		if(iter == m_mapSceneObject.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"������������Ϊ '" + name + "' �ĳ������󲻴���", 
				"Scene::destroyScene");
			return;
		}
		ISceneObjectFactory* factory = ISceneSystem::getSingleton().getFactory(iter->second->getType());
		if(!factory)
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"�Ҳ�������Ϊ '" + iter->second->getType() + "' �Ķ��󹤳�", 
				"Scene::destroySceneObject");
			return;
		}
		// �ص���������
		ExecuteListener1(IScene::Listener , onDestroySceneObject , iter->second);

		factory->destroy(iter->second);
		m_mapSceneObject.erase(iter);
	}

	void Scene::destroySceneObject(ISceneObject *obj)
	{
		destroySceneObject(obj->getName());
	}
	// ���һ����������
	ISceneObject *Scene::getSceneObject(const String &name)
	{
		MapSceneObject::iterator iter = m_mapSceneObject.find(name);
		if(iter == m_mapSceneObject.end())
		{
			// ���ظ�����
			if(name == m_rootObject->getName())
			{
				return m_rootObject;
			}

			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"������������Ϊ '" + name + "' �ĳ������󲻴���", 
				"Scene::getScene");
			return 0;
		}
		return iter->second;
	}
	// �Ƿ����ָ�����ֵĳ�������
	bool Scene::hasSceneObject(const String &name)const
	{
		return m_mapSceneObject.find(name) != m_mapSceneObject.end();
	}
	// �Ƿ��obj�����������
	bool Scene::hasSceneObject(ISceneObject *obj)const
	{
		if(!obj)
			return false;
		return hasSceneObject(obj->getName());
	}

	// ���ָ������ڴ˳����Ƿ���Ч
	bool Scene::isSceneObjectValid(ISceneObject *obj)const
	{
		for(MapSceneObject::const_iterator iter = m_mapSceneObject.begin() ; iter != m_mapSceneObject.end() ; iter ++)
		{
			if(iter->second == obj)
				return true;
		}
		return false;
	}

	// ���Ogre�ĳ���������
	SceneManager *Scene::getOgreSceneManager()const
	{
		return m_sceneMgr;
	}

	// ����һ������������
	IAnimationStateController* Scene::createAnimationStateController(const String &name , const String &animationName)
	{
		if(!m_sceneMgr->hasAnimation(animationName))
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"������������Ϊ '" + animationName + "' �ĳ�������������", 
				"Scene::createAnimationStateController");
			return 0;
		}
		MapAnimationStateController::iterator iter = m_mapAnimationStateController.find(name);
		if(iter != m_mapAnimationStateController.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"������������������Ϊ '" + name + "' �ĳ��������������Ѿ�����", 
				"Scene::createAnimationStateController");
			return 0;
		}

		AnimationState *state = m_sceneMgr->createAnimationState(animationName);
		AnimationStateController* ani = new AnimationStateController;
		ani->create(name , state);
		// ��ӵ��Զ������б�
		m_mapAnimationStateController[name] = ani;
		return ani;
	}

	// ����һ������������
	IAnimationStateController* Scene::createAnimationStateController(const String &animationName)
	{
		return createAnimationStateController(getUniqueName() , animationName);
	}

	// ����һ������������
	void Scene::destroyAnimationStateController(const String &name)
	{
		MapAnimationStateController::iterator iter = m_mapAnimationStateController.find(name);
		if(iter == m_mapAnimationStateController.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"������������������Ϊ '" + name + "' �ĳ�������������������", 
				"Scene::destroyAnimationStateController");
			return;
		}
		m_sceneMgr->destroyAnimationState(iter->second->getAnimationName());
		delete iter->second;
		m_mapAnimationStateController.erase(iter);
	}

	// ����һ������������
	void Scene::destroyAnimationStateController(IAnimationStateController *ani)
	{
		destroyAnimationStateController(ani->getName());
	}
	// ���һ������������
	IAnimationStateController* Scene::getAnimationStateController(const String &name)
	{
		MapAnimationStateController::iterator iter = m_mapAnimationStateController.find(name);
		if(iter == m_mapAnimationStateController.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"������������������Ϊ '" + name + "' �ĳ�������������������", 
				"Scene::getAnimationStateController");
			return 0;
		}
		return iter->second;

	}
	// �Ƿ�����������������
	bool Scene::hasAnimationStateController(const String &name)
	{
		return m_mapAnimationStateController.find(name) != m_mapAnimationStateController.end();
	}
	// �Ƿ�����������
	bool Scene::hasAnimation(const String &animationName)
	{
		return m_sceneMgr->hasAnimation(animationName);
	}

	// ֡��������ÿִ֡��
	bool Scene::frameStarted( const FrameEvent& evt )
	{
		for(MapAnimationStateController::iterator iter = m_mapAnimationStateController.begin() ; iter != m_mapAnimationStateController.end() ; iter ++)
		{
			if(iter->second->isPlaying())
			{
				iter->second->update(evt.timeSinceLastFrame);
			}
		}
		return true;
	}
	// ����������
	bool Scene::renameSceneObject(const String &oldName , const String &newName)
	{
		MapSceneObject::iterator iter = m_mapSceneObject.find(oldName);
		if(iter == m_mapSceneObject.end())
			return false;
		if(hasSceneObject(newName))
			return false;
		ISceneObject *obj = iter->second;
		m_mapSceneObject.erase(iter);
		m_mapSceneObject[newName] = obj;
		return true;
	}


	// ������ײ���
	// @ray ����
	// @collideWorld �Ƿ��������ײ���磺���Σ�
	// @collideObject �Ƿ�������ײ
	// @info ���������ײ�����Ϣ
	bool Scene::collide(const Ray& ray , bool collideWorld , bool collideObject , RayCheckInfo& info)
	{
		ListRayCheckInfo listInfo;
		bool ret = collide(ray , collideWorld , collideObject , true , true , listInfo);
		if(!ret)
		{
			return false;
		}
		else
		{
			if(listInfo.empty())
			{
				return false;
			}
			else
			{
				info = listInfo.front();
				return true;
			}
		}
	}

	// ������ײ��⣬�����ص�һ��������ײ����Ϣ
	// @ray ����
	// @collideWorld �Ƿ��������ײ���磺���Σ�
	// @collideObject �Ƿ�������ײ
	// @intersection �������
	bool Scene::collide(const Ray& ray , bool collideWorld , bool collideObject , Vector3& intersection)
	{
		RayCheckInfo info;
		if(!collide(ray , collideWorld , collideObject , info))
			return false;
		intersection = ray.getPoint(info.info.distance);
		return true;
	}

	// ������ײ���
	// @ray ����
	// @collideWorld �Ƿ��������ײ���磺���Σ�
	// @collideObject �Ƿ�������ײ
	// @infoList ���������ײ�����Ϣ
	// @sortByDistance �����infoList����������
	bool Scene::collide(const Ray& ray , bool collideWorld , bool collideObject , bool sortByDistance , ListRayCheckInfo& info)
	{
		return collide(ray , collideWorld , collideObject , sortByDistance , false , info);
	}

	// ������ײ���
	// @ray ����
	// @collideWorld �Ƿ��������ײ���磺���Σ�
	// @collideObject �Ƿ�������ײ
	// @infoList ���������ײ�����Ϣ
	// @sortByDistance �����infoList����������
	// @returnFirstOnly ֻ���ص�һ����ײ����
	bool Scene::collide(const Ray& ray , bool collideWorld , bool collideObject , bool sortByDistance , bool returnFirstOnly , ListRayCheckInfo& infoList)
	{
		m_rayQuery->setRay(ray);
		m_rayQuery->execute();
		RaySceneQueryResult& result = m_rayQuery->getLastResults();
		if(result.empty())
		{
			return false;
		}
		// �������н��
		for(RaySceneQueryResult::iterator iter = result.begin() ; iter != result.end() ; iter ++)
		{
			// ���ֻ����һ���������Ѿ�����ײ�����ˣ����˳���
			if(returnFirstOnly && !infoList.empty())
				return true;
			// �����������ײ
			if(iter->movable->getMovableType() == "Camera")
				continue;
			// ת���ɳ�������
			ISceneObject *obj = obj_cast(iter->movable->getUserAny());
			// ����г������󣬲��Ҵ˳�������μ���ײ����
			if(obj && obj->getCanCollide())
			{
				// ���Ҫ��ײ���磬�͸�������
				if(collideWorld)
				{
					// ����ǵ���Tile
					if(iter->movable->getMovableType() == Tile::MOVABLE_OBJECT_TYPE)
					{
						Tile *tile = static_cast<Tile*>(iter->movable);
						RayCheckInfo info;
						// ��ײ���
						if(tile->collide(ray , info.info.distance))
						{
							info.obj = obj;
							infoList.push_back(info);
						}
						continue;
					}
				}


				// ���Ҫ�������
				if(collideObject)
				{
					// ���β����������
					if(obj->getType() == TerrainObjectFactory::FACTORY_TYPE)
					{
						continue;
					}

					// ����о�ȷ��ײ�壬���þ�ȷ��ײ��������
					if(obj->getCollisionEntity() && obj->getCollisionEntity()->isInited())
					{
						RayCheckInfo info;
						// �����������ײ
						if(obj->getCollisionEntity()->collide(ray , info.info))
						{
							info.obj = obj;
							infoList.push_back(info);
						}
					}
					else
					{
						RayCheckInfo info;
						info.obj = obj;
						info.info.distance = iter->distance;
						infoList.push_back(info);
					}
				}
			}
		}
		// ����������
		if(sortByDistance && !infoList.empty())
		{
			std::sort(infoList.begin() , infoList.end());
		}

		return !infoList.empty();
	}
};