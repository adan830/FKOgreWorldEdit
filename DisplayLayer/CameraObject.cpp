#include "StdAfx.h"
#include "CameraObject.h"
#include "IScene.h"
#include "ISceneSystem.h"
#include "CameraObjectVisible.h"
namespace Ogre
{

	const String CameraObjectFactory::FACTORY_TYPE = "CameraObject";

	// ��øù�������������
	const String &CameraObjectFactory::getType() const
	{
		return FACTORY_TYPE;
	}
	// ��������Ĵ�������
	ISceneObject *CameraObjectFactory::create(const String &name , IScene *scene , const NameValuePairList* params)
	{
		CameraObject *obj = new CameraObject(name);
		obj->create(scene , params);
		return obj;
	}

	// ������������ٷ���
	void CameraObjectFactory::destroy(ISceneObject *obj)
	{
		OGRE_DELETE(obj);
	}

	CameraObject::CameraObject(const String &name)
		: NullObject(name)
		, m_camera(0)
	{
	}

	CameraObject::~CameraObject(void)
	{
		destroy();
	}

	// ��������ʵ��
	void CameraObject::create(IScene *scene , const NameValuePairList* params)
	{
		destroy();
		NullObject::create(scene , params);
		// ����ʵ��
		m_camera = static_cast<Camera*>(m_scene->getOgreSceneManager()->createCamera(getFullName()));
		m_camera->setUserAny(Any(static_cast<ISceneObject*>(this)));
		// �ҽ���ȥ
		m_node->attachObject(m_camera);
	}

	// ���ٶ���ʵ��
	void CameraObject::destroy()
	{
		if(m_camera)
		{
			// ����ģ��ʵ��
			m_scene->getOgreSceneManager()->destroyCamera(m_camera);
			m_camera = 0;
		}
		NullObject::destroy();
	}

	// ���������
	// @x ���x��Ļ���꣨0~1��
	// @y ���y��Ļ���꣨0~1��
	// @pos ����ཻ������
	// @return �Ƿ��ཻ��
	bool CameraObject::hitTest(float x , float y , Vector3 &pos)
	{
		if(!m_camera)
		{
			return false;
		}
		else
		{
			return m_scene->collide(m_camera->getCameraToViewportRay(x , y) , true ,true , pos);
		}
	}


	// ö���������ԣ���������ʽ
	void CameraObject::enumPropertiesTree(VTProperty &properties)const
	{
		NullObject::enumPropertiesTree(properties);
		properties.push_back(Property( ID_Camera , PT_GROUP , "���������" , "���������" , true , false , false));
		properties.back().addChild(Property(ID_PolygonMode , PT_LONG , "������Ⱦģʽ" , "������Ⱦģʽ" , true , true , true));
		properties.back().addChild(Property(ID_LodBias , PT_REAL , "LODƫ��" , "LODƫ��" , true , true , true));
		properties.back().addChild(Property(ID_NearClipDistance , PT_REAL , "������" , "������" , true , true , true));
		properties.back().addChild(Property(ID_FarClipDistance , PT_REAL , "Զ����" , "Զ����" , true , true , true));
		properties.back().addChild(Property(ID_FOVy , PT_REAL , "FOVy�Ƕ�" , "FOVy�Ƕ�" , true , true , true));
		properties.back().addChild(Property(ID_AspectRatio , PT_REAL , "���ڱ���" , "���ڱ���" , true , true , true));
		properties.back().addChild(Property(ID_ProjectionType , PT_LONG , "ͶӰ��ʽ" , "ͶӰ��ʽ" , true , true , true));
		properties.back().addChild(Property(ID_OrthoWindow , PT_VECTOR2 , "����ͶӰ���ڴ�С" , "����ͶӰ���ڴ�С" , true , true , true));
	}

	// ��������
	bool CameraObject::setProperty(uint id , const Any &value)
	{
		if(id > ID_NullObject_Begin && id < ID_NullObject_End)
			return NullObject::setProperty(id , value);
		switch(id)
		{
		case ID_Camera:							// ���������
			{
				return true;
			}
			break;
		case ID_PolygonMode:						// ������Ⱦģʽ
			{
				m_camera->setPolygonMode((PolygonMode)any_cast<long>(value));
				return true;
			}
			break;
		case ID_LodBias:							// LODƫ��
			{
				m_camera->setLodBias(any_cast<Real>(value));
				return true;
			}
			break;
		case ID_NearClipDistance:				// ������
			{
				m_camera->setNearClipDistance(any_cast<Real>(value));
				return true;
			}
			break;
		case ID_FarClipDistance:					// Զ����
			{
				m_camera->setFarClipDistance(any_cast<Real>(value));
				return true;
			}
			break;
		case ID_FOVy:							// FOVy�Ƕ�
			{
				m_camera->setFOVy(Degree(any_cast<Real>(value)));
				return true;
			}
			break;
		case ID_AspectRatio:						// ���ڱ���
			{
				m_camera->setAspectRatio(any_cast<Real>(value));
				return true;
			}
			break;
		case ID_ProjectionType:					// ͶӰ��ʽ
			{
				m_camera->setProjectionType((ProjectionType)any_cast<long>(value));
				return true;
			}
			break;
		case ID_OrthoWindow:						// ����ͶӰ���ڴ�С
			{
				Vector2 window(any_cast<Vector2>(value));
				m_camera->setOrthoWindow(window.x , window.y);
				return true;
			}
			break;
		default:
			return false;
			break;
		}
	}
	// ��ȡ����ֵ
	Any CameraObject::getProperty(uint id)const
	{
		if(id > ID_NullObject_Begin && id < ID_NullObject_End)
			return NullObject::getProperty(id);
		switch(id)
		{
		case ID_Camera:							// ���������
			{
				return Any();
			}
			break;
		case ID_PolygonMode:						// ������Ⱦģʽ
			{
				return Any((long)m_camera->getPolygonMode());
			}
			break;
		case ID_LodBias:							// LODƫ��
			{
				return Any(m_camera->getLodBias());
			}
			break;
		case ID_NearClipDistance:				// ������
			{
				return Any(m_camera->getNearClipDistance());
			}
			break;
		case ID_FarClipDistance:					// Զ����
			{
				return Any(m_camera->getFarClipDistance());
			}
			break;
		case ID_FOVy:							// FOVy�Ƕ�
			{
				return Any(m_camera->getFOVy().valueDegrees());
			}
			break;
		case ID_AspectRatio:						// ���ڱ���
			{
				return Any(m_camera->getAspectRatio());
			}
			break;
		case ID_ProjectionType:					// ͶӰ��ʽ
			{
				return Any((long)m_camera->getProjectionType());
			}
			break;
		case ID_OrthoWindow:						// ����ͶӰ���ڴ�С
			{
				return Any(Vector2(m_camera->getOrthoWindowWidth() , m_camera->getOrthoWindowHeight()));
			}
			break;
		default:
			return Any();
			break;
		}
	}

	// ����������ԣ���������ʽ
	const PropertyContainer::VTProperty* CameraObject::getPropertiesTree()const
	{
		static VTProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesTree(s_properties);
		}
		return &s_properties;
	}
	// ����������ԣ����б����ʽ
	const PropertyContainer::MapProperty* CameraObject::getPropertiesList()const
	{
		static MapProperty s_properties;
		if(s_properties.empty())
		{
			enumPropertiesList(s_properties);
		}
		return &s_properties;
	}

	// ��ó�������Ŀɼ����������ڱ༭����ѡ��
	ISceneObjectVisible *CameraObject::getSceneObjectVisible()
	{
		if(!m_sceneObjectVisible)
		{
			m_sceneObjectVisible = new CameraObjectVisible(this);
		}
		return m_sceneObjectVisible;
	}

};