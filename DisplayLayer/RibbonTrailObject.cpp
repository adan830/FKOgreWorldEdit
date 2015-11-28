#include "StdAfx.h"
#include "RibbonTrailObject.h"
#include "IScene.h"
#include "EntityObject.h"
#include "OgreTagPoint.h"
namespace Ogre
{

	const String RibbonTrailObjectFactory::FACTORY_TYPE = "RibbonTrailObject";

	// ��øù�������������
	const String &RibbonTrailObjectFactory::getType() const
	{
		return FACTORY_TYPE;
	}
	// ��������Ĵ�������
	ISceneObject *RibbonTrailObjectFactory::create(const String &name , IScene *scene , const NameValuePairList* params)
	{
		RibbonTrailObject *obj = new RibbonTrailObject(name);
		obj->create(scene , params);
		return obj;
	}

	// ������������ٷ���
	void RibbonTrailObjectFactory::destroy(ISceneObject *obj)
	{
		OGRE_DELETE(obj);
	}

	RibbonTrailObject::RibbonTrailObject(const String &name)
		: NullObject(name)
		, m_ribbonTrail(0)
	{
	}

	RibbonTrailObject::~RibbonTrailObject(void)
	{
		destroy();
	}

	// ��������ʵ��
	void RibbonTrailObject::create(IScene *scene , const NameValuePairList* params)
	{
		destroy();
		NullObject::create(scene , params);
		// ����ʵ��
		m_ribbonTrail = static_cast<RibbonTrail*>(m_scene->getOgreSceneManager()->
			createMovableObject(getFullName(), RibbonTrailFactory::FACTORY_TYPE_NAME, params));
		m_ribbonTrail->setUserAny(Any(static_cast<ISceneObject*>(this)));
		// �ҽ���ȥ
		m_node->attachObject(m_ribbonTrail);
	}

	// ���ٶ���ʵ��
	void RibbonTrailObject::destroy()
	{
		if(m_ribbonTrail)
		{
			// ����ģ��ʵ��
			m_scene->getOgreSceneManager()->destroyMovableObject(m_ribbonTrail);
			m_ribbonTrail = 0;
		}
		NullObject::destroy();
	}

	// ��������������������󣬷���chainIndex
	size_t RibbonTrailObject::trail(ISceneObject *obj)
	{
		ListTrailObject::iterator iter = std::find(m_listTrailObject.begin() , m_listTrailObject.end() , obj);
		if(iter != m_listTrailObject.end())
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
				"����Ϊ '" + obj->getName() + "�ĳ��������Ѿ�������", 
				"RibbonTrailObject::trail");
			return -1;
		}

		// ��ӵ�������
		m_ribbonTrail->addNode(static_cast<NullObject*>(obj)->getOgreSceneNode());
		// ��ӵ��������б�
		m_listTrailObject.push_back(TrailObject(obj));
		return m_listTrailObject.size() - 1;
	}

	size_t RibbonTrailObject::trail(const String &objName)
	{
		return trail(m_scene->getSceneObject(objName));
	}

	// ֹͣ����
	void RibbonTrailObject::stopTrail(ISceneObject *obj)
	{
		ListTrailObject::iterator iter = std::find(m_listTrailObject.begin() , m_listTrailObject.end() , obj);
		if(iter == m_listTrailObject.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"�Ҳ�������Ϊ '" + obj->getName() + "�ĳ�������", 
				"RibbonTrailObject::stopTrail");
			return;
		}
		m_ribbonTrail->removeNode(static_cast<NullObject*>(obj)->getOgreSceneNode());
		m_listTrailObject.erase(iter);
	}

	void RibbonTrailObject::stopTrail(const String &objName)
	{
		stopTrail(m_scene->getSceneObject(objName));
	}

	void RibbonTrailObject::stopTrail(size_t chainIndex)
	{
		if(chainIndex >= m_listTrailObject.size())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"����Խ�磬����ֵΪ '" + StringConverter::toString(chainIndex) + "���������СΪ" + StringConverter::toString(m_listTrailObject.size()), 
				"RibbonTrailObject::stopTrail");
			return;
		}
		const TrailObject &trail = m_listTrailObject[chainIndex];
		if(trail.boneName.empty())
		{
			stopTrail(trail.object);
		}
		else
		{
			stopTrail(trail.entity , trail.boneName);
		}
	}


	// �����������������������chainIndex
	size_t RibbonTrailObject::trail(EntityObject *obj , const String &boneName)
	{
		SkeletonInstance* skeleton = obj->getOgreEntity()->getSkeleton();
		if(skeleton == 0)
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"�������� '" + obj->getName() + "'û�й���",
				"RibbonTrailObject::trail");
			return -1;
		}

		// ����ظ�����
		for(ListTrailObject::iterator iter = m_listTrailObject.begin() ; iter != m_listTrailObject.end() ; iter ++)
		{
			if(iter->entity == obj
				&& iter->boneName == boneName
				&& iter->boneTag != 0
				)
			{
				OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
					"����Ϊ '" + obj->getName() + "'�ĳ�������Ĺ���'" + boneName + "'�Ѿ�������", 
					"RibbonTrailObject::trail");
				return -1;
			}
		}

		TagPoint* tag = skeleton->createTagPointOnBone(skeleton->getBone(boneName));
		tag->setParentEntity(obj->getOgreEntity());
		tag->setChildObject(m_ribbonTrail);
		m_ribbonTrail->addNode(tag);

		// ��ӵ��������б�
		m_listTrailObject.push_back(TrailObject(obj , boneName , tag));
		return m_listTrailObject.size() - 1;
	}

	size_t RibbonTrailObject::trail(const String &entityObjName , const String &boneName)
	{
		return trail(static_cast<EntityObject*>(m_scene->getSceneObject(entityObjName)) , boneName);
	}

	// ֹͣ����
	void RibbonTrailObject::stopTrail(EntityObject *obj , const String &boneName)
	{
		// �ҵ������ٵĹ�ͷ
		ListTrailObject::iterator iterFound;
		for(ListTrailObject::iterator iter = m_listTrailObject.begin() ; iter != m_listTrailObject.end() ; iter ++)
		{
			if(iter->entity == obj
				&& iter->boneName == boneName
				&& iter->boneTag != 0
				)
			{
				iterFound = iter;
				break;
			}
		}
		if(iterFound == m_listTrailObject.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"����Ϊ '" + obj->getName() + "'�ĳ�������Ĺ���'" + boneName + "'û�б�����", 
				"RibbonTrailObject::stopTrail");
			return;
		}
		m_ribbonTrail->removeNode(iterFound->boneTag);
		obj->getOgreEntity()->getSkeleton()->freeTagPoint(iterFound->boneTag);
		m_listTrailObject.erase(iterFound);
	}

	void RibbonTrailObject::stopTrail(const String &entityObjName , const String &boneName)
	{
		return stopTrail(static_cast<EntityObject*>(m_scene->getSceneObject(entityObjName)) , boneName);
	}

	// ����������ʼ��ɫ
	void RibbonTrailObject::setInitialColour(size_t chainIndex, const ColourValue& col)
	{
		m_ribbonTrail->setInitialColour(chainIndex , col);
	}

	const ColourValue& RibbonTrailObject::getInitialColour(size_t chainIndex) const
	{
		return m_ribbonTrail->getInitialColour(chainIndex);
	}


	// ����������ɫÿ���ȥ����ɫ
	void RibbonTrailObject::setColourChange(size_t chainIndex, const ColourValue& valuePerSecond)
	{
		m_ribbonTrail->setColourChange(chainIndex , valuePerSecond);
	}

	const ColourValue& RibbonTrailObject::getColourChange(size_t chainIndex) const
	{
		return m_ribbonTrail->getColourChange(chainIndex);
	}


	// ��������Ĭ�Ͽ��
	void RibbonTrailObject::setInitialWidth(size_t chainIndex, float width)
	{
		m_ribbonTrail->setInitialWidth(chainIndex , width);
	}

	float RibbonTrailObject::getInitialWidth(size_t chainIndex) const
	{
		return m_ribbonTrail->getInitialWidth(chainIndex);
	}

	// ��������ÿ���ȥ�Ŀ��
	void RibbonTrailObject::setWidthChange(size_t chainIndex, float widthDeltaPerSecond)
	{
		m_ribbonTrail->setInitialWidth(chainIndex , widthDeltaPerSecond);
	}

	float RibbonTrailObject::getWidthChange(size_t chainIndex) const
	{
		return m_ribbonTrail->getWidthChange(chainIndex);
	}

	// ���������ĳ���
	void RibbonTrailObject::setTrailLength(float len)
	{
		m_ribbonTrail->setTrailLength(len);
	}

	float RibbonTrailObject::getTrailLength(void) const
	{
		return m_ribbonTrail->getTrailLength();
	}

	// ���������Ĳ���
	void RibbonTrailObject::setMaterialName(const String &name)
	{
		m_ribbonTrail->setMaterialName(name);
	}

	const String &RibbonTrailObject::getMaterialName(void) const
	{
		return m_ribbonTrail->getMaterialName();
	}

};