// ============================================================
// �ļ���	:	PluginManager.h
// ������	:	������
// ˵��		:	���������ʵ��
// ����ʱ��	:	2008-1-22 7:43:06
// ============================================================
#pragma once
#include "IPluginManager.h"

namespace Ogre
{
	// ���������ʵ��
	class PluginManager : public IPluginManager
		, public Singleton<PluginManager>
	{
	public:
		PluginManager(void);
		virtual ~PluginManager(void);
		// ����ģʽ�ӿ�
		static PluginManager& getSingleton(void);
		static PluginManager* getSingletonPtr(void);


		// ��ʾ����������Ի���
		virtual void showPluginManagerDlg();

		// ����Ŀ¼���������в�������������˵Ĳ������
		virtual size_t loadPlugins(const String &pathName);

		// �����ļ�������һ�����
		virtual IPlugin *loadPlugin(const String &fileName);

		// ж��һ�����
		virtual void unloadPlugin(IPlugin *plugin);
		// ��������ж��һ�����
		virtual void unloadPlugin(const String &name);

		// ж�����в��
		virtual void unloadAllPlugins();


		// �������ƻ��һ�����
		virtual IPlugin *getPlugin(const String &name);

		// ��ò���б�
		virtual PluginIterator getPluginIterator();

		// ֪ͨ�и���ȾĿ�������
		virtual void _notifyRenderTargetAdded(RenderTarget *target);
		// ֪ͨ�и���ȾĿ��ɾ����
		virtual void _notifyRenderTargetRemove(RenderTarget *target);
	protected:
		// ��ʼ�����в��
		void initAllPlugins();

		MapPlugin m_mapPlugin;

		typedef std::map<String , DynLib*> MapDynLib;
		MapDynLib m_mapDynLib;

		typedef std::list<RenderTarget*> ListRenderTarget;
		ListRenderTarget m_listRenderTarget;
	};
};