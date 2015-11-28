#include "StdAfx.h"
#include "AxisGridPlugin.h"

namespace Ogre
{
	// ����ģʽʵ��
	template<> AxisGridPlugin* Singleton<AxisGridPlugin>::ms_Singleton = 0;
	AxisGridPlugin* AxisGridPlugin::getSingletonPtr(void)
	{
		return ms_Singleton;
	}
	AxisGridPlugin& AxisGridPlugin::getSingleton(void)
	{  
		assert( ms_Singleton );  return ( *ms_Singleton );  
	}

	AxisGridPlugin::AxisGridPlugin(HINSTANCE hInstance)
		: m_inited(false)
		, m_activated(false)
		, m_gridObject(0)
		, m_gridNode(0)
		, m_axisOverlay(0)
		, m_axisElement(0)
		, m_axisRTT(0)
		, m_statElement(0)
	{
		m_description.name = "��������ϵ���";
		TCHAR szFileName[MAX_PATH];
		::GetModuleFileName(hInstance , szFileName , MAX_PATH);
		String outPath;
		StringUtil::splitFilename(szFileName , m_description.fileName , outPath);
		m_description.desc = "��Ⱦ����ԭ��ƽ���������";
		m_description.publish = "������ 2008-1-26";
		m_description.version = 1;
	}

	AxisGridPlugin::~AxisGridPlugin(void)
	{
	}


	// ��ʼ���������������worldEditor������༭���Ľӿ�
	bool AxisGridPlugin::init(IWorldEditor *worldEditor)
	{
		if(m_inited)
			return true;
		m_worldEditor = worldEditor;
		// �����������
		m_gridObject = createGrid(128 , 128 , 1);
		// ��ʾ֮
		m_gridNode = m_worldEditor->getDisplaySystem()->getMainSceneMgr()->getRootSceneNode()->createChildSceneNode("GridNode");
		m_gridNode->attachObject(m_gridObject);


		m_axisRTT = new AxisRTT();
		m_axisRTT->init(worldEditor);
		m_axisRTT->setEnabled(true);
		// ����������
		createAxisOverlay();
		// ����ͳ�ƽ���
		createStatOverlay();

		m_worldEditor->getMainFrame()->addChildViewListener(this);

		activate();
		m_inited = true;
		return m_inited;
	}

	// ��������Ϣ
	bool AxisGridPlugin::onWndMsg(IChildView *view , UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		switch(message)
		{
		case WM_KEYUP:
			switch(wParam)
			{
			case 'G':
				m_gridObject->setVisible(!m_gridObject->getVisible());
				break;
			case VK_INSERT:
				Profiler::getSingleton().setEnabled(!Profiler::getSingleton().getEnabled());
				break;
			}
			break;
		}
		return true;
	}
	// �رղ��
	void AxisGridPlugin::shutdown()
	{
		if(!m_inited)
			return;
		m_worldEditor->getMainFrame()->removeChildViewListener(this);

		// �����������
		m_worldEditor->getDisplaySystem()->getMainSceneMgr()->destroyManualObject(m_gridObject);
		m_worldEditor->getDisplaySystem()->getMainSceneMgr()->destroySceneNode(m_gridNode->getName());
		m_gridObject = 0;
		m_gridNode = 0;

		// �������������
		OverlayManager::getSingleton().destroyOverlayElement(m_statElement);
		OverlayManager::getSingleton().destroyOverlayElement(m_axisElement);
		OverlayManager::getSingleton().destroy(m_axisOverlay);
		m_statElement = 0;
		m_axisOverlay = 0;
		m_axisElement = 0;
		OGRE_DELETE(m_axisRTT);

		m_inited = false;
	}

	// ��������
	ManualObject* AxisGridPlugin::createGrid(float width , float height , float interval)
	{
		// �����������
		static size_t s_index = 0;
		ManualObject *gridObject = m_worldEditor->getDisplaySystem()->getMainSceneMgr()->createManualObject(
			"GridObject" + StringConverter::toString(s_index));
		// �ú�ɫ���м���
		gridObject->begin(m_worldEditor->getDisplaySystem()->getColorMaterial(ColourValue::Black) , RenderOperation::OT_LINE_LIST);
		gridObject->position(-width , 0 , 0);
		gridObject->position(width , 0 , 0);
		gridObject->position(0 , 0 , -height);
		gridObject->position(0 , 0 , height);
		gridObject->end();
		// ����ͨ��
		gridObject->begin(m_worldEditor->getDisplaySystem()->getColorMaterial(ColourValue(0.37,0.37,0.37)) , RenderOperation::OT_LINE_LIST);
		for(float x = -width ; x <= width ; x += interval)
		{
			if(abs(x) > 0.0001f)
			{
				gridObject->position(x , 0 , -height);
				gridObject->position(x , 0 , height);
			}
		}
		for(float y = -height ; y <= height ; y += interval)
		{
			if(abs(y) > 0.0001f)
			{
				gridObject->position(-width , 0 , y);
				gridObject->position(width , 0 , y);
			}
		}
		gridObject->end();

		return gridObject;
	}
	// ����ͳ�ƽ���
	void AxisGridPlugin::createStatOverlay()
	{
		m_statElement = static_cast<TextAreaOverlayElement *>(OverlayManager::getSingleton().createOverlayElement("TextArea" , "StatElement"));
		m_statElement->setMetricsMode(GMM_PIXELS);
		m_statElement->setFontName("english");
		m_statElement->setColour(ColourValue(0.1 , 1 , 0.1));
		m_statElement->setDimensions(200 , 14);
		m_statElement->setLeft(m_axisElement->getWidth());
		m_statElement->setCharHeight(14);
		m_axisOverlay->add2D((OverlayContainer*)m_statElement);
	}
	// �������������
	void AxisGridPlugin::createAxisOverlay()
	{
		m_axisOverlay = OverlayManager::getSingleton().create("AxisOverlay");
		m_axisElement = OverlayManager::getSingleton().createOverlayElement("Panel" , "AxisElement");
		m_axisElement->setMetricsMode(GMM_PIXELS);
		m_axisElement->setDimensions(64 , 64);
		m_axisElement->setMaterialName(m_axisRTT->getAxisMaterialName());
		m_axisOverlay->add2D((OverlayContainer*)m_axisElement);
		m_axisOverlay->show();

	}
	// �Ƿ��Ѿ���ʼ��
	bool AxisGridPlugin::isInited()const
	{
		return m_inited;
	}


	// ������
	void AxisGridPlugin::activate()
	{
		if(m_activated)
			return;
		m_activated = true;
	}

	// ��������
	void AxisGridPlugin::deactivate()
	{
		if(!m_activated)
			return;
		m_activated = false;
	}

	// �Ƿ��Ѿ�����
	bool AxisGridPlugin::isActivate()const
	{
		return m_activated;
	}

	// ��ò������
	const IPlugin::Description &AxisGridPlugin::getDescription()const
	{
		return m_description;
	}

	// ��ѯһЩ���ݣ�Ȼ�󷵻ص�result��
	bool AxisGridPlugin::query(const String &name , const NameValuePairList *params , void *result)
	{
		return false;
	}

	// �ӿڸ���
	void AxisGridPlugin::preViewportUpdate(const RenderTargetViewportEvent& evt)
	{
		m_axisRTT->preViewportUpdate(evt);
		m_axisElement->setTop(evt.source->getTarget()->getHeight() - m_axisElement->getHeight());
		m_statElement->setTop(evt.source->getTarget()->getHeight() - m_statElement->getHeight());
	}

	// �ӿڸ���
	void AxisGridPlugin::postViewportUpdate(const RenderTargetViewportEvent& evt)
	{
		RenderTarget *target = evt.source->getTarget();
		char text[255];
		const RenderTarget::FrameStats& stats = target->getStatistics();
		sprintf(text , "FPS:%.2f TRI:%d BATCH:%d" , stats.avgFPS , stats.triangleCount , stats.batchCount);
		m_statElement->setCaption(text);
	}
};