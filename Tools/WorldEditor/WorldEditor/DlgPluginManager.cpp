// DlgPluginManager.cpp : implementation file
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "DlgPluginManager.h"
#include "PluginManager.h"

using namespace Ogre;
// CDlgPluginManager dialog

IMPLEMENT_DYNAMIC(CDlgPluginManager, CDialog)

CDlgPluginManager::CDlgPluginManager(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPluginManager::IDD, pParent)
{

}

CDlgPluginManager::~CDlgPluginManager()
{
}

void CDlgPluginManager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PLUGINS, m_lstPlugins);
	DDX_Control(pDX, IDC_CHECK_AUTO_LOAD, m_chkAutoLoad);
	DDX_Control(pDX, IDC_CHECK_LOAD, m_chkLoad);
}


BEGIN_MESSAGE_MAP(CDlgPluginManager, CDialog)
	ON_NOTIFY(HDN_ITEMCHANGED, 0, &CDlgPluginManager::OnHdnItemchangedListPlugins)
	ON_BN_CLICKED(IDOK, &CDlgPluginManager::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgPluginManager message handlers

BOOL CDlgPluginManager::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_lstPlugins.InsertColumn(0 , "�������" , 0 , 60);
	m_lstPlugins.InsertColumn(1 , "��������" , 0 , 60);
	m_lstPlugins.InsertColumn(2 , "����״̬" , 0 , 60);
	m_lstPlugins.InsertColumn(3 , "����״̬" , 0 , 60);
	m_lstPlugins.InsertColumn(4 , "�������" , 0 , 60);
	m_lstPlugins.InsertColumn(5 , "�ļ���" , 0 , 60);
	m_lstPlugins.InsertColumn(6 , "������Ϣ" , 0 , 60);
	m_lstPlugins.InsertColumn(7 , "�汾��" , 0 , 60);
	m_lstPlugins.InsertColumn(8 , "��������б�" , 0 , 60);



	OnBnClickedOk();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPluginManager::OnHdnItemchangedListPlugins(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CDlgPluginManager::OnBnClickedOk()
{
	m_lstPlugins.DeleteAllItems();
	m_chkAutoLoad.EnableWindow(FALSE);
	m_chkLoad.EnableWindow(FALSE);

	int index = 0;
	PluginManager::PluginIterator iterator = PluginManager::getSingleton().getPluginIterator();
	while(iterator.hasMoreElements())
	{
		IPlugin *plugin = iterator.getNext();
		const IPlugin::Description &desc = plugin->getDescription();
		m_lstPlugins.InsertItem(index , desc.name.c_str());
		m_lstPlugins.SetItemText(index , 2 , plugin->isInited() ? "������" : "δ����");
		m_lstPlugins.SetItemText(index , 3 , plugin->isActivate() ? "�Ѽ���" : "δ����");
		m_lstPlugins.SetItemText(index , 4 , desc.desc.c_str());
		m_lstPlugins.SetItemText(index , 5 , desc.fileName.c_str());
		m_lstPlugins.SetItemText(index , 6 , desc.publish.c_str());
		m_lstPlugins.SetItemText(index , 7 , StringConverter::toString(desc.version).c_str());
		m_lstPlugins.SetItemText(index , 8 , StringConverter::toString(desc.dependentPlugins).c_str());
		index++;
	}
}
