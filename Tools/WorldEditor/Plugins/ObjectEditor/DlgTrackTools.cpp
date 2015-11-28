// DlgTrackTools.cpp : implementation file
//

#include "stdafx.h"
#include "DlgTrackTools.h"
#include "ObjectEditorPlugin.h"

#include "TrackSelectAction.h"
#include "TrackDragAction.h"
#include "TrackCloneAction.h"
#include "TrackMoveAction.h"
#include "TrackRotateAction.h"
#include "TrackScaleAction.h"
#include "TrackDeleteAction.h"
#include "TrackPlaceFloorAction.h"
#include "TrackNewAction.h"

#include "TrackKeyFrameObject.h"
#include "AnimationTrackObjectVisible.h"

// DlgTrackTools dialog

IMPLEMENT_DYNAMIC(DlgTrackTools, CDialog)

DlgTrackTools::DlgTrackTools(CWnd* pParent /*=NULL*/)
	: CDialog(DlgTrackTools::IDD, pParent)
	, m_object(0)
	, m_simulateObject(0)
	, m_keyObjectRoot(0)
{

}

DlgTrackTools::~DlgTrackTools()
{
}

void DlgTrackTools::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgTrackTools, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_NEW, &DlgTrackTools::OnBnClickedButtonNew)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &DlgTrackTools::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &DlgTrackTools::OnBnClickedButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_DELETE3, &DlgTrackTools::OnBnClickedButtonDelete3)
	ON_BN_CLICKED(IDC_BUTTON_CLONE2, &DlgTrackTools::OnBnClickedButtonClone2)
	ON_BN_CLICKED(IDC_BUTTON_PLACEFLOOR2, &DlgTrackTools::OnBnClickedButtonPlacefloor2)
	ON_BN_CLICKED(IDC_BUTTON_DRAG, &DlgTrackTools::OnBnClickedButtonDrag)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_X, &DlgTrackTools::OnBnClickedButtonMoveX)
	ON_BN_CLICKED(IDC_BUTTON_ROTATE_X2, &DlgTrackTools::OnBnClickedButtonRotateX2)
	ON_BN_CLICKED(IDC_BUTTON_SCALE_X2, &DlgTrackTools::OnBnClickedButtonScaleX2)
	ON_BN_CLICKED(IDC_BUTTON_SIMULATE, &DlgTrackTools::OnBnClickedButtonSimulate)
END_MESSAGE_MAP()


// DlgTrackTools message handlers

void DlgTrackTools::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(!bShow)
	{
		deactivate();
	}
}


// ����༭һ�������켣����
void DlgTrackTools::activate(AnimationTrackObject *obj)
{
	if(isActivate())
		return;
	deactivate();
	m_object = obj;

	// �����ؼ���ĸ�
	m_keyObjectRoot = DisplaySystem::getSingleton().getMainScene()->getRootObject()->createChildObject("NullObject");
	size_t index = 0;
	AnimationTrackObject::ConstKeyFrameIterator iter = m_object->getConstKeyFrameIterator();
	// �������йؼ���
	while(iter.hasMoreElements())
	{
		const AnimationTrackObject::KeyFrame &keyFrame = iter.getNext();
		KeyObjectData data = createKeyObject(index++);
		m_listKeyObject.push_back(data.m_keyObject);
	}
	// �����������йؼ�֡����
	rebuildKeyFrameIndex();

	ObjectEditorPlugin &plugin = ObjectEditorPlugin::getSingleton();

	// ���Action
	plugin.setObjectAction(0);

	// ���û�йؼ�֡�����л����½��ؼ�֡����ģʽ
	if(m_object->getKeyFrameCount() == 0)
	{
		plugin.setObjectAction(0);
		plugin.setObjectAction(new TrackNewAction(this));
	}

	// ��ʼ����������Ϣ
	plugin.getWorldEditor()->getMainFrame()->addChildViewListener(this , false);

	// ��ʾ�߿�
	static_cast<AnimationTrackObjectVisible*>(m_object->getSceneObjectVisible())->showBorder(true);
	// ���عؼ�֡�ڵ�
	static_cast<AnimationTrackObjectVisible*>(m_object->getSceneObjectVisible())->showKeyNode(false);

	// �ر�����༭��
	plugin.getWorldEditor()->getMainFrame()->getDockingPaneManager()->ShowPane(DlgTrackTools::IDD);
	plugin.getWorldEditor()->getMainFrame()->getDockingPaneManager()->ClosePane(DlgEditTools::IDD);
	plugin.getWorldEditor()->getMainFrame()->getDockingPaneManager()->ClosePane(DlgPlaceSceneObject::IDD);
	plugin.getWorldEditor()->getMainFrame()->getDockingPaneManager()->ClosePane(DlgObjectList::IDD);

}

// ������
void DlgTrackTools::deactivate()
{
	if(m_object == 0)
		return;
	ObjectEditorPlugin &plugin = ObjectEditorPlugin::getSingleton();
	// ���ٹ켣�ؼ������
	for(ListKeyObject::iterator iter = m_listKeyObject.begin() ; iter != m_listKeyObject.end() ; iter ++)
	{
		plugin.getWorldEditor()->getMapManager()->deselectObject(*iter);
		m_keyObjectRoot->destroyChildObject(*iter);
	}
	m_listKeyObject.clear();
	
	// ���ٹؼ������ĸ��ڵ�
	m_keyObjectRoot->getScene()->destroySceneObject(m_keyObjectRoot);
	m_keyObjectRoot = 0;
	// ���ó�ѡ��ģʽ
	plugin.setObjectAction(0);
	plugin.setObjectAction(new ObjectSelectAction());
	// ֹͣ����������Ϣ
	plugin.getWorldEditor()->getMainFrame()->removeChildViewListener(this);

	// ����ģ�����
	if(m_simulateObject)
	{
		m_object->destroyTrack(m_controller);
		m_controller = 0;
		m_simulateObject->getScene()->destroySceneObject(m_simulateObject);
		m_simulateObject = 0;
	}
	// ���ù켣���������λ��Ϊ�ؼ�֡�Ŀ�ʼ֡
	if(m_object->getKeyFrameCount() > 0)
	{
		const AnimationTrackObject::KeyFrame &keyFrame = m_object->getKeyFrame(0);
		m_object->setWorldPosition(keyFrame.m_translate);
		m_object->setWorldScale(keyFrame.m_scale);
		m_object->setWorldOrientation(keyFrame.m_rotate);
	}
	// ��ʾ�ؼ�֡�ڵ�
	static_cast<AnimationTrackObjectVisible*>(m_object->getSceneObjectVisible())->showKeyNode(true);
	// ���ر߿�
	static_cast<AnimationTrackObjectVisible*>(m_object->getSceneObjectVisible())->showBorder(false);
	// ��ѡ�켣��������
	plugin.getWorldEditor()->getMapManager()->deselectObject(m_object);

	GetDlgItem(IDC_BUTTON_SIMULATE)->SetWindowText("ģ��(&I)");

	m_object = 0;
	// ������༭��
	plugin.getWorldEditor()->getMainFrame()->getDockingPaneManager()->ClosePane(DlgTrackTools::IDD);
	plugin.getWorldEditor()->getMainFrame()->getDockingPaneManager()->ShowPane(DlgEditTools::IDD);
	plugin.getWorldEditor()->getMainFrame()->getDockingPaneManager()->ShowPane(DlgPlaceSceneObject::IDD);
	plugin.getWorldEditor()->getMainFrame()->getDockingPaneManager()->ShowPane(DlgObjectList::IDD);

}

// ɾ��һ���ؼ�֡
void DlgTrackTools::deleteKeyFrame(KeyObjectData &data)
{
	// ��ѡ����
	ObjectEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->deselectObject(data.m_keyObject);
	// ���ٶ���
	m_keyObjectRoot->destroyChildObject(data.m_keyObject);
	// ���б���ɾ��
	m_listKeyObject.erase(std::find(m_listKeyObject.begin() , m_listKeyObject.end() , data.m_keyObject));
	// ɾ���ؼ�֡
	m_object->removeKeyFrame(data.m_index);	
	// �����������йؼ�֡����
	rebuildKeyFrameIndex();
}

// ����һ���ؼ������
KeyObjectData DlgTrackTools::createKeyObject(size_t index)
{
	const AnimationTrackObject::KeyFrame &keyFrame = m_object->getKeyFrame(index);
	NameValuePairList params;
	params["mesh"] = "axes.mesh";
	KeyObjectData newData;
	TrackKeyFrameObject * keyObject = static_cast<TrackKeyFrameObject*>(m_keyObjectRoot->createChildObject("TrackKeyFrameObject" , &params));
	newData.m_object = m_object;
	newData.m_index = index;
	newData.m_dlgTrackTools = this;
	newData.m_keyObject = keyObject;
	keyObject->setUserAny(Any(newData));
	keyObject->updateFromKeyFrame();
	return newData;
}

// ��¡һ���ؼ�֡
KeyObjectData DlgTrackTools::cloneKeyFrame(KeyObjectData &data)
{
	AnimationTrackObject::KeyFrame newKeyFrame = m_object->getKeyFrame(data.m_index);
	newKeyFrame.m_translate += Vector3(0.5f , 0 , 0.5f);
	size_t index = m_object->insertKeyFrame(data.m_index , newKeyFrame.m_length , newKeyFrame.m_translate , newKeyFrame.m_scale , newKeyFrame.m_rotate);
	KeyObjectData newData = createKeyObject(index);
	// ��ӵ��б���
	m_listKeyObject.insert(std::find(m_listKeyObject.begin() , m_listKeyObject.end() , data.m_keyObject) + 1 , newData.m_keyObject);

	// �����������йؼ�֡����
	rebuildKeyFrameIndex();

	return safe_cast<KeyObjectData>(newData.m_keyObject->getUserAny());
}


// �����������йؼ�֡����
void DlgTrackTools::rebuildKeyFrameIndex()
{
	size_t index = 0;
	// �������ɹ켣�ؼ�����������
	for(ListKeyObject::iterator iter = m_listKeyObject.begin() ; iter != m_listKeyObject.end() ; iter ++)
	{
		KeyObjectData data = safe_cast<KeyObjectData>((*iter)->getUserAny());
		data.m_index = index++;
		(*iter)->setUserAny(Any(data));
		(*iter)->updateFromKeyFrame();
	}
}

// ��β������һ���ؼ�֡
KeyObjectData DlgTrackTools::pushBackKeyFrame(const AnimationTrackObject::KeyFrame &keyFrame)
{
	// ��ӵ��ؼ�֡
	size_t index = m_object->addKeyFrameToBack(keyFrame.m_length , keyFrame.m_translate , keyFrame.m_scale , keyFrame.m_rotate);
	KeyObjectData data = createKeyObject(index);
	// ��ӵ��б���
	m_listKeyObject.push_back(data.m_keyObject);
	// �����������йؼ�֡����
	rebuildKeyFrameIndex();
	return safe_cast<KeyObjectData>(data.m_keyObject->getUserAny());
}

// ��β����һ���ؼ�֡ɾ��
void DlgTrackTools::popBackKeyFrame()
{
	if(m_object->getKeyFrameCount() > 0)
	{
		deleteKeyFrame(safe_cast<KeyObjectData>(m_listKeyObject.back()->getUserAny()));
	}
}

// ���ӿ���Ϣ������false��ִ�к����������
bool DlgTrackTools::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	ObjectEditorPlugin &plugin = ObjectEditorPlugin::getSingleton();

	if(message == WM_KEYUP)
	{
		switch(wParam)
		{
		case 'S':
			{
				plugin.setObjectAction(0);
				plugin.setObjectAction(new TrackSelectAction(m_object));

				return false;
			}
			break;
		case 'D':
		case VK_DELETE:
			{
				TrackDeleteAction action(m_object);
				action.execute();
				return false;
			}
			break;
		case 'C':
			{
				TrackCloneAction action(m_object);
				action.execute();
				return false;
			}
			break;
		case 'T':
			{
				TrackPlaceFloorAction action(m_object);
				action.execute();
				return false;
			}
			break;
		case 'A':
			{
				plugin.setObjectAction(0);
				plugin.setObjectAction(new TrackDragAction(m_object));
				return false;
			}
			break;
		case 'M':
			{
				plugin.setObjectAction(0);
				plugin.setObjectAction(new TrackMoveAction(m_object));

				return false;
			}
			break;
		case 'R':
			{
				plugin.setObjectAction(0);
				plugin.setObjectAction(new TrackRotateAction(m_object));
				return false;
			}
			break;
		case 'L':
			{
				plugin.setObjectAction(0);
				plugin.setObjectAction(new TrackScaleAction(m_object));
				return false;
			}
			break;
		case 'N':
			{
				plugin.setObjectAction(0);
				plugin.setObjectAction(new TrackNewAction(this));
				return false;
			}
			break;
		case 'I':
			{
				OnBnClickedButtonSimulate();
				return false;
			}
			break;
		}
	}
	return true;
}

void DlgTrackTools::OnBnClickedButtonClose()
{
	ObjectEditorPlugin::getSingleton().getWorldEditor()->getMainFrame()->getDockingPaneManager()->ClosePane(DlgTrackTools::IDD);
}

void DlgTrackTools::OnBnClickedButtonNew()
{
	ObjectEditorPlugin &plugin = ObjectEditorPlugin::getSingleton();
	plugin.setObjectAction(0);
	plugin.setObjectAction(new TrackNewAction(this));
}

void DlgTrackTools::OnBnClickedButtonSelect()
{
	ObjectEditorPlugin &plugin = ObjectEditorPlugin::getSingleton();
	plugin.setObjectAction(0);
	plugin.setObjectAction(new TrackSelectAction(m_object));
}

void DlgTrackTools::OnBnClickedButtonDelete3()
{
	TrackDeleteAction action(m_object);
	action.execute();
}

void DlgTrackTools::OnBnClickedButtonClone2()
{
	TrackCloneAction action(m_object);
	action.execute();
}

void DlgTrackTools::OnBnClickedButtonPlacefloor2()
{
	TrackPlaceFloorAction action(m_object);
	action.execute();
}

void DlgTrackTools::OnBnClickedButtonDrag()
{
	ObjectEditorPlugin &plugin = ObjectEditorPlugin::getSingleton();
	plugin.setObjectAction(0);
	plugin.setObjectAction(new TrackDragAction(m_object));
}

void DlgTrackTools::OnBnClickedButtonMoveX()
{
	ObjectEditorPlugin &plugin = ObjectEditorPlugin::getSingleton();
	plugin.setObjectAction(0);
	plugin.setObjectAction(new TrackMoveAction(m_object));
}

void DlgTrackTools::OnBnClickedButtonRotateX2()
{
	ObjectEditorPlugin &plugin = ObjectEditorPlugin::getSingleton();
	plugin.setObjectAction(0);
	plugin.setObjectAction(new TrackRotateAction(m_object));
}

void DlgTrackTools::OnBnClickedButtonScaleX2()
{
	ObjectEditorPlugin &plugin = ObjectEditorPlugin::getSingleton();
	plugin.setObjectAction(0);
	plugin.setObjectAction(new TrackScaleAction(m_object));
}

void DlgTrackTools::OnBnClickedButtonSimulate()
{
	if(m_simulateObject == 0)
	{
		NameValuePairList params;
		params["mesh"] = "R_F_Z_Head_01.mesh";
		m_simulateObject = static_cast<CharacterObject*>(DisplaySystem::getSingleton().getMainScene()->getRootObject()->createChildObject("CharacterObject" , &params));
		m_simulateObject->getAnimationController().play("run");
		m_keyObjectRoot->setVisible(false);
		m_controller = m_object->createTrack(m_simulateObject);
		m_controller->playByIndex();

		GetDlgItem(IDC_BUTTON_SIMULATE)->SetWindowText("ֹͣ(&I)");

		Root::getSingleton().addFrameListener(this);
	}
	else
	{
		m_object->destroyTrack(m_controller);
		m_controller = 0;
		m_simulateObject->getScene()->destroySceneObject(m_simulateObject);
		m_simulateObject = 0;

		m_keyObjectRoot->setVisible(true);
		// ���ٹ켣�ؼ������
		for(ListKeyObject::iterator iter = m_listKeyObject.begin() ; iter != m_listKeyObject.end() ; iter ++)
		{
			(*iter)->getSceneObjectVisible()->showBorder((*iter)->getSceneObjectVisible()->isShowBorder());
		}

		GetDlgItem(IDC_BUTTON_SIMULATE)->SetWindowText("ģ��(&I)");

		Root::getSingleton().removeFrameListener(this);
	}
}
// ÿ֡�����Ƿ�ģ�����
bool DlgTrackTools::frameStarted(const FrameEvent& evt)
{
	if(m_simulateObject && m_controller)
	{
		if(m_controller->hasEnded())
		{
			if(m_simulateObject->getAnimationController().getPlayingAnimation(m_simulateObject->getAnimationInfo()["idle"]).isNull())
			{
				m_simulateObject->getAnimationController().play("idle");
			}
		}
	}
	return true;
}
