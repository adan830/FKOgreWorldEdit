// ============================================================
// �ļ���	:	DisplaySystem.h
// ������	:	������
// ˵��		:	��ʾ��ϵͳ��ʵ��
// ����ʱ��	:	2008-1-20 20:51:58
// ============================================================

#pragma once
namespace Ogre
{
	class IScene;
	class CameraObject;
	class ICollisionSystem;
	class ICollisionWorld;
	// ��ʾ��ϵͳ
	class DISPLAYLAYER_API DisplaySystem : public Singleton<DisplaySystem>
	{
	public:
		DisplaySystem(void);
		virtual ~DisplaySystem(void);
		// ����ģʽ�ӿ�
		static DisplaySystem& getSingleton(void);
		static DisplaySystem* getSingletonPtr(void);

		// ��ʼ����ʾ��ϵͳ
		// params�Ǵ���ʱ�Ĳ��������ò�������
		// ������WINDOW��ֵ��HWND���ַ���ֵ����ʾ����Ogre���������HWNDָ���Ĵ����ϣ�Ĭ����Ogre�Զ�����
		// ������TITLE��ֵ���ַ�������ʾ������Ogre���ڵı��⣬Ĭ��ֵ��"û���ñ���"
		virtual bool init(NameValuePairList *params = 0);
		// ������ʾ��ϵͳ
		virtual void shutdown();

		// �Ƿ��Ѿ���ʼ��
		virtual bool isInited()const
		{
			return m_inited;
		}

		// ��ʼ��Ⱦ
		virtual void startRendering();

		// ��Ⱦһ֡
		virtual void renderOneFrame();

		// Ogreϵͳ
		Root *getRoot()const
		{
			return m_root;
		}

		// �������
		Camera* getMainCamera()const;
		// �������
		CameraObject* getMainCameraObject()const
		{
			return m_mainCamera;
		}
		// ������������
		SceneManager* getMainSceneMgr()const
		{
			return m_mainSceneMgr;
		}
		// ������
		IScene* getMainScene()const
		{
			return m_mainScene;
		}
		// ����Ⱦ����
		RenderWindow* getMainWindow()const
		{
			return m_mainWindow;
		}
		// ���ӿ�
		Viewport* getMainViewport()const
		{
			return m_mainViewport;
		}

		// ��Դ·��
		const String &getResourcePath()const
		{
			return m_resourcePath;
		}

		// ��ʾ�����
		const NameValuePairList &getParams()const
		{
			return m_params;
		}

		// �����ײϵͳ
		ICollisionSystem *getCollisionSystem()const;

		// �����ײ����
		ICollisionWorld *getCollisionWorld()const
		{
			return m_collisionWorld;
		}

		// ���һ����ɫ����
		const String &DisplaySystem::getColorMaterial(const ColourValue &color);
	protected:
		// ��������Ⱦ���ڣ�����ʼ��Ogre���ڼ�����m_mainWindow
		virtual bool createRenderWindow(void);
		// �����������������ڼ�����m_mainSceneMgr
		virtual void createSceneManager(void);
		// ������������ڼ�����m_mainCamera
		virtual void createCamera(void);
		// �����ӿڣ��ڼ�����m_mainViewport
		virtual void createViewports(void);
		// ������Դ·�����ڼ�����һϵ��ResourceGroupManager::getSingleton().addResourceLocation();
		virtual void setupResourcesLocation(void);
		// ������Դ������������������ʾ������Դ������
		virtual void createResourceListener(void);
		// ����������Դ���ڼ�����ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
		virtual void loadResources(void);

		// ϵͳ�Ƿ��Ѿ�����
		bool m_inited;
		// Ogreϵͳ
		Root *m_root;

		// �������
		CameraObject* m_mainCamera;
		// ������
		SceneManager* m_mainSceneMgr;
		// ����Ⱦ����
		RenderWindow* m_mainWindow;
		// ���ӿ�
		Viewport* m_mainViewport;

		// ������
		IScene *m_mainScene;

		// ��ײ����
		ICollisionWorld *m_collisionWorld;

		// ����������
		ITriggerWorld *m_triggerWorld;

		// ��Դ·��
		String m_resourcePath;

		// ��ʾ�����
		NameValuePairList m_params;
	};
};