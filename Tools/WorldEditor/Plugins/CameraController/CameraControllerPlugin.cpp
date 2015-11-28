#include "StdAfx.h"
#include "CameraControllerPlugin.h"

namespace Ogre
{
	// ����ģʽʵ��
	template<> CameraControllerPlugin* Singleton<CameraControllerPlugin>::ms_Singleton = 0;
	CameraControllerPlugin* CameraControllerPlugin::getSingletonPtr(void)
	{
		return ms_Singleton;
	}
	CameraControllerPlugin& CameraControllerPlugin::getSingleton(void)
	{  
		assert( ms_Singleton );  return ( *ms_Singleton );  
	}

	CameraControllerPlugin::CameraControllerPlugin(HINSTANCE hInstance)
		: m_inited(false)
		, m_activated(false)
	{
		m_description.name = "��������������";
		TCHAR szFileName[MAX_PATH];
		::GetModuleFileName(hInstance , szFileName , MAX_PATH);
		String outPath;
		StringUtil::splitFilename(szFileName , m_description.fileName , outPath);
		m_description.desc = "��������������������";
		m_description.publish = "������ 2008-3-7";
		m_description.version = 1;
	}

	CameraControllerPlugin::~CameraControllerPlugin(void)
	{
	}


	// ��ʼ���������������worldEditor������༭���Ľӿ�
	bool CameraControllerPlugin::init(IWorldEditor *worldEditor)
	{
		if(m_inited)
			return true;
		m_worldEditor = worldEditor;

		m_worldEditor->getMainFrame()->addChildViewListener(this , false);
		activate();
		m_inited = true;
		return m_inited;
	}

	// ��������Ϣ
	bool CameraControllerPlugin::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		switch(message)
		{
		case WM_MOUSEMOVE:
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
			// �����ALT����������������
			if(GetAsyncKeyState(VK_MENU))
			{
				return false;
			}
			break;
		}
		return true;
	}


	// �رղ��
	void CameraControllerPlugin::shutdown()
	{
		if(!m_inited)
			return;
		m_worldEditor->getMainFrame()->removeChildViewListener(this);
		m_inited = false;
	}

	// �Ƿ��Ѿ���ʼ��
	bool CameraControllerPlugin::isInited()const
	{
		return m_inited;
	}

	// ������
	void CameraControllerPlugin::activate()
	{
		if(m_activated)
			return;
		m_activated = true;
	}

	// ��������
	void CameraControllerPlugin::deactivate()
	{
		if(!m_activated)
			return;
		m_activated = false;
	}

	// �Ƿ��Ѿ�����
	bool CameraControllerPlugin::isActivate()const
	{
		return m_activated;
	}

	// ��ò������
	const IPlugin::Description &CameraControllerPlugin::getDescription()const
	{
		return m_description;
	}

	// ��ѯһЩ���ݣ�Ȼ�󷵻ص�result��
	bool CameraControllerPlugin::query(const String &name , const NameValuePairList *params , void *result)
	{
		return false;
	}

	// �ӿڸ���
	void CameraControllerPlugin::preViewportUpdate(const RenderTargetViewportEvent& evt)
	{

	}

	// �ӿڸ���
	void CameraControllerPlugin::postViewportUpdate(const RenderTargetViewportEvent& evt)
	{

	}

	// ����ƶ�
	bool CameraControllerPlugin::onMouseMove(IChildView *view , const CPoint &moveDistance , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		// �����ALT���Ϳ�ʼ���������
		if(GetAsyncKeyState(VK_MENU))
		{
			// �����м��
			switch(wParam)
			{
			case MK_MBUTTON:
				{
					moveCameraRel(view , moveDistance.x * 0.1f , -moveDistance.y * 0.1f , 0);
					return false;
				}
				break;
			case MK_RBUTTON:
				{
					moveCameraRel(view , 0 , 0 , -moveDistance.y * 0.1f + moveDistance.x * 0.1f);
					return false;
				}
				break;
			case MK_LBUTTON:
				{
					Vector3 center;
					//if(m_worldEditor->getMapManager()->getSelectedObjectsAvgCenter(center))
					//{
					//	rotateCameraAroundPosition(view , center , Degree(moveDistance.x * 0.5) , Degree(moveDistance.y * 0.5));
					//}
					//else
					{
						rotateCameraAroundViewportCenter(view , Degree(moveDistance.x * 0.5) , Degree(moveDistance.y * 0.5));
					}
					return false;
				}
				break;
			};
		}
		return true;
	}

	// �ƶ������
	void CameraControllerPlugin::moveCameraRel(IChildView *view , float x , float y , float z)
	{
		Camera *camera = view->getCamera();
		// ����ͶӰ���⴦��
		if(camera->getProjectionType() == PT_ORTHOGRAPHIC)
		{
			camera->moveRelative(Vector3(x , y , 0));
			if(camera->getOrthoWindowWidth() + z > 0
				&& camera->getOrthoWindowHeight() + z > 0)
			{
				camera->setOrthoWindow(camera->getOrthoWindowWidth() + z
					, camera->getOrthoWindowHeight() + z
					);
			}
		}
		else
		{
			Vector3 rel(x , y , z);
			rel = view->getCamera()->getDerivedOrientation() * rel;
			view->getMoveCameraSceneNode()->translate(rel);
		}
	}
	// ��ת�����������ĳ����ת
	void CameraControllerPlugin::rotateCameraAroundPosition(IChildView *view , const Vector3 &aroundPosition , const Radian &x , const Radian &y)
	{
		// ����ͶӰ��������ת
		if(view->getCamera()->getProjectionType() == PT_ORTHOGRAPHIC)
		{
			return;
		}
		SceneNode *rotateNode = view->getRotateCameraSceneNode();
		SceneNode *moveNode = view->getMoveCameraSceneNode();

		Vector3 posCamera = (rotateNode->getWorldPosition() - aroundPosition);

		moveNode->setPosition(aroundPosition);
		rotateNode->setPosition(posCamera);

		rotateCameraAroundViewportCenter(view , x , y);


	}

	// ��ת����������ӿ�������ת
	void CameraControllerPlugin::rotateCameraAroundViewportCenter(IChildView *view , const Radian &x , const Radian &y)
	{
		Camera *camera = view->getCamera();
		// ����ͶӰ��������ת
		if(camera->getProjectionType() == PT_ORTHOGRAPHIC)
		{
			return;
		}
		SceneNode *rotateNode = view->getRotateCameraSceneNode();
		camera->yaw(x);
		camera->pitch(y);
	}

};