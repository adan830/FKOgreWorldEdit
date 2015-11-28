#include "StdAfx.h"
#include "PluginManager.h"
#include "IPlugin.h"
#include "OgreDynLibManager.h"
#include "OgreDynLib.h"
#include "WorldEditor.h"
#include "DlgPluginManager.h"
#include "SettingManager.h"

namespace Ogre
{
	// ����ģʽʵ��
	template<> PluginManager* Singleton<PluginManager>::ms_Singleton = 0;
	PluginManager* PluginManager::getSingletonPtr(void)
	{
		return ms_Singleton;
	}
	PluginManager& PluginManager::getSingleton(void)
	{  
		assert( ms_Singleton );  return ( *ms_Singleton );  
	}

	typedef IPlugin* (*DLL_GET_PLUGIN)();

	PluginManager::PluginManager(void)
	{
	}

	PluginManager::~PluginManager(void)
	{
	}

	// ����Ŀ¼���������в�������������˵Ĳ������
	size_t PluginManager::loadPlugins(const String &pathName)
	{
		String strOldEnv(_tgetenv("Path"));
		// �Ѳ��Ŀ¼���뵽�����������Ա������ϵͳ����DLLʱ�ҵ������������DLL
		if(_tputenv(("Path=" + strOldEnv + ";" + pathName).c_str()) == -1)
			return 0;
		CFileFind finder;
		BOOL bWorking = finder.FindFile((pathName + "\\*.dll").c_str());
		size_t count = 0;
		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			// ����һ�����
			if(loadPlugin((LPCSTR)finder.GetFilePath()) != 0)
			{
				count ++;
			}
		}
		// ��ʼ�����в��
		initAllPlugins();

		return count;
	}
	// ��ʼ�����в��
	void PluginManager::initAllPlugins()
	{
		// �Զ�������б�
		StringVector autoLoads = SettingManager::getSingleton().getStringVector("�Զ������б�" , StringVector() , "���");
		typedef std::list<IPlugin*> ListPlugin;
		ListPlugin loads;
		ListPlugin endLoads;
		// ��������Ĳ��
		for(MapPlugin::iterator iter = m_mapPlugin.begin() ; iter != m_mapPlugin.end() ; iter ++)
		{
			IPlugin *plugin = iter->second;
			// ֻ����Ҫ�Զ������load
			if(std::count(autoLoads.begin() , autoLoads.end() , plugin->getDescription().fileName) > 0)
			{
				loads.push_back(plugin);
				endLoads.push_back(plugin);
			}
		}
		// ����
		for(ListPlugin::iterator iter = loads.begin() ; iter != loads.end() ; iter ++)
		{
			IPlugin *plugin = (*iter);
			const IPlugin::Description &desc = plugin->getDescription();
			// ѭ�����������б�
			for(size_t i = 0 ; i < desc.dependentPlugins.size() ; i ++)
			{
				ListPlugin::iterator dependentPlugin = loads.end();
				// ������������Ƿ��Ѿ�����
				for(ListPlugin::iterator iterFind = loads.begin() ; iterFind != loads.end() ; iterFind ++)
				{
					if((*iterFind)->getDescription().name == desc.dependentPlugins[i])
					{
						dependentPlugin = iterFind;
						break;
					}
				}
				// ���û���룬��ô������Ͳ���������
				if(dependentPlugin == loads.end())
				{
					endLoads.remove(plugin);
					break;
				}
				// ���򣬾Ͱѱ�������뵽���������ĺ���
				else
				{
					ListPlugin::iterator iterDependent = std::find(endLoads.begin() , endLoads.end() , *dependentPlugin);
					endLoads.splice(++std::find(endLoads.begin() , endLoads.end() , *dependentPlugin)
						, endLoads
						, std::find(endLoads.begin() , endLoads.end() , plugin)
						);
				}
			}
		}

		// ��˳������
		for(ListPlugin::iterator iter = endLoads.begin() ; iter != endLoads.end() ; iter ++)
		{
			(*iter)->init(WorldEditor::getSingletonPtr());
		}
	}

	// �����ļ�������һ�����
	IPlugin *PluginManager::loadPlugin(const String &fileName)
	{
		// ����DLL
		DynLib* lib = DynLibManager::getSingleton().load(fileName);
		if(!lib)
		{
			return 0;
		}
		// �����������
		DLL_GET_PLUGIN pFunc = (DLL_GET_PLUGIN)lib->getSymbol("getPlugin");
		if(!pFunc)
		{
			DynLibManager::getSingleton().unload(lib);
			return 0;
		}

		// ����֮
		IPlugin *plugin = pFunc();
		// ����ʧ�ܾͷ���
		if(!plugin)
		{
			DynLibManager::getSingleton().unload(lib);
			return 0;
		}
		// ����ָ��
		m_mapDynLib[plugin->getDescription().name] = lib;
		m_mapPlugin[plugin->getDescription().name] = plugin;
		return plugin;
	}

	// ж��һ�����
	void PluginManager::unloadPlugin(IPlugin *plugin)
	{
		unloadPlugin(plugin->getDescription().name);
	}
	// ��������ж��һ�����
	void PluginManager::unloadPlugin(const String &name)
	{
		// �رղ��
		MapPlugin::iterator iter = m_mapPlugin.find(name);
		if(iter == m_mapPlugin.end())
			return;
		// ɾ��������
		for(ListRenderTarget::iterator iterTarget = m_listRenderTarget.begin() ; iterTarget != m_listRenderTarget.end() ; iterTarget ++)
		{
			(*iterTarget)->removeListener(iter->second);
		}
		iter->second->shutdown();
		m_mapPlugin.erase(iter);

		// ж��DLL
		MapDynLib::iterator iterDynLib = m_mapDynLib.find(name);
		if(iterDynLib == m_mapDynLib.end())
			return;
		DynLibManager::getSingleton().unload(iterDynLib->second);
		m_mapDynLib.erase(iterDynLib);
	}

	// ж�����в��
	void PluginManager::unloadAllPlugins()
	{
		// �ر����в��
		for(MapPlugin::iterator iter = m_mapPlugin.begin() ; iter != m_mapPlugin.end() ; iter ++)
		{
			iter->second->shutdown();
		}
		m_mapPlugin.clear();
		// ж��DLL
		for(MapDynLib::iterator iterDynLib = m_mapDynLib.begin() ; iterDynLib != m_mapDynLib.end() ; iterDynLib ++)
		{
			DynLibManager::getSingleton().unload(iterDynLib->second);
		}
		m_mapDynLib.clear();
	}

	// �������ƻ��һ�����
	IPlugin *PluginManager::getPlugin(const String &name)
	{
		MapPlugin::iterator iter = m_mapPlugin.find(name);
		if(iter == m_mapPlugin.end())
			return 0;
		return iter->second;
	}

	// ��ò���б�
	PluginManager::PluginIterator PluginManager::getPluginIterator()
	{
		return PluginIterator(m_mapPlugin.begin() , m_mapPlugin.end());
	}


	// ֪ͨ�и���ȾĿ�������
	void PluginManager::_notifyRenderTargetAdded(RenderTarget *target)
	{
		for(MapPlugin::iterator iter = m_mapPlugin.begin() ; iter != m_mapPlugin.end() ; iter ++)
		{
			if(iter->second->isInited())
			{
				target->addListener(iter->second);
			}
		}		
		m_listRenderTarget.push_back(target);
	}
	// ֪ͨ�и���ȾĿ��ɾ����
	void PluginManager::_notifyRenderTargetRemove(RenderTarget *target)
	{
		for(MapPlugin::iterator iter = m_mapPlugin.begin() ; iter != m_mapPlugin.end() ; iter ++)
		{
			if(iter->second->isInited())
			{
				target->removeListener(iter->second);
			}
		}
		m_listRenderTarget.remove(target);
	}


	// ��ʾ����������Ի���
	void PluginManager::showPluginManagerDlg()
	{
		CDlgPluginManager dlg(WorldEditor::getSingleton().getMainFrame()->getWnd());
		dlg.DoModal();
	}

};