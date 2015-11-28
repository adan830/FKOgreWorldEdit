// ============================================================
// �ļ���	:	WorldEditor.h
// ������	:	������
// ˵��		:	�༭�������������༭���Ľӿ�ʵ��
// ����ʱ��	:	2008-1-20 23:29:11
// ============================================================
#pragma once
#include "IWorldEditor.h"
namespace Ogre
{
	// �༭�������������༭���Ľӿ�ʵ��
	class WorldEditor : public IWorldEditor
		, public Singleton<WorldEditor>
	{
	public:
		WorldEditor(IMainFrame *mainFrm);
		virtual ~WorldEditor(void);
		// ����ģʽ�ӿ�
		static WorldEditor& getSingleton(void);
		static WorldEditor* getSingletonPtr(void);

		// ��ʼ���༭��ϵͳ
		void init(HWND hWnd);

		// ���ٱ༭��ϵͳ
		void shutdown();

		// �Ƿ��Ѿ���ʼ��
		bool isInited()const
		{
			return m_inited;
		}

		// �����ʾ��
		virtual DisplaySystem *getDisplaySystem();
		// ��ò��������
		virtual IPluginManager *getPluginManager();
		// ���������
		virtual IMainFrame *getMainFrame();
		// ������ù�����
		virtual ISettingManager *getSettingManager();
		// ��õ�ͼ������
		virtual IMapManager *getMapManager();
		// ��ü��а������
		virtual IClipboardManager *getClipboardManager();

	protected:
		bool m_inited;

		// ���������
		IPluginManager *m_pluginManager;
		// ��ʾϵͳ
		DisplaySystem *m_displaySystem;
		// ���ù�����
		ISettingManager *m_settingManager;
		// ��ͼ������
		IMapManager *m_mapManager;
		// ���а������
		IClipboardManager *m_clipboardManager;
		// ������
		IMainFrame *m_mainFrm;


	};

};