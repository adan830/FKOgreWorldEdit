#include "StdAfx.h"
#include "TerrainEditorPlugin.h"
#include "DecalSelectionRenderer.h"
#include "SingleSelectionAction.h"
#include "LineDecalSelectionRenderer.h"
#include "ETTerrain.h"
#include "TerrainBrush.h"
#include "SelectionRenderer.h"
#include "TerrainAction.h"
#include "SelectTerrainAction.h"

namespace Ogre
{
	// ����ģʽʵ��
	template<> TerrainEditorPlugin* Singleton<TerrainEditorPlugin>::ms_Singleton = 0;
	TerrainEditorPlugin* TerrainEditorPlugin::getSingletonPtr(void)
	{
		return ms_Singleton;
	}
	TerrainEditorPlugin& TerrainEditorPlugin::getSingleton(void)
	{  
		assert( ms_Singleton );  return ( *ms_Singleton );  
	}

	TerrainEditorPlugin::TerrainEditorPlugin(HINSTANCE hInstance)
		: m_inited(false)
		, m_activated(false)
		, m_selectionRenderer(0)
		, m_selection(0)
		, m_activatedTerrain(0)
		, m_terrainAction(0)
		, m_brush(0)
	{
		m_description.name = "���α༭�����";
		TCHAR szFileName[MAX_PATH];
		::GetModuleFileName(hInstance , szFileName , MAX_PATH);
		String outPath;
		StringUtil::splitFilename(szFileName , m_description.fileName , outPath);
		m_description.desc = "��Ⱦ����ԭ��ƽ���������";
		m_description.publish = "������ 2008-1-26";
		m_description.version = 1;
	}

	TerrainEditorPlugin::~TerrainEditorPlugin(void)
	{
	}


	// ��ʼ���������������worldEditor������༭���Ľӿ�
	bool TerrainEditorPlugin::init(IWorldEditor *worldEditor)
	{
		if(m_inited)
			return true;
		m_worldEditor = worldEditor;
		m_brush = new TerrainBrush;
		m_brush->loadBrushFromFile("Բ��_��.tga");
		m_worldEditor->getMapManager()->addListener(this);

		m_worldEditor->getMainFrame()->addListener(this);
		CXTPDockingPane* paneBrush = m_worldEditor->getMainFrame()->createPane(DlgTerrainBrush::IDD , "��ˢ����" , CRect(0 , 0 , 230 , 230) , xtpPaneDockRight);
		CXTPDockingPane* paneHeight = m_worldEditor->getMainFrame()->createPane(DlgTerrainHeight::IDD , "���ι���" , CRect(0 , 0 , 230 , 230) , xtpPaneDockBottom , paneBrush);
		CXTPDockingPane* panePaint = m_worldEditor->getMainFrame()->createPane(DlgTerrainPaint::IDD , "�ر���ͼ" , CRect(0 , 200 , 230 , 430) , xtpPaneDockBottom , paneHeight);

		m_worldEditor->getMainFrame()->createPluginToolBarButton("���β��" , this);

		m_activated = true;
		deactivate();
		m_inited = true;
		return m_inited;
	}

	// �����Ա�༭�������
	void TerrainEditorPlugin::activateTerrain(ETTerrain *terrain)
	{
		if(m_activatedTerrain == terrain)
			return;
		deactivateTerrain();
		m_activatedTerrain = terrain;
		m_activatedTerrain->getTerrainImpl()->getParentSceneNode()->showBoundingBox(true);
		m_selection = new Selection(m_activatedTerrain);
		m_selectionRenderer = new LineDecalSelectionRenderer(m_selection , m_brush);
		m_DlgTerrainPaint.refreshAllTexture();
		addTerrainBlockerPass(terrain);

		setTerrainAction(new SelectTerrainAction(getActivatedTerrain()
			, getSelectionRenderer()
			, getSelection() 
			, getTerrainBrush()
			));

	}
	// ���������
	void TerrainEditorPlugin::deactivateTerrain()
	{
		OGRE_DELETE(m_terrainAction);
		OGRE_DELETE(m_selectionRenderer);
		OGRE_DELETE(m_selection);
		if(m_activatedTerrain)
		{
			m_activatedTerrain->getTerrainImpl()->getParentSceneNode()->showBoundingBox(false);
			m_activatedTerrain = 0;
		}
	}

	// �رղ��
	void TerrainEditorPlugin::shutdown()
	{
		if(!m_inited)
			return;
		m_worldEditor->getMainFrame()->destroyPluginToolBarButton("���β��");
		m_worldEditor->getMapManager()->removeListener(this);

		m_worldEditor->getMainFrame()->destroyPane(DlgTerrainBrush::IDD);
		m_worldEditor->getMainFrame()->destroyPane(DlgTerrainHeight::IDD);
		m_worldEditor->getMainFrame()->destroyPane(DlgTerrainPaint::IDD);
		deactivateTerrain();
		OGRE_DELETE(m_brush);

		m_worldEditor->getMainFrame()->removeListener(this);
		m_inited = false;
	}

	// �Ƿ��Ѿ���ʼ��
	bool TerrainEditorPlugin::isInited()const
	{
		return m_inited;
	}

	// ������
	void TerrainEditorPlugin::activate()
	{
		if(m_activated)
			return;
		m_worldEditor->getMainFrame()->getDockingPaneManager()->ShowPane(DlgTerrainBrush::IDD);
		m_worldEditor->getMainFrame()->getDockingPaneManager()->ShowPane(DlgTerrainHeight::IDD);
		m_worldEditor->getMainFrame()->getDockingPaneManager()->ShowPane(DlgTerrainPaint::IDD);
		m_worldEditor->getMainFrame()->addChildViewListener(this);

		setTerrainAction(new SelectTerrainAction(getActivatedTerrain()
			, getSelectionRenderer()
			, getSelection() 
			, getTerrainBrush()
			));

		m_activated = true;

	}

	// ��������
	void TerrainEditorPlugin::deactivate()
	{
		if(!m_activated)
			return;
		deactivateTerrain();
		m_worldEditor->getMainFrame()->removeChildViewListener(this);

		m_worldEditor->getMainFrame()->getDockingPaneManager()->ClosePane(DlgTerrainBrush::IDD);
		m_worldEditor->getMainFrame()->getDockingPaneManager()->ClosePane(DlgTerrainHeight::IDD);
		m_worldEditor->getMainFrame()->getDockingPaneManager()->ClosePane(DlgTerrainPaint::IDD);

		m_activated = false;
	}

	// �Ƿ��Ѿ�����
	bool TerrainEditorPlugin::isActivate()const
	{
		return m_activated;
	}
	// ʵ��������β�����
	void TerrainEditorPlugin::setTerrainAction(TerrainAction *action)
	{
		OGRE_DELETE(m_terrainAction);
		m_terrainAction = action;
	}
	// ��ò������
	const IPlugin::Description &TerrainEditorPlugin::getDescription()const
	{
		return m_description;
	}

	// ��ѯһЩ���ݣ�Ȼ�󷵻ص�result��
	bool TerrainEditorPlugin::query(const String &name , const NameValuePairList *params , void *result)
	{
		return false;
	}

	// �ӿڸ���
	void TerrainEditorPlugin::preViewportUpdate(const RenderTargetViewportEvent& evt)
	{
		if(m_terrainAction)
		{
			m_terrainAction->preViewportUpdate(evt);
		}
	}
	// ��������֪ͨ
	LRESULT TerrainEditorPlugin::onDockingPaneNotify(WPARAM wParam, LPARAM lParam)
	{
		if (wParam == XTP_DPN_SHOWWINDOW)
		{
			CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;

			if (!pPane->IsValid())
			{
				switch (pPane->GetID())
				{
				case DlgTerrainBrush::IDD:
					{
						if (m_DlgTerrainBrush.GetSafeHwnd() == 0)
						{
							m_DlgTerrainBrush.Create(DlgTerrainBrush::IDD, m_worldEditor->getMainFrame()->getWnd());
						}
						pPane->Attach(&m_DlgTerrainBrush);
					}
					break;
				case DlgTerrainHeight::IDD:
					{
						if (m_DlgTerrainHeight.GetSafeHwnd() == 0)
						{
							m_DlgTerrainHeight.Create(DlgTerrainHeight::IDD, m_worldEditor->getMainFrame()->getWnd());
						}
						pPane->Attach(&m_DlgTerrainHeight);
					}
					break;
				case DlgTerrainPaint::IDD:
					{
						if (m_DlgTerrainPaint.GetSafeHwnd() == 0)
						{
							m_DlgTerrainPaint.Create(DlgTerrainPaint::IDD, m_worldEditor->getMainFrame()->getWnd());
						}
						pPane->Attach(&m_DlgTerrainPaint);
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

	// ���ӿ���Ϣ
	bool TerrainEditorPlugin::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		switch(message)
		{
		case WM_LBUTTONUP:
			{
				//short xPos = GET_X_LPARAM(lParam); 
				//short yPos = GET_Y_LPARAM(lParam); 
				//CRect rect;
				//::GetClientRect(view->getWnd()->GetSafeHwnd() , &rect);
				//Vector3 pos;
				//Ray ray = view->getCamera()->getCameraToViewportRay(xPos / (float)rect.Width()
				//	, yPos / (float)rect.Height());
				//if(ISceneSystem::getSingleton().rayQuary(ray , 
				//	pos))
				//{
				//	pos.length();
				//}
			}
			break;
		case WM_MOUSEMOVE:
			{
			}
			break;
		case WM_CHAR:
			switch(wParam)
			{
			case '[':
				m_DlgTerrainBrush.m_sliSize.SetPos(m_DlgTerrainBrush.m_sliSize.GetPos() - 1);
				break;
			case ']':
				m_DlgTerrainBrush.m_sliSize.SetPos(m_DlgTerrainBrush.m_sliSize.GetPos() + 1);
				break;
			}
			break;
		}
		return true;
	}

	// ���ٳ���
	void TerrainEditorPlugin::onDestroyScene(IScene *scene)
	{
		deactivateTerrain();
	}

	// ���泡��
	void TerrainEditorPlugin::onPreSaveScene(const String &fileName ,IScene *scene)
	{
		// �ҵ����е��Σ�����֮
		IScene::SceneObjectIterator iter = scene->getSceneObjectIterator();
		while(iter.hasMoreElements())
		{
			ISceneObject *obj = iter.getNext();
			if(obj->getType() == TerrainObjectFactory::FACTORY_TYPE)
			{
				TerrainObject *terrain = static_cast<TerrainObject*>(obj);

				String outBasename;
				String outExtention; 
				String outPath;
				StringUtil::splitFullFilename(fileName , outBasename , outExtention , outPath);
				removeTerrainBlockerPass(terrain->getOgreTerrain());
				terrain->save(outBasename);
				addTerrainBlockerPass(terrain->getOgreTerrain());
			}
		}
	}

	// ����һ������
	void TerrainEditorPlugin::onPostLoadScene(const String &fileName ,IScene *scene)
	{
		// �ҵ����е��Σ�����֮
		IScene::SceneObjectIterator iter = scene->getSceneObjectIterator();
		while(iter.hasMoreElements())
		{
			ISceneObject *obj = iter.getNext();
			if(obj->getType() == TerrainObjectFactory::FACTORY_TYPE)
			{
				TerrainObject *terrain = static_cast<TerrainObject*>(obj);
				activateTerrain(terrain->getOgreTerrain());
			}
		}
	}

	// �½�����
	void TerrainEditorPlugin::onNewScene(IScene *scene)
	{
		// �ҵ����е��Σ�����֮
		IScene::SceneObjectIterator iter = scene->getSceneObjectIterator();
		while(iter.hasMoreElements())
		{
			ISceneObject *obj = iter.getNext();
			if(obj->getType() == TerrainObjectFactory::FACTORY_TYPE)
			{
				TerrainObject *terrain = static_cast<TerrainObject*>(obj);
				activateTerrain(terrain->getOgreTerrain());
			}
		}
	}

	// Ϊ�������һ���ϰ��������ʾPass
	void TerrainEditorPlugin::addTerrainBlockerPass(ETTerrain *terrain)
	{
		MaterialPtr mat = terrain->getTerrainImpl()->getMaterial();
		Technique *tech = mat->getTechnique(0);
		// ����û�����Blocker��Pass������оͲ��������
		for(size_t i = 0 ; i < tech->getNumPasses() ; i ++)
		{
			if(tech->getPass(i)->getName() == "Blocker")
				return;
		}
		Pass *blockerPass = mat->getTechnique(0)->createPass();
		blockerPass->setName("Blocker");
		// ʹ��͸����Ⱦ
		blockerPass->setSceneBlending(SBT_TRANSPARENT_ALPHA);
		blockerPass->setVertexProgram("ET/Programs/VSLodMorph2");
		//blockerPass->setPolygonMode(PM_WIREFRAME);
		//blockerPass->setLightingEnabled(false);
		blockerPass->setCullingMode(CULL_ANTICLOCKWISE);
		//blockerPass->setDepthCheckEnabled(false);
		// ��������ͼ
		TextureUnitState *state = blockerPass->createTextureUnitState(terrain->getBlockerTexture()->getName());
		state->setTextureFiltering(TFO_NONE);

	}
	// �ѵ��ε��ϰ�������ʾPass�Ƴ�
	void TerrainEditorPlugin::removeTerrainBlockerPass(ETTerrain *terrain)
	{
		MaterialPtr mat = terrain->getTerrainImpl()->getMaterial();
		Technique *tech = mat->getTechnique(0);
		// ����û�����Blocker��Pass������о�ɾ��֮
		for(size_t i = 0 ; i < tech->getNumPasses() ; i ++)
		{
			if(tech->getPass(i)->getName() == "Blocker")
			{
				tech->removePass(i);
				return;
			}
		}
	}

	// Ϊ��ǰ����ĵ������һ���ϰ��������ʾPass
	void TerrainEditorPlugin::addTerrainBlockerPass()
	{
		if(m_activatedTerrain)
		{
			addTerrainBlockerPass(m_activatedTerrain);
		}
	}
	// �ѵ�ǰ����ĵ��ε��ϰ�������ʾPass�Ƴ�
	void TerrainEditorPlugin::removeTerrainBlockerPass()
	{
		if(m_activatedTerrain)
		{
			removeTerrainBlockerPass(m_activatedTerrain);
		}
	}


};