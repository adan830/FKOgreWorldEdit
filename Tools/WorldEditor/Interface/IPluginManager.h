// ============================================================
// �ļ���	:	IPluginManager.h
// ������	:	������
// ˵��		:	����������Ľӿ�
// ����ʱ��	:	2008-1-20 22:31:47
// ============================================================
#pragma once

namespace Ogre
{
	class IPlugin;
	// ����������Ľӿ�
	class IPluginManager
	{
	public:

		IPluginManager(void)
		{
		}

		virtual ~IPluginManager(void)
		{
		}

		// ��ʾ����������Ի���
		virtual void showPluginManagerDlg() = 0;

		// ����Ŀ¼����֧��ͨ�������c:\\plugins\\*.dll���������в�������������˵Ĳ������
		virtual size_t loadPlugins(const String &pathName) = 0;

		// �����ļ�������һ�����
		virtual IPlugin *loadPlugin(const String &fileName) = 0;

		// ж��һ�����
		virtual void unloadPlugin(IPlugin *plugin) = 0;
		// ��������ж��һ�����
		virtual void unloadPlugin(const String &name) = 0;

		// ж�����в��
		virtual void unloadAllPlugins() = 0;
		
		// �������ƻ��һ�����
		virtual IPlugin *getPlugin(const String &name) = 0;
		
		typedef std::map<String , IPlugin*> MapPlugin;
		typedef MapIterator<MapPlugin> PluginIterator;
		// ��ò���б�
		virtual PluginIterator getPluginIterator() = 0;

		// ֪ͨ�и���ȾĿ�������
		virtual void _notifyRenderTargetAdded(RenderTarget *target) = 0;
		// ֪ͨ�и���ȾĿ��ɾ����
		virtual void _notifyRenderTargetRemove(RenderTarget *target) = 0;
	};
};