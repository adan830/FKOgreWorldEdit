// ============================================================
// �ļ���	:	IClipboardManager.h
// ������	:	������
// ˵��		:	���а��������ʵ��
// ����ʱ��	:	2008-2-29 22:31:47
// ============================================================
#pragma once
#include "IClipboardManager.h"
namespace Ogre
{
	// ���а�������Ľӿ�
	class ClipboardManager : public IClipboardManager
	{
	public:
		ClipboardManager(void);
		virtual ~ClipboardManager(void);


		// �����ݱ��浽���а�
		virtual void save(const void *dataPtr , size_t dataSize , const String &clipboardFormat);
		// �Ӽ������ȡ����
		virtual size_t load(void *dataPtr , const String &clipboardFormat);
		// �鿴������������ݴ�С
		virtual size_t getSize(const String &clipboardFormat);

		// �����ݱ��浽���а�
		virtual void save(const void *dataPtr , size_t dataSize , uint clipboardFormat);
		// �Ӽ������ȡ����
		virtual size_t load(void *dataPtr , uint clipboardFormat);
		// �鿴������������ݴ�С
		virtual size_t getSize(uint clipboardFormat);

		// ���ַ������浽���а�
		virtual void save(const String &text);
		// �Ӽ������ȡ�ַ���
		virtual String load();

	};
};