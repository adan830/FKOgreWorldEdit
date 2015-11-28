#pragma once

namespace Ogre
{
	// ���󹤳���ʵ��ģ��
	template<typename T>
	class TFactorySystem
	{
	public:

		TFactorySystem(void)
		{
		}

		virtual ~TFactorySystem(void)
		{
			removeAllFactory();
		}

		// ���һ������
		virtual void addFactory(T *factory)
		{
			m_mapFactory[factory->getType()] = factory;
		}
		// ɾ��һ������
		virtual void removeFactory(T *factory)
		{
			removeFactory(factory->getType());
		}
		// ɾ��һ������
		virtual void removeFactory(const String &typeName)
		{
			MapFactory::iterator iter = m_mapFactory.find(typeName);
			if(iter != m_mapFactory.end())
			{
				delete iter->second;
				m_mapFactory.erase(iter);
			}
		}
		// ɾ�����й���
		virtual void removeAllFactory()
		{
			for(MapFactory::iterator iter = m_mapFactory.begin() ; iter != m_mapFactory.end() ; iter ++)
			{
				delete iter->second;
			}
			m_mapFactory.clear();
		}
		// ���һ������
		virtual T* getFactory(const String &typeName)const
		{
			MapFactory::const_iterator iter = m_mapFactory.find(typeName);
			if(iter == m_mapFactory.end())
			{
				return 0;
			}
			else
			{
				return iter->second;
			}
		}
		// ��ù��������б�
		virtual StringVector getFactoriesName()const
		{
			StringVector str;
			for(MapFactory::const_iterator iter = m_mapFactory.begin() ; iter != m_mapFactory.end() ; iter ++)
			{
				str.push_back(iter->second->getType());
			}
			return str;
		}
	protected:
		typedef std::map<String , T*> MapFactory;
		// ���󹤳�ע�Ἧ��
		MapFactory m_mapFactory;
	};

};