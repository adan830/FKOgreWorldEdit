#include "StdAfx.h"
#include "ObjectEditorPlugin.h"
#include "DlgPlaceSceneObject.h"
#include "DlgEditTools.h"
#include "DlgTrackTools.h"
#include "DlgObjectProperty.h"

#include "ISceneObjectVisible.h"

#include "ObjectSelectAction.h"
#include "ObjectDragAction.h"
#include "ObjectMoveAction.h"
#include "ObjectRotateAction.h"
#include "ObjectScaleAction.h"
#include "ObjectDeleteAction.h"
#include "ObjectCloneAction.h"
#include "ObjectPlaceFloorAction.h"

#include "TrackKeyFrameObject.h"

namespace Ogre
{
	// ����ģʽʵ��
	template<> ObjectEditorPlugin* Singleton<ObjectEditorPlugin>::ms_Singleton = 0;
	ObjectEditorPlugin* ObjectEditorPlugin::getSingletonPtr(void)
	{
		return ms_Singleton;
	}
	ObjectEditorPlugin& ObjectEditorPlugin::getSingleton(void)
	{  
		assert( ms_Singleton );  return ( *ms_Singleton );  
	}

	ObjectEditorPlugin::ObjectEditorPlugin(HINSTANCE hInstance)
		: m_inited(false)
		, m_activated(false)
		, m_objectAction(0)
	{
		m_description.name = "����༭���";
		TCHAR szFileName[MAX_PATH];
		::GetModuleFileName(hInstance , szFileName , MAX_PATH);
		String outPath;
		StringUtil::splitFilename(szFileName , m_description.fileName , outPath);
		m_description.desc = "��������İڷű༭";
		m_description.publish = "������ 2008-2-24";
		m_description.version = 1;

		m_editableSceneObjectType.push_back(NullObjectFactory::FACTORY_TYPE);
		m_editableSceneObjectType.push_back(EntityObjectFactory::FACTORY_TYPE);
		m_editableSceneObjectType.push_back(CameraObjectFactory::FACTORY_TYPE);
		m_editableSceneObjectType.push_back(ParticleObjectFactory::FACTORY_TYPE);
		m_editableSceneObjectType.push_back(EnvironmentObjectFactory::FACTORY_TYPE);
		m_editableSceneObjectType.push_back(BoxTriggerObjectFactory::FACTORY_TYPE);
		m_editableSceneObjectType.push_back(SphereTriggerObjectFactory::FACTORY_TYPE);
		m_editableSceneObjectType.push_back(AnimationTrackObjectFactory::FACTORY_TYPE);
	}

	ObjectEditorPlugin::~ObjectEditorPlugin(void)
	{
	}


	// ��ʼ���������������worldEditor������༭���Ľӿ�
	bool ObjectEditorPlugin::init(IWorldEditor *worldEditor)
	{
		if(m_inited)
			return true;
		m_worldEditor = worldEditor;

		m_worldEditor->getMainFrame()->addListener(this);


		CXTPDockingPane* paneInsert = m_worldEditor->getMainFrame()->createPane(DlgPlaceSceneObject::IDD , "����ڷ�" , CRect(0 , 0 , 230 , 230) , xtpPaneDockRight);
		CXTPDockingPane* paneTools = m_worldEditor->getMainFrame()->createPane(DlgEditTools::IDD , "���������" , CRect(0 , 0 , 230 , 230) , xtpPaneDockBottom , paneInsert);
		CXTPDockingPane* paneTrackTools = m_worldEditor->getMainFrame()->createPane(DlgTrackTools::IDD , "�켣�༭��" , CRect(0 , 0 , 230 , 230) , xtpPaneDockBottom , paneInsert);
		CXTPDockingPane* paneProperty = m_worldEditor->getMainFrame()->createPane(DlgObjectProperty::IDD , "�������" , CRect(0 , 0 , 230 , 230) , xtpPaneDockBottom , paneInsert);
		CXTPDockingPane* paneList = m_worldEditor->getMainFrame()->createPane(DlgObjectList::IDD , "����б�" , CRect(0 , 0 , 230 , 230) , xtpPaneDockRight , paneInsert);
		m_worldEditor->getMainFrame()->getDockingPaneManager()->AttachPane(paneList , paneInsert);
		paneInsert->SetFocus();

		getWorldEditor()->getMapManager()->addListener(&m_DlgObjectProperty);
		getWorldEditor()->getMapManager()->addListener(&m_DlgObjectList);
		getWorldEditor()->getMapManager()->addListener(this);
		// ��ӳ���������
		if(m_worldEditor->getMapManager()->getScene())
		{
			m_worldEditor->getMapManager()->getScene()->addListener(this);
		}

		m_worldEditor->getMainFrame()->createPluginToolBarButton("������" , this);

		// ��ӹ켣�ؼ�֡�Ķ��󹤳�
		ISceneSystem::getSingleton().addFactory(new TrackKeyFrameObjectFactory);

		m_activated = true;
		deactivate();
		m_inited = true;
		return m_inited;
	}

	// �رղ��
	void ObjectEditorPlugin::shutdown()
	{
		if(!m_inited)
			return;

		deactivate();
		m_worldEditor->getMainFrame()->destroyPluginToolBarButton("������");
		getWorldEditor()->getMapManager()->removeListener(&m_DlgObjectProperty);
		getWorldEditor()->getMapManager()->removeListener(&m_DlgObjectList);
		getWorldEditor()->getMapManager()->removeListener(this);
		m_worldEditor->getMainFrame()->removeListener(this);
		m_worldEditor->getMainFrame()->destroyPane(DlgPlaceSceneObject::IDD);
		m_worldEditor->getMainFrame()->destroyPane(DlgTrackTools::IDD);
		m_worldEditor->getMainFrame()->destroyPane(DlgEditTools::IDD);
		m_worldEditor->getMainFrame()->destroyPane(DlgObjectProperty::IDD);
		m_worldEditor->getMainFrame()->destroyPane(DlgObjectList::IDD);
		// ɾ������������
		if(m_worldEditor->getMapManager()->getScene())
		{
			m_worldEditor->getMapManager()->getScene()->removeListener(this);
		}

		// ɾ���켣�ؼ�֡�Ķ��󹤳�
		ISceneSystem::getSingleton().removeFactory(TrackKeyFrameObjectFactory::FACTORY_TYPE);

		m_inited = false;
	}


	// ���õ�ǰ�����������
	void ObjectEditorPlugin::setObjectAction(ObjectAction *objectAction)
	{
		OGRE_DELETE(m_objectAction);
		m_objectAction = objectAction;
	}

	// �Ƿ��Ѿ���ʼ��
	bool ObjectEditorPlugin::isInited()const
	{
		return m_inited;
	}

	// ������
	void ObjectEditorPlugin::activate()
	{
		if(m_activated)
			return;
		m_worldEditor->getMainFrame()->getDockingPaneManager()->ShowPane(DlgPlaceSceneObject::IDD);
		m_worldEditor->getMainFrame()->getDockingPaneManager()->ShowPane(DlgEditTools::IDD);
		m_worldEditor->getMainFrame()->getDockingPaneManager()->ShowPane(DlgObjectProperty::IDD);
		m_worldEditor->getMainFrame()->getDockingPaneManager()->ShowPane(DlgObjectList::IDD);
		m_worldEditor->getMainFrame()->addChildViewListener(this);
		m_activated = true;
	}

	// ��������
	void ObjectEditorPlugin::deactivate()
	{
		if(!m_activated)
			return;
		m_worldEditor->getMainFrame()->getDockingPaneManager()->ClosePane(DlgTrackTools::IDD);
		m_worldEditor->getMainFrame()->getDockingPaneManager()->ClosePane(DlgPlaceSceneObject::IDD);
		m_worldEditor->getMainFrame()->getDockingPaneManager()->ClosePane(DlgEditTools::IDD);
		m_worldEditor->getMainFrame()->getDockingPaneManager()->ClosePane(DlgObjectProperty::IDD);
		m_worldEditor->getMainFrame()->getDockingPaneManager()->ClosePane(DlgObjectList::IDD);
		m_DlgTrackTools.deactivate();
		OGRE_DELETE(m_objectAction);
		m_worldEditor->getMainFrame()->removeChildViewListener(this);
		m_activated = false;
	}

	// �Ƿ��Ѿ�����
	bool ObjectEditorPlugin::isActivate()const
	{
		return m_activated;
	}

	// ��ò������
	const IPlugin::Description &ObjectEditorPlugin::getDescription()const
	{
		return m_description;
	}

	// ��������Ϣ
	bool ObjectEditorPlugin::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		if(message == WM_KEYUP)
		{
			switch(wParam)
			{
			case 'S':
				{
					setObjectAction(0);
					setObjectAction(new ObjectSelectAction());
				}
				break;
			case 'D':
			case VK_DELETE:
				{
					ObjectDeleteAction action;
					action.execute();
				}
				break;
			case 'C':
				{
					ObjectCloneAction action;
					action.execute();
				}
				break;
			case 'T':
				{
					ObjectPlaceFloorAction action;
					action.execute();
				}
				break;
			case 'A':
				{
					setObjectAction(0);
					setObjectAction(new ObjectDragAction());
				}
				break;
			case 'M':
				{
					setObjectAction(0);
					setObjectAction(new ObjectMoveAction());
				}
				break;
			case 'R':
				{
					setObjectAction(0);
					setObjectAction(new ObjectRotateAction());
				}
				break;
			case 'L':
				{
					setObjectAction(0);
					setObjectAction(new ObjectScaleAction());
				}
				break;
			}
		}
		return true;
	}
	// ��ѯһЩ���ݣ�Ȼ�󷵻ص�result��
	bool ObjectEditorPlugin::query(const String &name , const NameValuePairList *params , void *result)
	{
		return false;
	}

	// ��������֪ͨ
	LRESULT ObjectEditorPlugin::onDockingPaneNotify(WPARAM wParam, LPARAM lParam)
	{
		if (wParam == XTP_DPN_SHOWWINDOW)
		{
			CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;

			if (!pPane->IsValid())
			{
				switch (pPane->GetID())
				{
				case DlgPlaceSceneObject::IDD:
					{
						if (m_PlaceSceneObject.GetSafeHwnd() == 0)
						{
							m_PlaceSceneObject.Create(DlgPlaceSceneObject::IDD, m_worldEditor->getMainFrame()->getWnd());
						}
						pPane->Attach(&m_PlaceSceneObject);
					}
					break;
				case DlgEditTools::IDD:
					{
						if (m_DlgEditTools.GetSafeHwnd() == 0)
						{
							m_DlgEditTools.Create(DlgEditTools::IDD, m_worldEditor->getMainFrame()->getWnd());
						}
						pPane->Attach(&m_DlgEditTools);
					}
					break;
				case DlgTrackTools::IDD:
					{
						if (m_DlgTrackTools.GetSafeHwnd() == 0)
						{
							m_DlgTrackTools.Create(DlgTrackTools::IDD, m_worldEditor->getMainFrame()->getWnd());
						}
						pPane->Attach(&m_DlgTrackTools);
					}
					break;
				case DlgObjectProperty::IDD:
					{
						if (m_DlgObjectProperty.GetSafeHwnd() == 0)
						{
							m_DlgObjectProperty.Create(DlgObjectProperty::IDD, m_worldEditor->getMainFrame()->getWnd());
						}
						pPane->Attach(&m_DlgObjectProperty);
					}
					break;
				case DlgObjectList::IDD:
					{
						if (m_DlgObjectList.GetSafeHwnd() == 0)
						{
							m_DlgObjectList.Create(DlgObjectList::IDD, m_worldEditor->getMainFrame()->getWnd());
						}
						pPane->Attach(&m_DlgObjectList);
					}
					break;
				default:
					return FALSE;
					break;
				}
			}
			return TRUE;
		}
		return FALSE;
	}

	// �ӿڸ���
	void ObjectEditorPlugin::preViewportUpdate(const RenderTargetViewportEvent& evt)
	{
	}


	// �Ƿ�֧�ִ�����Ķ�̬�༭
	bool ObjectEditorPlugin::isEditable(ISceneObject *obj)const
	{
		return std::find(m_editableSceneObjectType.begin() , m_editableSceneObjectType.end() , obj->getType()) != m_editableSceneObjectType.end()
			&& ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getScene()->hasSceneObject(obj->getName());
	}
	// ��һ��MO���һ���ɱ༭�ĳ������
	ISceneObject* ObjectEditorPlugin::getEditableSceneObject(MovableObject *obj)const
	{
		ISceneObject *sceneObject = obj_cast(obj->getUserAny());
		if(!sceneObject || !isEditable(sceneObject))
			return 0;
		if(sceneObject->getType() == CameraObjectFactory::FACTORY_TYPE && obj->getMovableType() == "Camera")
			return 0;
		return sceneObject;
	}

	// �½�����
	void ObjectEditorPlugin::onNewScene(IScene *scene)
	{
		scene->addListener(this);
	}
	// ����һ������
	void ObjectEditorPlugin::onPreLoadScene(const String &fileName ,IScene *scene)
	{
		scene->addListener(this);
	}
	// ���ٳ���
	void ObjectEditorPlugin::onDestroyScene(IScene *scene)
	{
		m_DlgTrackTools.deactivate();
		scene->removeListener(this);
		OGRE_DELETE(m_objectAction);
	};


	// ������ײ��⣬�����ص�һ��������ײ����Ϣ�����û�е��Σ�����ײ0ƽ�棩
	// @ray ����
	// @collideWorld �Ƿ��������ײ���磺���Σ�
	// @collideObject �Ƿ�������ײ
	// @intersection �������
	bool ObjectEditorPlugin::collide(const Ray& ray , bool collideWorld , bool collideObject , Vector3& intersection)
	{
		// ʹ�ó�����ײ���
		if(!m_worldEditor->getMapManager()->getScene()->collide(ray , true , false , intersection))
		{
			// ���û����ײ����������ʹ��0ƽ��������ײ
			std::pair<bool, Real> result = Math::intersects(ray , Plane(Vector3::UNIT_Y , 0));
			if(result.first)
			{
				intersection = ray.getPoint(result.second);
				return true;
			}
			else
			{
				// ���������ײ����������ײ����
				std::pair<bool, Real> result = Math::intersects(ray , Plane(Vector3::NEGATIVE_UNIT_Y , 0));
				if(result.first)
				{
					intersection = ray.getPoint(result.second);
					return true;
				}
				else
				{
					return false;
				}
			}
		}
		else
		{
			return true;
		}
	}


	// ������һ��������������
	void ObjectEditorPlugin::onCreateSceneObject(ISceneObject *obj)
	{
		if(isEditable(obj))
		{
			obj->getSceneObjectVisible()->showEntity(true);
		}
	}


	// ѡ��һ����������
	void ObjectEditorPlugin::onSelectObject(ISceneObject *obj)
	{
		//// �������Ѱ����һ�������켣�����򼤻�༭����켣
		//if(obj->getType() == AnimationTrackObjectFactory::FACTORY_TYPE)
		//{
		//	m_DlgTrackTools.activate(static_cast<AnimationTrackObject*>(obj));
		//}
	}
	// ��ѡһ����������
	void ObjectEditorPlugin::onDeselectObject(ISceneObject *obj)
	{
	}

};