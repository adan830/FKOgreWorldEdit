#include "StdAfx.h"
#include "WorldEditor.h"
#include "PluginManager.h"
#include "SettingManager.h"
#include "MapManager.h"
#include "ClipboardManager.h"

namespace Ogre
{
	// ����ģʽʵ��
	template<> WorldEditor* Singleton<WorldEditor>::ms_Singleton = 0;
	WorldEditor* WorldEditor::getSingletonPtr(void)
	{
		return ms_Singleton;
	}
	WorldEditor& WorldEditor::getSingleton(void)
	{  
		assert( ms_Singleton );  return ( *ms_Singleton );  
	}


	WorldEditor::WorldEditor(IMainFrame *mainFrm)
		: m_pluginManager(0)
		, m_inited(false)
		, m_displaySystem(0)
		, m_mainFrm(mainFrm)
		, m_settingManager(0)
		, m_mapManager(0)
		, m_clipboardManager(0)
	{
	}

	WorldEditor::~WorldEditor(void)
	{
	}

	// ��ʼ���༭��ϵͳ
	void WorldEditor::init(HWND hWnd)
	{
		if(m_inited)
			return;

		// ������ù�����
		m_settingManager = new SettingManager;
		// ������ʾϵͳ
		m_displaySystem = new DisplaySystem;
		// �ڵ�һ��Paint��ʱ��ʹ���Ogreϵͳ
		if(!m_displaySystem->isInited())
		{
			Ogre::NameValuePairList params;
			params["WINDOW"] = Ogre::StringConverter::toString((long)hWnd);
			m_displaySystem->init(&params);
			// ���óɻ�ɫ����
			m_displaySystem->getMainViewport()->setBackgroundColour(ColourValue(0.5,0.5,0.5));
			m_displaySystem->getMainCamera()->setPosition(Vector3(0 , 100 , -100));
			m_displaySystem->getMainCamera()->lookAt(Vector3::ZERO);
		}
		// ������ͼ������
		m_mapManager = new MapManager;
		// ����һ���³���
		m_mapManager->newScene();
		// �������а������
		m_clipboardManager = new ClipboardManager;
		// �������ϵͳ
		m_pluginManager = new PluginManager;
		// �����Ŀ¼�����в��
		m_pluginManager->loadPlugins(SettingManager::getSingleton().getString("���·��" , ".\\WorldEditorPlugins" , "���"));


		// ����һ��ƽ�й�
		Light *light = getMapManager()->getScene()->getOgreSceneManager()->createLight("TerrainLight");
		light->setType(Light::LT_DIRECTIONAL);
		light->setDirection(Vector3(1 , -1 , -1).normalisedCopy());
		light->setDiffuseColour(ColourValue(0.8,0.8,0.8));
		light->setVisible(true);
		light->setCastShadows(true);
		getMapManager()->getScene()->getOgreSceneManager()->setAmbientLight(ColourValue(0.5,0.5,0.5));

		m_inited = true;
	}

	// ���ٱ༭��ϵͳ
	void WorldEditor::shutdown()
	{
		if(!m_inited)
			return;

		// ����ƽ�й�
		getMapManager()->getScene()->getOgreSceneManager()->destroyLight("TerrainLight");

		m_pluginManager->_notifyRenderTargetRemove(m_displaySystem->getMainWindow());
		// ж�����в��
		m_pluginManager->unloadAllPlugins();
		OGRE_DELETE(m_pluginManager);
		OGRE_DELETE(m_mapManager);
		OGRE_DELETE(m_clipboardManager);

		// ������ʾ��
		m_displaySystem->shutdown();
		OGRE_DELETE(m_displaySystem);

		// �������ù�����
		OGRE_DELETE(m_settingManager);

		m_inited = false;
	}

	// �����ʾ��
	DisplaySystem *WorldEditor::getDisplaySystem()
	{
		return DisplaySystem::getSingletonPtr();
	}

	// ��ò��������
	IPluginManager *WorldEditor::getPluginManager()
	{
		return m_pluginManager;
	}

	// ���������
	IMainFrame *WorldEditor::getMainFrame()
	{
		return m_mainFrm;
	}
	// ������ù�����
	ISettingManager *WorldEditor::getSettingManager()
	{
		return m_settingManager;
	}
	// ��õ�ͼ������
	IMapManager *WorldEditor::getMapManager()
	{
		return m_mapManager;
	}
	// ��ü��а������
	IClipboardManager *WorldEditor::getClipboardManager()
	{
		return m_clipboardManager;
	}

};