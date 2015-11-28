#include "StdAfx.h"
#include "SceneSystem.h"
#include "Scene.h"
#include "SceneSerializer.h"
#include "TerrainObject.h"
#include "ETTerrain.h"

namespace Ogre
{

	SceneSystem::SceneSystem(void)
		: m_sceneMgr(0)
		, m_collisionWorld(0)
		, m_triggerWorld(0)
	{
	}

	SceneSystem::~SceneSystem(void)
	{
		for(MapScene::iterator iter = m_mapScene.begin() ; iter != m_mapScene.end() ; iter ++)
		{
			delete iter->second;
		}
		m_mapScene.clear();
	}
	// ����ʵ�����ƺ��ļ�������һ������
	IScene* SceneSystem::loadScene(const String &name , const String &fileName)
	{
		IScene *scene = createScene(name);
		if(!scene)
		{
			return 0;
		}
		SceneSerializer serializer;
		if(!serializer.importScene(fileName , scene))
		{
			destroyScene(scene);
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
				"���볡��" + fileName + "ʧ��", 
				"SceneSystem::loadScene");
			return 0;
		}
		return scene;
	}
	// �����ļ�������һ������
	IScene* SceneSystem::loadScene(const String &fileName , IScene *scene)
	{
		if(scene == 0)
		{
			return loadScene(getUniqueName() , fileName);
		}
		else
		{
			SceneSerializer serializer;
			if(!serializer.importScene(fileName , scene))
			{
				OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
					"���볡��" + fileName + "ʧ��", 
					"SceneSystem::loadScene");
				return 0;
			}
			return scene;
		}
	}


	// �����ļ�������һ������
	bool SceneSystem::saveScene(IScene* scene , const String &fileName)
	{
		SceneSerializer serializer;
		if(!serializer.exportScene(scene , fileName))
		{
			OGRE_EXCEPT(Exception::ERR_CANNOT_WRITE_TO_FILE, 
				"���泡��" + fileName + "ʧ��", 
				"SceneSystem::saveScene");
			return false;
		}
		return true;
	}
	// ����ʵ�����ƺ��ļ�������һ������
	bool SceneSystem::saveScene(const String &name , const String &fileName)
	{
		IScene *scene = getScene(name);
		return saveScene(scene , fileName);
	}

	// ����һ������
	IScene* SceneSystem::createScene()
	{
		return createScene(getUniqueName());
	}

	// ���һ��ȫ��ΨһID
	uint32 SceneSystem::getUniqueID()
	{
		static uint32 s_index = 0;
		return ++s_index;
	}

	// ���һ��ȫ��Ψһ����
	String SceneSystem::getUniqueName()
	{
		String name;
		do 
		{
			name = "_scene_" + StringConverter::toString(getUniqueID());
		}while(hasScene(name));
		return name;
	}

	// ����һ������
	IScene* SceneSystem::createScene(const String &name)
	{
		if(hasScene(name))
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
				"��������Ϊ '" + name + "' �ĳ����Ѿ�����", 
				"SceneSystem::createScene");
			return 0;
		}
		IScene *scene = new Scene(m_sceneMgr , name , m_collisionWorld , m_triggerWorld);
		m_mapScene[name] = scene;
		return scene;
	}

	// ���ٳ���
	void SceneSystem::destroyScene(const String &name)
	{
		MapScene::iterator iter = m_mapScene.find(name);
		if(iter == m_mapScene.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"��������Ϊ '" + name + "' �ĳ���������", 
				"SceneSystem::destroyScene");
			return;
		}
		delete (iter->second);
		m_mapScene.erase(iter);
	}
	
	void SceneSystem::destroyScene(IScene *scene)
	{
		if(scene == 0)
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"����ָ��Ϊ��", 
				"SceneSystem::destroyScene");
			return;
		}
		MapScene::iterator iter = m_mapScene.find(scene->getName());
		if(iter == m_mapScene.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"��������Ϊ '" + scene->getName() + "' �ĳ���������", 
				"SceneSystem::destroyScene");
			return;
		}
		delete (iter->second);
		m_mapScene.erase(iter);
	}
	// ���һ������
	IScene *SceneSystem::getScene(const String &name)
	{
		MapScene::iterator iter = m_mapScene.find(name);
		if(iter == m_mapScene.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"��������Ϊ '" + name + "' �ĳ���������", 
				"SceneSystem::getScene");
			return 0;
		}
		return iter->second;
	}
	// �Ƿ����ָ�����ֵĳ���
	bool SceneSystem::hasScene(const String &name)const
	{
		return m_mapScene.find(name) != m_mapScene.end();
	}


	// Ϊһ��Viewport�����˶�ģ��
	void SceneSystem::setMotionBlurEnabled(Viewport *vp , bool enable)
	{
		// ��̬�����˶�ģ���Ļ����
		if(m_motionBlur.isNull() && enable)
		{
			// �˶�ģ����Ч
			m_motionBlur = CompositorManager::getSingleton().create(
				"Motion Blur", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
				);
			{
				CompositionTechnique *t = m_motionBlur->createTechnique();
				{
					CompositionTechnique::TextureDefinition *def = t->createTextureDefinition("scene");
					def->width = 0;
					def->height = 0;
					def->format = PF_R8G8B8;
				}
				{
					CompositionTechnique::TextureDefinition *def = t->createTextureDefinition("sum");
					def->width = 0;
					def->height = 0;
					def->format = PF_R8G8B8;
				}
				{
					CompositionTechnique::TextureDefinition *def = t->createTextureDefinition("temp");
					def->width = 0;
					def->height = 0;
					def->format = PF_R8G8B8;
				}
				// ��Ⱦ����
				{
					CompositionTargetPass *tp = t->createTargetPass();
					tp->setInputMode(CompositionTargetPass::IM_PREVIOUS);
					tp->setOutputName("scene");
				}
				// Ϊ�ۻ���ͼ��ʼ��
				{
					CompositionTargetPass *tp = t->createTargetPass();
					tp->setInputMode(CompositionTargetPass::IM_PREVIOUS);
					tp->setOutputName("sum");
					tp->setOnlyInitial(true);
				}
				// �˶�ģ��
				{
					CompositionTargetPass *tp = t->createTargetPass();
					tp->setInputMode(CompositionTargetPass::IM_NONE);
					tp->setOutputName("temp");
					{ CompositionPass *pass = tp->createPass();
					pass->setType(CompositionPass::PT_RENDERQUAD);
					pass->setMaterialName("Compositor/Combine");
					pass->setInput(0, "scene");
					pass->setInput(1, "sum");
					}
				}
				// ����back����
				{
					CompositionTargetPass *tp = t->createTargetPass();
					tp->setInputMode(CompositionTargetPass::IM_NONE);
					tp->setOutputName("sum");
					{ CompositionPass *pass = tp->createPass();
					pass->setType(CompositionPass::PT_RENDERQUAD);
					pass->setMaterialName("Compositor/Copyback");
					pass->setInput(0, "temp");
					}
				}
				// ��ʾ���
				{
					CompositionTargetPass *tp = t->getOutputTargetPass();
					tp->setInputMode(CompositionTargetPass::IM_NONE);
					{ CompositionPass *pass = tp->createPass();
					pass->setType(CompositionPass::PT_RENDERQUAD);
					pass->setMaterialName("Compositor/MotionBlur");
					pass->setInput(0, "sum");
					}
				}
			}
		}
		// ��������ģ��
		if(!m_motionBlur.isNull())
		{
			setCompositorEnabled(vp , m_motionBlur->getName() , enable);
		}
	}
	// ȷ��һ��������Ѿ���ӵ�vp��
	void SceneSystem::ensureCompositorAdd(Viewport *vp , const String &compositor)
	{
		bool added = false;
		CompositorChain *chain = CompositorManager::getSingleton().getCompositorChain(vp);
		if(chain)
		{
			for(size_t i = 0 ; i < chain->getNumCompositors() ; i ++)
			{
				if(chain->getCompositor(i)->getCompositor()->getName() == compositor)
				{
					added = true;
					break;
				}
			}
		}
		if(!added)
		{
			// ��ӵ������
			CompositorManager::getSingleton().addCompositor(vp, m_motionBlur->getName());
			// Ĭ�Ͻ�ֹ
			CompositorManager::getSingleton().setCompositorEnabled(vp, m_motionBlur->getName(), false);
		}
	}

	// ��vp����������ĳ��compositor
	void SceneSystem::setCompositorEnabled(Viewport *vp , const String &compositor , bool enable)
	{
		ensureCompositorAdd(vp , compositor);
		CompositorManager::getSingleton().setCompositorEnabled(vp, compositor, enable);
	}


}