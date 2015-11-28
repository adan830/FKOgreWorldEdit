#pragma once
#include "NullObject.h"
namespace Ogre
{
	class ETTerrain;
	// ��������Ķ��󹤳�
	class DISPLAYLAYER_API TerrainObjectFactory : public ISceneObjectFactory
	{
	public:
		static const String FACTORY_TYPE;
		// ��øù�������������
		virtual const String &getType() const;
		// ��������Ĵ�������
		virtual ISceneObject *create(const String &name , IScene *scene , const NameValuePairList* params);
		// ������������ٷ���
		virtual void destroy(ISceneObject *obj);
	};

	// ���ζ���
	class DISPLAYLAYER_API TerrainObject : public NullObject
	{
	public:
		TerrainObject(const String &name);
		virtual ~TerrainObject(void);
		// ��ö�������
		virtual const String &getType()const
		{
			return TerrainObjectFactory::FACTORY_TYPE;
		}
		ETTerrain *getOgreTerrain()const
		{
			return m_terrain;
		}
		// ��������ʵ��
		virtual void create(IScene *scene , const NameValuePairList* params);
		// ���ٶ���ʵ��
		virtual void destroy();
		// ���MovableObject
		virtual MovableObject *getOgreMovableObject()const;


		// ��������
		virtual bool setProperty(uint id , const Any &value);
		// ��ȡ����ֵ
		virtual Any getProperty(uint id)const;
		// ����������ԣ���������ʽ
		virtual const VTProperty* getPropertiesTree()const;
		// ����������ԣ����б����ʽ
		virtual const MapProperty* getPropertiesList()const;

		// ����ļ���
		const String &getFileName()const
		{
			return m_fileName;
		}
		// ������ε��ļ�
		void save(const String &fileName);
	protected:
		// ö���������ԣ���������ʽ
		virtual void enumPropertiesTree(VTProperty &properties)const;
		// ����
		ETTerrain *m_terrain;

		// �����ļ���
		String m_fileName;
	};


};