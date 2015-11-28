// DlgCreateTerrain.cpp : implementation file
//

#include "stdafx.h"
#include "DlgCreateTerrain.h"
#include "TerrainEditorPlugin.h"
#include "ETTerrain.h"

using namespace Ogre;
// DlgCreateTerrain dialog

IMPLEMENT_DYNAMIC(DlgCreateTerrain, CDialog)

DlgCreateTerrain::DlgCreateTerrain(CWnd* pParent /*=NULL*/)
	: CDialog(DlgCreateTerrain::IDD, pParent)
{

}

DlgCreateTerrain::~DlgCreateTerrain()
{
}

void DlgCreateTerrain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TILE_X, m_cmbTileX);
	DDX_Control(pDX, IDC_COMBO_TILE_Y, m_cmbTileY);
	DDX_Control(pDX, IDC_EDIT_SIZE, m_edtSize);
}


BEGIN_MESSAGE_MAP(DlgCreateTerrain, CDialog)
	ON_BN_CLICKED(IDOK, &DlgCreateTerrain::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_TILE_X, &DlgCreateTerrain::OnCbnSelchangeComboTileX)
	ON_CBN_SELCHANGE(IDC_COMBO_TILE_Y, &DlgCreateTerrain::OnCbnSelchangeComboTileY)
END_MESSAGE_MAP()


// DlgCreateTerrain message handlers

BOOL DlgCreateTerrain::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ȥ��8192
	for(int i = 1 ; i < 14 ; i ++)
	{
		m_cmbTileX.AddString(StringConverter::toString((int)pow(2.0f , i)).c_str());
		m_cmbTileY.AddString(StringConverter::toString((int)pow(2.0f , i)).c_str());
	}
	m_cmbTileX.SetCurSel(7);
	m_cmbTileY.SetCurSel(7);
	m_edtSize.SetWindowText("256 512 256");

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void DlgCreateTerrain::OnBnClickedOk()
{
	IScene *scene = TerrainEditorPlugin::getSingleton().getWorldEditor()->getMapManager()->getScene();

	// ����Ƿ��Ѿ�����
	if(scene->hasSceneObject("Terrain_0_0"))
	{
		AfxMessageBox("�����Ѿ����ڣ�һ������������һ�����δ��ڣ��������ظ�������");
		return;
	}

	// ��ȡ����ѡ��
	CString strTileX , strTileY;
	size_t tileX , tileY;
	m_cmbTileX.GetLBText(m_cmbTileX.GetCurSel() , strTileX);
	tileX = StringConverter::parseUnsignedLong((LPCSTR)strTileX);
	m_cmbTileY.GetLBText(m_cmbTileY.GetCurSel() , strTileY);
	tileY = StringConverter::parseUnsignedLong((LPCSTR)strTileY);

	CString strSize;
	m_edtSize.GetWindowText(strSize);
	Vector3 size(StringConverter::parseVector3((LPCSTR)strSize));
	

	// ����һ������
	TerrainObject *terrain = static_cast<TerrainObject*>(scene->getRootObject()->createChildObject("Terrain_0_0" , TerrainObjectFactory::FACTORY_TYPE));
	// ����LOD���������ƫ��
	terrain->getOgreTerrain()->setLODErrorMargin( 2 , 600);
	size_t tileSize = std::min(std::min(tileX + 1 , tileY + 1) , (size_t)17);
	// ��������
	terrain->getOgreTerrain()->createTerrain(tileX + 1 , tileY + 1 , AxisAlignedBox(0, -size.y, 0, size.x, size.y, size.z) , 0.5f , tileSize);
	terrain->getOgreTerrain()->setSplattingSplit(size.x / 6.4 , size.z / 6.4);

	// �����������
	TerrainEditorPlugin::getSingleton().activateTerrain(terrain->getOgreTerrain());


	OnOK();
}

void DlgCreateTerrain::OnCbnSelchangeComboTileX()
{
	// ��ȡ����ѡ��
	CString strTileX;
	size_t tileX;
	m_cmbTileX.GetLBText(m_cmbTileX.GetCurSel() , strTileX);
	tileX = StringConverter::parseUnsignedLong((LPCSTR)strTileX);

	CString strSize;
	m_edtSize.GetWindowText(strSize);
	Vector3 size(StringConverter::parseVector3((LPCSTR)strSize));
	size.x = tileX;
	m_edtSize.SetWindowText(StringConverter::toString(size).c_str());
}

void DlgCreateTerrain::OnCbnSelchangeComboTileY()
{
	// ��ȡ����ѡ��
	CString strTileY;
	size_t tileY;
	m_cmbTileY.GetLBText(m_cmbTileY.GetCurSel() , strTileY);
	tileY = StringConverter::parseUnsignedLong((LPCSTR)strTileY);

	CString strSize;
	m_edtSize.GetWindowText(strSize);
	Vector3 size(StringConverter::parseVector3((LPCSTR)strSize));
	size.z = tileY;
	m_edtSize.SetWindowText(StringConverter::toString(size).c_str());
}
