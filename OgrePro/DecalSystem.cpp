/******************************************************************
** �ļ���:	DecalSystem.cpp
** ��  Ȩ:	(C)  
** ������:	FreeKnight(������)
** ��  ��:	2008-1-10 9:53:29
** ��  ��:
******************************************************************/
#include "stdafx.h"
#include "DecalSystem.h"
#include "MeshDecal.h"
#include "NormalDecal.h"
#include "GridDecal.h"
#include "ETTerrain.h"

namespace Ogre
{
	DecalSystem::DecalSystem(SceneManager *mgr)
		: m_sceneMgr(0)
		, m_rayQuery(0)

	{
		acceptSceneMgr(mgr);
		Root::getSingleton().addFrameListener(this);
	}

	DecalSystem::~DecalSystem()
	{
		destroyAllDecal();
		if(m_rayQuery)
		{
			m_sceneMgr->destroyQuery(m_rayQuery);
			m_rayQuery = 0;
		}
		Root::getSingleton().removeFrameListener(this);
	}


	// ʹ����������������������������֮��createDecal��ʹ���������������
	void DecalSystem::acceptSceneMgr(SceneManager *mgr)
	{
		m_sceneMgr = mgr;
		if(m_rayQuery)
		{
			m_sceneMgr->destroyQuery(m_rayQuery);
			m_rayQuery = 0;
		}
		m_rayQuery = m_sceneMgr->createRayQuery(Ray());
		m_rayQuery->setSortByDistance(true);
	}


	// ���ݲ�����ʵ��������һ�����������ʵ����Ϊ�վ��Զ�����
	IDecal *DecalSystem::createDecal(DecalType decalType
		, const String &name
		, const NameValuePairList *params
		)
	{
		String nameCreate(name);
		if(nameCreate.empty())
		{
			nameCreate = getUniqueDecalName();
		}
		if(hasDecal(nameCreate))
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM,
				"ָ������������������ϵͳ���Ѿ����ڣ�" + nameCreate,
				"DecalSystem::createDecal");
			return 0;
		}
		IDecal *decal = 0;
		switch(decalType)
		{
			// ��������
		case DT_NORMAL:
			decal = new NormalDecal(m_sceneMgr , nameCreate);
			break;
			// ��������
		case DT_MESH:
			decal = new MeshDecal(m_sceneMgr , nameCreate);
			break;
		case DT_PROJECTIVE:
		case DT_GRID:
			decal = new GridDecal(m_sceneMgr , nameCreate);
			if(params)
			{
				NameValuePairList::const_iterator iter;

				// ��ʼ������
				iter = params->find("WidthGrid");
				if(iter != params->end())
				{
					static_cast<GridDecal*>(decal)->setWidthGridCount(StringConverter::parseLong(iter->second));
				}

				iter = params->find("WidthGrid");
				if(iter != params->end())
				{
					static_cast<GridDecal*>(decal)->setHeightGridCount(StringConverter::parseLong(iter->second));
				}
			}
			break;
		default:
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"��֧�ֵ�DecalType�����decalTypeΪ��" + StringConverter::toString(decalType),
				"DecalSystem::createDecal");
			return 0;
			break;
		}
		m_mapDecal[nameCreate] = decal;
		return decal;
	}

	// ����һ������
	void DecalSystem::destroyDecal(IDecal *decal)
	{
		destroyDecal(decal->getName());
	}

	void DecalSystem::destroyDecal(const String &name)
	{
		MapDecal::iterator iter = m_mapDecal.find(name);
		if(iter == m_mapDecal.end())
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM,
				"ָ������������������ϵͳ�в����ڣ�" + name,
				"DecalSystem::destroyDecal");
			return;
		}
		delete iter->second;
		m_mapDecal.erase(iter);
	}

	// ������������
	void DecalSystem::destroyAllDecal()
	{
		for(MapDecal::iterator iter = m_mapDecal.begin() ; iter != m_mapDecal.end() ; iter ++)
		{
			delete iter->second;
		}
		m_mapDecal.clear();
	}

	// ����ʵ�������һ������
	IDecal *DecalSystem::getDecal(const String &name)
	{
		MapDecal::iterator iter = m_mapDecal.find(name);
		if(iter == m_mapDecal.end())
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM,
				"ָ������������������ϵͳ�в����ڣ�" + name,
				"DecalSystem::getDecal");
			return 0;
		}
		return iter->second;
	}
	// ����Ƿ������ʵ����������
	bool DecalSystem::hasDecal(const String &name)const
	{
		return m_mapDecal.find(name) != m_mapDecal.end();
	}

	// �����������ö��
	DecalSystem::DecalIterator DecalSystem::getDecalIterator()
	{
		return DecalIterator(m_mapDecal.begin() , m_mapDecal.end());
	}

	// �����������ö��
	DecalSystem::ConstDecalIterator DecalSystem::getDecalIterator()const
	{
		return ConstDecalIterator(m_mapDecal.begin() , m_mapDecal.end());
	}

	// ÿ֡������������
	bool DecalSystem::frameStarted(const FrameEvent& evt)
	{
		for(MapDecal::iterator iter = m_mapDecal.begin() ; iter != m_mapDecal.end() ; iter ++)
		{
			iter->second->update(evt.timeSinceLastFrame);
		}
		return true;
	}


	// ���߼��
	ETTerrain* DecalSystem::worldRayQuery(Vector3 &posIntersect , const Vector3 &posStart , const Vector3 &posEnd)
	{
		Ray ray(posStart , (posEnd - posStart).normalisedCopy());
		return worldRayQuery(posIntersect , ray);
	}

	// ���߼��
	ETTerrain* DecalSystem::worldRayQuery(Vector3 &posIntersect , const Ray &ray)
	{
		typedef SharedPtr<RaySceneQuery> RaySceneQueryPtr;
		RaySceneQueryPtr query(m_sceneMgr->createRayQuery(ray));
		query->setSortByDistance(true);
		query->execute();
		RaySceneQueryResult& result = query->getLastResults();
		if(result.empty())
		{
			return 0;
		}
		for(RaySceneQueryResult::iterator iter = result.begin() ; iter != result.end() ; iter ++)
		{
			// ����ǵ���
			if(iter->movable->getMovableType() == ETTerrain::MOVABLE_TYPE)
			{
				// ת���ɵ��ζ���
				ETTerrain *terrain = static_cast<TerrainImpl*>(iter->movable)->getTerrain();
				if(!terrain)
				{
					return 0;
				}
				std::pair<bool, Vector3> res = terrain->rayIntersects(ray);
				if(!res.first)
					return 0;
				posIntersect = res.second;
				return terrain;
			}
		}
		return 0;
	}
	// ���߼��
	bool DecalSystem::rayQuery(Vector3 &posIntersect , const Vector3 &posStart , const Vector3 &posEnd)
	{
		Ray ray(posStart , (posEnd - posStart).normalisedCopy());
		m_rayQuery->setRay(ray);
		RaySceneQueryResult& result = m_rayQuery->execute();
		for(RaySceneQueryResult::iterator iter = result.begin() ; iter != result.end() ; iter ++)
		{
			// ʹ�����������ƽ�淨��
			if(iter->worldFragment)
			{
				posIntersect = iter->worldFragment->singleIntersection;
				return true;
			}
			// ʹ��ģ�͵�����ƽ�淨��
			else
			{
				posIntersect = ray.getPoint(iter->distance);
				return true;
			}
		}
		return false;
	}

	// ��������ԣ��������罻�����꣨���Σ�
	ETTerrain* DecalSystem::hitTestWorld(Vector3 &posIntersect , short cursorX, short cursorY , Camera *camera)
	{
		// �õ���괦��һ������
		float fx,fy;
		RenderTarget* target = camera->getViewport()->getTarget();
		fx = (float)cursorX / (target->getWidth());
		fy = (float)cursorY / (target->getHeight());
		Ray hitRay = camera->getCameraToViewportRay(fx, fy); 

		return worldRayQuery(posIntersect , hitRay);
	}

};