// PaneInsertEntity.cpp : implementation file
//

#include "stdafx.h"
#include "DlgPlaceSceneObject.h"
#include "ObjectEditorPlugin.h"

#include "PlaceSceneObjectAction.h"

using namespace Ogre;
// DlgPlaceSceneObject dialog

IMPLEMENT_DYNAMIC(DlgPlaceSceneObject, CDialog)

DlgPlaceSceneObject::DlgPlaceSceneObject(CWnd* pParent /*=NULL*/)
	: CDialog(DlgPlaceSceneObject::IDD, pParent)
	, m_loadingType(LT_NONE)
{

}

DlgPlaceSceneObject::~DlgPlaceSceneObject()
{
}

void DlgPlaceSceneObject::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILETREE, m_fileTree);
}


BEGIN_MESSAGE_MAP(DlgPlaceSceneObject, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_FILETREE, &DlgPlaceSceneObject::OnTvnSelchangedFiletree)
	ON_NOTIFY(NM_CLICK, IDC_FILETREE, &DlgPlaceSceneObject::OnNMClickFiletree)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// DlgPlaceSceneObject message handlers

BOOL DlgPlaceSceneObject::OnInitDialog()
{
	CDialog::OnInitDialog();

	reloadTree();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
// ����������
void DlgPlaceSceneObject::reloadTree()
{
	m_fileTree.DeleteAllItems();

	// ���뾲̬ģ��
	HTREEITEM hModel = m_fileTree.InsertItem("��̬ģ��");
	m_loadingType = LT_MODEL;
	m_fileTree.loadTree(hModel , "..\\Data" , "mesh" , true , this);

	// ��������Ч��
	HTREEITEM hParticle = m_fileTree.InsertItem("����Ч��");
	m_loadingType = LT_PARTICLE;
	m_fileTree.loadTree(hParticle , "..\\Data" , "particle" , true , this);

	// �������
	HTREEITEM hSpecial = m_fileTree.InsertItem("�������");
	m_loadingType = LT_SPECIAL;
	m_fileTree.SetItemData(m_fileTree.InsertItem("�սڵ�" , hSpecial) , LT_SPECIAL);
	m_fileTree.SetItemData(m_fileTree.InsertItem("�����" , hSpecial) , LT_SPECIAL);
	m_fileTree.SetItemData(m_fileTree.InsertItem("��������" , hSpecial) , LT_SPECIAL);
	m_fileTree.SetItemData(m_fileTree.InsertItem("���Ӵ�����" , hSpecial) , LT_SPECIAL);
	m_fileTree.SetItemData(m_fileTree.InsertItem("���崥����" , hSpecial) , LT_SPECIAL);
	m_fileTree.SetItemData(m_fileTree.InsertItem("�����켣" , hSpecial) , LT_SPECIAL);


	// �������
	m_loadingType = LT_NONE;
}

// ����ص�
void DlgPlaceSceneObject::loadFile(HTREEITEM hItem , CFileFind &file)
{
	if(!file.IsDirectory())
	{
		switch(m_loadingType)
		{
		case LT_PARTICLE:
			{

				DataStreamPtr stream = ResourceGroupManager::getSingleton().openResource((LPCSTR)file.GetFileName());
				if(stream.isNull())
				{
					m_fileTree.DeleteItem(hItem);
				}
				else
				{
					size_t depth = 0;
					String line;
					while(!stream->eof())
					{
						line = stream->getLine();
						// Ignore comments & blanks
						if(!(line.length() == 0 || line.substr(0,2) == "//"))
						{
							if(depth == 0)
							{
								HTREEITEM hParticle = m_fileTree.InsertItem(line.c_str() , hItem);
								m_fileTree.SetItemData(hParticle , LT_PARTICLE);
								skipToNextOpenBrace(stream);
								depth++;
							}
							else
							{
								// �˳�һ��
								if(line == "}")
								{
									depth--;
								}
								// ����һ��
								else if(line == "{")
								{
									depth++;
								}
							}
						}
					}
					// ���û��ȫ���˳�������������ļ�������
					if(depth != 0)
					{
						m_fileTree.DeleteItem(hItem);
					}
				}
			}
			break;
		default:
			m_fileTree.SetItemData(hItem , m_loadingType);
			break;
		}
	}
}

void DlgPlaceSceneObject::skipToNextCloseBrace(DataStreamPtr& stream)
{
	String line;
	while (!stream->eof() && line != "}")
	{
		line = stream->getLine();
	}

}
//-----------------------------------------------------------------------
void DlgPlaceSceneObject::skipToNextOpenBrace(DataStreamPtr& stream)
{
	String line;
	while (!stream->eof() && line != "{")
	{
		line = stream->getLine();
	}

}

void DlgPlaceSceneObject::OnTvnSelchangedFiletree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	if(pNMTreeView->itemNew.hItem)
	{
		selectItem(pNMTreeView->itemNew.hItem);
	}
	*pResult = 0;
}

void DlgPlaceSceneObject::OnNMClickFiletree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	CPoint pt;
	GetCursorPos(&pt);
	m_fileTree.ScreenToClient(&pt);
	HTREEITEM hItem = m_fileTree.HitTest(pt);
	if(hItem)
	{
		if(m_fileTree.GetSelectedItem() == hItem)
		{
			selectItem(hItem);
		}
	}
	*pResult = 0;
}

// ѡ��һ��ģ��
void DlgPlaceSceneObject::selectItem(HTREEITEM hItem)
{
	LoadingType loadingType = (LoadingType)m_fileTree.GetItemData(hItem);
	ISceneObject *obj = 0;
	String itemText((LPCSTR)m_fileTree.GetItemText(hItem));
	try
	{
		switch(loadingType)
		{
		case LT_MODEL:
			{
				obj = DisplaySystem::getSingleton().getMainScene()->getRootObject()->createChildObject("EntityObject");
				// ��͸����
				static_cast<EntityObject*>(obj)->setMeshFile(itemText);
			}
			break;
		case LT_PARTICLE:
			{
				obj = DisplaySystem::getSingleton().getMainScene()->getRootObject()->createChildObject("ParticleObject");
				static_cast<ParticleObject*>(obj)->setParticleTemplate(itemText);
			}
			break;
		case LT_SPECIAL:
			{
				if(itemText == "�սڵ�")
				{
					obj = DisplaySystem::getSingleton().getMainScene()->getRootObject()->createChildObject("NullObject");
				}
				else if(itemText == "�����")
				{
					obj = DisplaySystem::getSingleton().getMainScene()->getRootObject()->createChildObject("CameraObject");
				}
				else if(itemText == "��������")
				{
					obj = DisplaySystem::getSingleton().getMainScene()->getRootObject()->createChildObject("EnvironmentObject");
				}
				else if(itemText == "���Ӵ�����")
				{
					obj = DisplaySystem::getSingleton().getMainScene()->getRootObject()->createChildObject("BoxTriggerObject");
				}
				else if(itemText == "���崥����")
				{
					obj = DisplaySystem::getSingleton().getMainScene()->getRootObject()->createChildObject("SphereTriggerObject");
				}
				else if(itemText == "�����켣")
				{
					obj = DisplaySystem::getSingleton().getMainScene()->getRootObject()->createChildObject("AnimationTrackObject");
				}
			}
			break;
		default:
			return;
			break;
		}
		if(obj)
		{
			ObjectEditorPlugin &plugin = ObjectEditorPlugin::getSingleton();
			plugin.setObjectAction(0);
			plugin.setObjectAction(new PlaceSceneObjectAction(obj));
		}
	}
	catch (Exception& e)
	{
		MessageBox(e.getFullDescription().c_str(), "��������ʧ��", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		return;
	}

}

void DlgPlaceSceneObject::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(m_fileTree.GetSafeHwnd())
	{
		m_fileTree.SetWindowPos(0 , 0 , 0 , cx , cy , 0);
	}
}
