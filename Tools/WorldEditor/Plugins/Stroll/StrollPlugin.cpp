#include "StdAfx.h"
#include "StrollPlugin.h"

namespace Ogre
{
	// ����ģʽʵ��
	template<> StrollPlugin* Singleton<StrollPlugin>::ms_Singleton = 0;
	StrollPlugin* StrollPlugin::getSingletonPtr(void)
	{
		return ms_Singleton;
	}
	StrollPlugin& StrollPlugin::getSingleton(void)
	{  
		assert( ms_Singleton );  return ( *ms_Singleton );  
	}

	StrollPlugin::StrollPlugin(HINSTANCE hInstance)
		: m_inited(false)
		, m_activated(false)
		, m_character(0)
		, m_camera(0)
		, m_state(0)
		, m_autoPlaceFloor(true)
	{
		m_description.name = "�����������";
		TCHAR szFileName[MAX_PATH];
		::GetModuleFileName(hInstance , szFileName , MAX_PATH);
		String outPath;
		StringUtil::splitFilename(szFileName , m_description.fileName , outPath);
		m_description.desc = "�Ե����˳���������";
		m_description.publish = "������ 2008-3-14";
		m_description.version = 1;
	}

	StrollPlugin::~StrollPlugin(void)
	{
	}


	// ��ʼ���������������worldEditor������༭���Ľӿ�
	bool StrollPlugin::init(IWorldEditor *worldEditor)
	{
		if(m_inited)
			return true;
		m_worldEditor = worldEditor;

		m_worldEditor->getMainFrame()->createPluginToolBarButton("�������" , this);

		m_activated = true;
		deactivate();
		m_inited = true;
		return m_inited;
	}

	// �رղ��
	void StrollPlugin::shutdown()
	{
		if(!m_inited)
			return;
		deactivate();
		m_worldEditor->getMainFrame()->destroyPluginToolBarButton("�������");

		m_inited = false;
	}

	// �Ƿ��Ѿ���ʼ��
	bool StrollPlugin::isInited()const
	{
		return m_inited;
	}

	// ������
	void StrollPlugin::activate()
	{
		if(m_activated)
			return;

		NameValuePairList params;
		params["mesh"] = "R_F_Z_Head_01.mesh";
		// ����һ���������
		m_character = static_cast<CharacterObject*>(DisplaySystem::getSingleton().getMainScene()->getRootObject()->createChildObject("CharacterObject" , &params));
		// ����������������˵ĺ���
		m_camera = static_cast<CameraObject*>(DisplaySystem::getSingleton().getMainCameraObject()->clone("CameraObject"));
		m_camera->removeFromParentObject();
		m_character->addChildObject(m_camera);
		m_camera->getOgreSceneNode()->setFixedYawAxis(true);
		// �ŵ�������ҵĺ��Ϸ������ҿ������ǰ��
		m_camera->setPosition(Vector3(0 , 3 , 10));
		m_camera->lookAt(Vector3(0 , 2 , 0));

		// ע���������
		m_character->setCallback(this);
		// ����������
		m_character->setEnableTrigger(true);

		IMainFrame::ChildViewIterator iter = m_worldEditor->getMainFrame()->getChildViewIterator();
		while(iter.hasMoreElements())
		{
			IChildView *view = iter.getNext();
			// ��͸����ͼ����������óɵ�ǰ�����
			if(view->getCameraType() == CT_PERSPECTIVE)
			{
				m_character->setWorldPosition(view->getMoveCameraSceneNode()->getWorldPosition() * Vector3(1 , 0 , 1));
				view->getViewport()->setCamera(m_camera->getOgreCamera());
				break;
			}
		}
		// ��ʼ����
		m_worldEditor->getMainFrame()->addChildViewListener(this);
		Root::getSingleton().addFrameListener(this);

		m_worldEditor->getMapManager()->addListener(this);

		m_activated = true;
	}

	// ��������
	void StrollPlugin::deactivate()
	{
		if(!m_activated)
			return;

		// ֹͣ����
		m_worldEditor->getMapManager()->removeListener(this);
		Root::getSingleton().removeFrameListener(this);
		m_worldEditor->getMainFrame()->removeChildViewListener(this);

		// �ָ������
		IMainFrame::ChildViewIterator iter = m_worldEditor->getMainFrame()->getChildViewIterator();
		while(iter.hasMoreElements())
		{
			IChildView *view = iter.getNext();
			// ��͸����ͼ����������ó�ԭ���������
			if(view->getCameraType() == CT_PERSPECTIVE)
			{
				view->getViewport()->setCamera(view->getCamera());
				break;
			}
		}

		// ���������
		if(m_camera)
		{
			DisplaySystem::getSingleton().getMainScene()->destroySceneObject(m_camera);
			m_camera = 0;
		}
		// ������Ҷ���
		if(m_character)
		{
			DisplaySystem::getSingleton().getMainScene()->destroySceneObject(m_character);
			m_character = 0;
		}
		m_state = 0;

		m_activated = false;
	}

	// �Ƿ��Ѿ�����
	bool StrollPlugin::isActivate()const
	{
		return m_activated;
	}

	// ��ò������
	const IPlugin::Description &StrollPlugin::getDescription()const
	{
		return m_description;
	}

	// ��ѯһЩ���ݣ�Ȼ�󷵻ص�result��
	bool StrollPlugin::query(const String &name , const NameValuePairList *params , void *result)
	{
		return false;
	}

	// ��������Ϣ
	bool StrollPlugin::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		switch(message)
		{
		case WM_KEYDOWN:
			switch(wParam)
			{
			case 'W':
				m_state |= CS_Forward;
				m_character->run(5);
				break;
			case 'S':
				m_state |= CS_Backward;
				m_character->walk(-3.0f);
				break;
			case 'A':
				m_state |= CS_TurnLeft;
				break;
			case 'D':
				m_state |= CS_TurnRight;
				break;
			}
			break;
		case WM_KEYUP:
			switch(wParam)
			{
			case 'W':
				m_state &= ~CS_Forward;
				m_character->stand();
				break;
			case 'S':
				m_state &= ~CS_Backward;
				m_character->stand();
				break;
			case 'A':
				m_state &= ~CS_TurnLeft;
				break;
			case 'D':
				m_state &= ~CS_TurnRight;
				break;
			}
			break;
		}
		return true;
	}

	bool StrollPlugin::frameStarted( const FrameEvent& evt )
	{
		if(m_character)
		{
			// ǰ��
			if(m_state & CS_Forward)
			{
			}
			// �˺�
			if(m_state & CS_Backward)
			{
			}

			// ��ת
			if(m_state & CS_TurnLeft)
			{
				m_character->turn(MathEx::EularToQuaternion(Vector3(0 
					, Degree(45).valueRadians() * evt.timeSinceLastFrame , 0)));
			}

			// ��ת
			if(m_state & CS_TurnRight)
			{
				m_character->turn(MathEx::EularToQuaternion(Vector3(0 
					, - Degree(45).valueRadians() * evt.timeSinceLastFrame , 0)));
			}
		}
		return true;
	}


	// �ӿڸ���
	void StrollPlugin::preViewportUpdate(const RenderTargetViewportEvent& evt)
	{
		if(m_character && m_autoPlaceFloor)
		{
			Vector3 pos = m_character->getWorldPosition();
			RayCheckInfo info;
			Ray ray(pos + Vector3(0 , 1 , 0) , Vector3::NEGATIVE_UNIT_Y);
			if(m_character->getScene()->collide(Ray(pos + Vector3(0 , 1 , 0) , Vector3::NEGATIVE_UNIT_Y) , true , true , info))
			{
				if(info.obj->getType() == BoxTriggerObjectFactory::FACTORY_TYPE 
					|| info.obj->getType() == SphereTriggerObjectFactory::FACTORY_TYPE)
				{
					return;
				}
				m_character->setWorldPosition(ray.getPoint(info.info.distance));
			}
		}
	}

	// �ӿڸ���
	void StrollPlugin::postViewportUpdate(const RenderTargetViewportEvent& evt)
	{
	}

	// ������һ��������
	// @triggerObject ������Ĵ�����
	// @player ��������
	void StrollPlugin::onEnterTrigger(TriggerObject *triggerObject , CharacterObject *player)
	{
		String msg("���[" + player->getName() + "]������һ��������[" + triggerObject->getName() + "]\n");
		ConsoleDebugger::printStr(msg.c_str());
		//AfxMessageBox(msg.c_str());
	}

	// ��һ����������ȥ
	// @triggerObject ��Ҵ������������ȥ
	// @player ��ȥ�����
	void StrollPlugin::onLeaveTrigger(TriggerObject *triggerObject , CharacterObject *player)
	{
		String msg("���[" + player->getName() + "]��һ��������[" + triggerObject->getName() + "]��ȥ��\n");
		ConsoleDebugger::printStr(msg.c_str());
		//AfxMessageBox(msg.c_str());
	}

	// ���ٳ���
	void StrollPlugin::onDestroyScene(IScene *scene)
	{
		deactivate();
	}

};