// ============================================================
// �ļ���	:	IWorldEditor.h
// ������	:	������
// ˵��		:	�༭�������������༭���Ľӿ�
// ����ʱ��	:	2008-1-20 22:31:47
// ============================================================
#pragma once
#include "IPluginManager.h"
#include "IMainFrame.h"
#include "IChildView.h"
#include "IPlugin.h"
#include "ISettingManager.h"
#include "IMapManager.h"
#include "IClipboardManager.h"
namespace Ogre
{
	// �༭�������������༭���Ľӿ�
	class IWorldEditor
	{
	public:

		IWorldEditor(void)
		{
		}

		virtual ~IWorldEditor(void)
		{
		}

		// ������ù�����
		virtual ISettingManager *getSettingManager() = 0;
		// ��ò��������
		virtual IPluginManager *getPluginManager() = 0;
		// �����ʾ��
		virtual DisplaySystem *getDisplaySystem() = 0;
		// ���������
		virtual IMainFrame *getMainFrame() = 0;
		// ��õ�ͼ������
		virtual IMapManager *getMapManager() = 0;
		// ��ü��а������
		virtual IClipboardManager *getClipboardManager() = 0;

	};
};