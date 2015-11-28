#include "StdAfx.h"
#include "EnvironmentObject.h"
#include "IScene.h"
#include "ISceneSystem.h"
#include "EnvironmentObjectVisible.h"
namespace Ogre
{

	const String EnvironmentObjectFactory::FACTORY_TYPE = "EnvironmentObject";

	// ��øù�������������
	const String &EnvironmentObjectFactory::getType() const
	{
		return FACTORY_TYPE;
	}
	// ��������Ĵ�������
	ISceneObject *EnvironmentObjectFactory::create(const String &name , IScene *scene , const NameValuePairList* params)
	{
		EnvironmentObject *obj = new EnvironmentObject(name);
		obj->create(scene , params);
		return obj;
	}

	// ������������ٷ���
	void EnvironmentObjectFactory::destroy(ISceneObject *obj)
	{
		OGRE_DELETE(obj);
	}

	EnvironmentObject::EnvironmentObject(const String &name)
		: NullObject(name)
		, m_environmentInited(false)
		, m_skyBoxEnabled(false)
		, m_skyBoxRenderDistance(0)
		, m_fogColor(ColourValue::White)
		, m_fogDensity(0)
		, m_fogMode(FOG_NONE)
		, m_fogStart(0)
		, m_fogEnd(0)
		, m_ambientLight(ColourValue::White)
	{
		m_visible = false;
	}

	EnvironmentObject::~EnvironmentObject(void)
	{
		destroy();
	}

	// ��������ʵ��
	void EnvironmentObject::create(IScene *scene , const NameValuePairList* params)
	{
		destroy();
		NullObject::create(scene , params);

		// �ӳ�����������ȡ��������
		_getEnvironment();
	}

	// ���ٶ���ʵ��
	void EnvironmentObject::destroy()
	{
		_disableEnvironment();
		NullObject::destroy();
		m_environmentInited = false;
	}

	// �ӳ�����������ȡ��������
	void EnvironmentObject::_getEnvironment()
	{
		SceneManager *mgr = m_scene->getOgreSceneManager();
		// ��պ�����
		m_skyBoxEnabled = mgr->isSkyBoxEnabled();					// �Ƿ�����պ�
		//m_skyBoxMaterialName;			// ��պ�����
		m_skyBoxRenderDistance = 50.0f;			// ��Ⱦ����

		// ������
		m_fogMode = mgr->getFogMode();						// ����ʽ
		m_fogColor = mgr->getFogColour();					// ����ɫ
		m_fogDensity = mgr->getFogDensity();						// ���ܶ�
		m_fogStart = mgr->getFogStart();						// ����ʼ����
		m_fogEnd = mgr->getFogEnd();							// ����������

		// ��������
		m_ambientLight = mgr->getAmbientLight();				// ��������ɫ

	}


	// ö���������ԣ���������ʽ
	void EnvironmentObject::enumPropertiesTree(VTProperty &properties)const
	{
		NullObject::enumPropertiesTree(properties);
		properties.push_back(Property( ID_Environment , PT_GROUP , "��������" , "��������" , true , false , false));
		properties.back().addChild(Property( ID_SkyBox , PT_GROUP , "��պ�����" , "��պ�����" , true , false , false));
		properties.back().childs.back().addChild(Property(ID_SkyBoxEnabled , PT_BOOL , "�Ƿ�����պ�" , "�Ƿ�����պ�" , true , true , true));
		properties.back().childs.back().addChild(Property(ID_SkyBoxMaterialName , PT_STRING , "��պ�����" , "��պ�����" , true , true , true));
		properties.back().childs.back().addChild(Property(ID_SkyBoxRenderDistance , PT_REAL , "��Ⱦ����" , "��Ⱦ����" , true , true , true));

		properties.back().addChild(Property( ID_Fog , PT_GROUP , "������" , "������" , true , false , false));
		properties.back().childs.back().addChild(Property(ID_FogMode , PT_UINT , "����ʽ" , "����ʽ" , true , true , true));
		properties.back().childs.back().addChild(Property(ID_FogColor , PT_COLOURVALUE , "����ɫ" , "����ɫ" , true , true , true));
		properties.back().childs.back().addChild(Property(ID_FogExpDensity , PT_REAL , "���ܶ�" , "���ܶ�" , true , true , true));
		properties.back().childs.back().addChild(Property(ID_FogStart , PT_REAL , "����ʼ����" , "����ʼ����" , true , true , true));
		properties.back().childs.back().addChild(Property(ID_FogEnd , PT_REAL , "����������" , "����������" , true , true , true));

		properties.back().addChild(Property( ID_Lighting , PT_GROUP , "��������" , "��������" , true , false , false));
		properties.back().childs.back().addChild(Property(ID_AmbientLight , PT_COLOURVALUE , "��������ɫ" , "��������ɫ" , true , true , true));

	}

	// ��������
	bool EnvironmentObject::setProperty(uint id , const Any &value)
	{
		if(id > ID_NullObject_Begin && id < ID_NullObject_End)
			return NullObject::setProperty(id , value);
		switch(id)
		{
		case ID_Environment:						// ��������
			{
				return true;
			}
			break;
		case ID_SkyBox:							// ��պ�����
			{
				return true;
			}
			break;
		case ID_SkyBoxEnabled:					// �Ƿ�����պ�
			{
				bool skyBoxEnabled = any_cast<bool>(value);
				if(skyBoxEnabled != m_skyBoxEnabled)
				{
					m_skyBoxEnabled = skyBoxEnabled;
					_setSky();
				}
				return true;
			}
			break;
		case ID_SkyBoxMaterialName:				// ��պ�����
			{
				String skyBoxMaterialName = any_cast<String>(value);
				if(skyBoxMaterialName != m_skyBoxMaterialName)
				{
					m_skyBoxMaterialName = skyBoxMaterialName;
					_setSky();
				}
				return true;
			}
			break;
		case ID_SkyBoxRenderDistance:			// ��Ⱦ����
			{
				Real skyBoxRenderDistance = any_cast<Real>(value);
				if(skyBoxRenderDistance != m_skyBoxRenderDistance)
				{
					m_skyBoxRenderDistance = skyBoxRenderDistance;
					_setSky();
				}
				return true;
			}
			break;

		case ID_Fog:								// ������
			{
				return true;
			}
			break;
		case ID_FogMode:							// ����ʽ
			{
				FogMode fogMode = (FogMode)any_cast<uint>(value);
				if(fogMode != m_fogMode)
				{
					m_fogMode = fogMode;
					_setFog();
				}
				return true;
			}
			break;
		case ID_FogColor:						// ����ɫ
			{
				ColourValue fogColor = any_cast<ColourValue>(value);
				if(fogColor != m_fogColor)
				{
					m_fogColor = fogColor;
					_setFog();
				}
				return true;
			}
			break;
		case ID_FogExpDensity:					// ���ܶ�
			{
				Real fogDensity = any_cast<Real>(value);
				if(fogDensity != m_fogDensity)
				{
					m_fogDensity = fogDensity;
					_setFog();
				}
				return true;
			}
			break;
		case ID_FogStart:						// ����ʼ����
			{
				Real fogStart = any_cast<Real>(value);
				if(fogStart != m_fogStart)
				{
					m_fogStart = fogStart;
					_setFog();
				}
				return true;
			}
			break;
		case ID_FogEnd:							// ����������
			{
				Real fogEnd = any_cast<Real>(value);
				if(fogEnd != m_fogEnd)
				{
					m_fogEnd = fogEnd;
					_setFog();
				}
				return true;
			}
			break;

		case ID_Lighting:						// ��������
			{
				return true;
			}
			break;
		case ID_AmbientLight:					// ��������ɫ
			{
				ColourValue ambientLight = any_cast<ColourValue>(value);
				if(ambientLight != m_ambientLight)
				{
					m_ambientLight = ambientLight;
					_setLighting();
				}
				return true;
			}
			break;
		default:
			return false;
			break;
		}
	}
	// ��ȡ����ֵ
	Any EnvironmentObject::getProperty(uint id)const
	{
		switch(id)
		{
		case ID_Environment:						// ��������
			{
				return Any();
			}
			break;
		case ID_SkyBox:							// ��պ�����
			{
				return Any();
			}
			break;
		case ID_SkyBoxEnabled:					// �Ƿ�����պ�
			{
				return Any(m_skyBoxEnabled);
			}
			break;
		case ID_SkyBoxMaterialName:				// ��պ�����
			{
				return Any(m_skyBoxMaterialName);
			}
			break;
		case ID_SkyBoxRenderDistance:			// ��Ⱦ����
			{
				return Any(m_skyBoxRenderDistance);
			}
			break;

		case ID_Fog:								// ������
			{
				return Any();
			}
			break;
		case ID_FogMode:							// ����ʽ
			{
				return Any((uint)m_fogMode);
			}
			break;
		case ID_FogColor:						// ����ɫ
			{
				return Any(m_fogColor);
			}
			break;
		case ID_FogExpDensity:					// ���ܶ�
			{
				return Any(m_fogDensity);
			}
			break;
		case ID_FogStart:						// ����ʼ����
			{
				return Any(m_fogStart);
			}
			break;
		case ID_FogEnd:							// ����������
			{
				return Any(m_fogEnd);
			}
			break;

		case ID_Lighting:						// ��������
			{
				return Any();
			}
			break;
		case ID_AmbientLight:					// ��������ɫ
			{
				return Any(m_ambientLight);
			}
			break;
		default:
			if(id > ID_NullObject_Begin && id < ID_NullObject_End)
				return NullObject::getProperty(id);
			else
				return Any();
			break;
		}
	}
	// �����Ƿ���ʾ
	void EnvironmentObject::setVisible(bool visible)
	{
		if(m_visible == visible && !m_environmentInited)
			return;

		if(visible)
		{
			_setEnvironment();
			m_environmentInited = true;
		}
		else
		{
			_disableEnvironment();
		}		
	}
	// �������
	void EnvironmentObject::_setSky()
	{
		if(m_visible)
		{
			m_scene->getOgreSceneManager()->setSkyBox(!m_skyBoxMaterialName.empty() && m_skyBoxEnabled , m_skyBoxMaterialName , m_skyBoxRenderDistance);
		}
	}
	// ������
	void EnvironmentObject::_setFog()
	{
		if(m_visible)
		{
			m_scene->getOgreSceneManager()->setFog(m_fogMode , m_fogColor , m_fogDensity , m_fogStart , m_fogEnd);
		}
	}
	// ���ù���
	void EnvironmentObject::_setLighting()
	{
		if(m_visible)
		{
			m_scene->getOgreSceneManager()->setAmbientLight(m_ambientLight);
		}
	}

	// ���û���
	void EnvironmentObject::_disableEnvironment()
	{
		if(m_scene && m_visible)
		{
			m_scene->getOgreSceneManager()->setSkyBox(false , StringUtil::BLANK);
			m_scene->getOgreSceneManager()->setFog(FOG_NONE);
			m_scene->getOgreSceneManager()->setAmbientLight(ColourValue(0.5f , 0.5f , 0.5f));
			m_visible = false;
		}
	}

	// ���û���
	void EnvironmentObject::_setEnvironment()
	{
		m_visible = true;
		_setFog();
		_setSky();
		_setLighting();
	}

	// ����������ԣ���������ʽ
	const PropertyContainer::VTProperty* EnvironmentObject::getPropertiesTree()const
	{
		static VTProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesTree(s_properties);
		}
		return &s_properties;
	}
	// ����������ԣ����б����ʽ
	const PropertyContainer::MapProperty* EnvironmentObject::getPropertiesList()const
	{
		static MapProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesList(s_properties);
		}
		return &s_properties;
	}

	// ��ó�������Ŀɼ����������ڱ༭����ѡ��
	ISceneObjectVisible *EnvironmentObject::getSceneObjectVisible()
	{
		if(!m_sceneObjectVisible)
		{
			m_sceneObjectVisible = new EnvironmentObjectVisible(this);
		}
		return m_sceneObjectVisible;
	}
};