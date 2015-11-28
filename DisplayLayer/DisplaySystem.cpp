#include "StdAfx.h"
#include "DisplaySystem.h"
#include "ISceneSystem.h"
#include "IScene.h"
#include "CameraObject.h"
#include "IDecalSystem.h"
#include "CollisionSystem.h"

namespace Ogre
{

	// ����ģʽʵ��
	template<> DisplaySystem* Singleton<DisplaySystem>::ms_Singleton = 0;
	DisplaySystem* DisplaySystem::getSingletonPtr(void)
	{
		return ms_Singleton;
	}
	DisplaySystem& DisplaySystem::getSingleton(void)
	{  
		assert( ms_Singleton );  return ( *ms_Singleton );  
	}

	DisplaySystem::DisplaySystem(void)
		: m_root(0)
		, m_inited(false)
		, m_mainCamera(0)
		, m_mainSceneMgr(0)
		, m_mainWindow(0)
		, m_mainViewport(0)
		, m_mainScene(0)
		, m_collisionWorld(0)
		, m_triggerWorld(0)
	{
		// Provide a nice cross platform solution for locating the configuration files
		// On windows files are searched for in the current working directory, on OS X however
		// you must provide the full path, the helper function macBundlePath does this for us.
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
		m_resourcePath = macBundlePath() + "/Contents/Resources/";
#else
		m_resourcePath = "";
#endif

	}

	DisplaySystem::~DisplaySystem(void)
	{
	}


	// ��ʼ����ʾ��ϵͳ
	bool DisplaySystem::init(NameValuePairList *params)
	{
		if(m_inited)
			return true;

		if(params)
		{
			m_params = *params;
		}

		// ��ʼ��Ĭ�ϲ���
		if(m_params["TITLE"].empty())
		{
			m_params["TITLE"] = "û���ñ���";
		}

		String pluginsPath;
		// only use plugins.cfg if not static
#ifndef OGRE_STATIC_LIB
		pluginsPath = m_resourcePath + "plugins.cfg";
#endif

		// ����Unicodeת���Ĵ���ҳ
		setlocale(LC_ALL , "");

		// ����Ogreϵͳ
		m_root = new Root(pluginsPath, 
			m_resourcePath + "ogre.cfg", m_resourcePath + "Ogre.log");

		// ������Դ·��
		setupResourcesLocation();

		// ���ô���
		if (!createRenderWindow())
		{
			return false;
		}

		// ѡ�񴴾�������
		createSceneManager();
		// ����Ĭ�����
		createCamera();
		// ����Ĭ���ӿ�
		createViewports();

		// Set default mipmap level (NB some APIs ignore this)
		TextureManager::getSingleton().setDefaultNumMipmaps(5);

		// Create any resource listeners (for loading screens)
		createResourceListener();
		// ����������Դ
		loadResources();

		// ��ʼ������ϵͳ
		IDecalSystem::init(m_mainSceneMgr);

		m_inited = true;

		return true;
	}
	// ������ʾ��ϵͳ
	void DisplaySystem::shutdown()
	{
		if(!m_inited)
			return;
		// �ر�����ϵͳ
		IDecalSystem::shutdown();

		ISceneSystem::getSingleton().destroyScene(m_mainScene);
		// �رճ���ϵͳ
		ISceneSystem::shutdown();

		// ������ײ����
		ICollisionSystem::getSingleton().destroyWorld(m_collisionWorld);
		m_collisionWorld = 0;
		// �ر���ײϵͳ
		ICollisionSystem::shutdown();

		// ���ٴ���������
		ITriggerSystem::getSingleton().destroyWorld(m_triggerWorld);
		m_triggerWorld = 0;
		// �رմ�����ϵͳ
		ITriggerSystem::shutdown();


		// ����Ogreϵͳ
		OGRE_DELETE(m_root);

		m_inited = false;
	}

	// ��ʼ��Ⱦ
	void DisplaySystem::startRendering()
	{
		m_root->startRendering();
	}
	// ��Ⱦһ֡
	void DisplaySystem::renderOneFrame()
	{
		m_root->renderOneFrame();
	}

	/** Configures the application - returns false if the user chooses to abandon configuration. */
	bool DisplaySystem::createRenderWindow(void)
	{
		// ������������ã������û�ѡ������
		if(!m_root->restoreConfig())
		{
			if(!m_root->showConfigDialog())
				return false;
		}


		if(!m_params["WINDOW"].empty())
		{
			// ��ʼ��Ogreϵͳ
			m_root->initialise(false);
			NameValuePairList misc;
			misc["externalWindowHandle"] = m_params["WINDOW"];

			HWND hWnd = (HWND)StringConverter::parseLong(misc["externalWindowHandle"]);
			RECT rect;
			::GetClientRect(hWnd , &rect);
			// ʹ���ⲿ���ڴ�����Ⱦ����
			m_mainWindow = m_root->createRenderWindow(m_params["TITLE"]
				, rect.right - rect.left
				, rect.bottom - rect.top
				, false
				, &misc);
		}
		else
		{
			// ��ʼ��Ogreϵͳ
			m_mainWindow = m_root->initialise(true , m_params["TITLE"]);
		}
		if(m_mainWindow == 0)
			return false;
		return true;
	}

	void DisplaySystem::createSceneManager(void)
	{
		// Create the SceneManager, in this case a generic one
		m_mainSceneMgr = m_root->createSceneManager(ST_GENERIC, "MainScene");
		// Set ambient light
		m_mainSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

		// ��ʼ����ײϵͳ
		ITriggerSystem::init();
		// ��ʼ��������ϵͳ
		ICollisionSystem::init(m_mainSceneMgr);
		// ����һ��Ĭ�ϵ���ײ����
		m_collisionWorld = ICollisionSystem::getSingleton().createWorld("DefaultCollisionWorld");
		// ����һ��Ĭ�ϵĴ���������
		m_triggerWorld = ITriggerSystem::getSingleton().createWorld("DefaultTriggerWorld");
		// ��ʼ������ϵͳ
		ISceneSystem::init(m_mainSceneMgr);
		ISceneSystem::getSingleton().setOgreSceneManager(m_mainSceneMgr);
		ISceneSystem::getSingleton().setCollisionWorld(m_collisionWorld);
		ISceneSystem::getSingleton().setTriggerWorld(m_triggerWorld);


		m_mainScene = ISceneSystem::getSingleton().createScene("MainScene");
	}

	void DisplaySystem::createCamera(void)
	{
		// Create the camera
		m_mainCamera = static_cast<CameraObject*>(m_mainScene->getRootObject()->createChildObject("CameraObject"));

		// Position it at 500 in Z direction
		m_mainCamera->setPosition(Vector3(0,100,-1));
		// Look back along -Z
		m_mainCamera->lookAt(Vector3(0,0,0));
		m_mainCamera->getOgreCamera()->setNearClipDistance(0.1);
		m_mainCamera->getOgreCamera()->setFarClipDistance(1000);

	}

	void DisplaySystem::createViewports(void)
	{
		// Create one viewport, entire window
		m_mainViewport = m_mainWindow->addViewport(m_mainCamera->getOgreCamera());
		m_mainViewport->setBackgroundColour(ColourValue(0,0,1));

		// Alter the camera aspect ratio to match the viewport
		m_mainCamera->getOgreCamera()->setAspectRatio(
			Real(m_mainViewport->getActualWidth()) / Real(m_mainViewport->getActualHeight()));
	}

	/// Method which will define the source of resources (other than current folder)
	void DisplaySystem::setupResourcesLocation(void)
	{
		// Load resource paths from config file
		ConfigFile cf;
		cf.load(m_resourcePath + "resources.cfg");

		// Go through all sections & settings in the file
		ConfigFile::SectionIterator seci = cf.getSectionIterator();

		String secName, typeName, archName;
		while (seci.hasMoreElements())
		{
			secName = seci.peekNextKey();
			ConfigFile::SettingsMultiMap *settings = seci.getNext();
			ConfigFile::SettingsMultiMap::iterator i;
			for (i = settings->begin(); i != settings->end(); ++i)
			{
				typeName = i->first;
				String path =  i->second;
				StringVector sv = StringUtil::split(path , "," , 1);
				bool recursive = false;
				archName = sv[0];
				// �����Ƿ���ѭ����Ŀ¼���ļ�ϵͳ
				if(sv.size() > 1)
				{
					recursive = (sv[1] == "recursive");
				}
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
				// OS X does not set the working directory relative to the app,
				// In order to make things portable on OS X we need to provide
				// the loading with it's own bundle path location
				ResourceGroupManager::getSingleton().addResourceLocation(
					String(macBundlePath() + "/" + archName), typeName, secName , recursive);
#else
				ResourceGroupManager::getSingleton().addResourceLocation(
					archName, typeName, secName , recursive);
#endif
			}
		}
	}

	/// Optional override method where you can create resource listeners (e.g. for loading screens)
	void DisplaySystem::createResourceListener(void)
	{

	}

	/// Optional override method where you can perform resource group loading
	/// Must at least do ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	void DisplaySystem::loadResources(void)
	{
		// Initialise, parse scripts etc
		ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	}

	// ���һ����ɫ����
	const String &DisplaySystem::getColorMaterial(const ColourValue &color)
	{
		String name("AutoColor_" + StringConverter::toString(color));
		if(MaterialManager::getSingleton().resourceExists(name))
		{
			return MaterialManager::getSingleton().getByName(name)->getName();
		}
		MaterialPtr mat = MaterialManager::getSingleton().create(name , ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		mat->getTechnique(0)->getPass(0)->setDiffuse(ColourValue::Black);
		mat->getTechnique(0)->getPass(0)->setSelfIllumination(color);
		mat->getTechnique(0)->getPass(0)->setAmbient(ColourValue::Black);
		return mat->getName();
	}
	// �������
	Camera* DisplaySystem::getMainCamera()const
	{
		return m_mainCamera->getOgreCamera();
	}

	// �����ײϵͳ
	ICollisionSystem *DisplaySystem::getCollisionSystem()const
	{
		return ICollisionSystem::getSingletonPtr();
	}
};