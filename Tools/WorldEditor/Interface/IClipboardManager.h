// ============================================================
// �ļ���	:	IClipboardManager.h
// ������	:	������
// ˵��		:	���а�������Ľӿ�
// ����ʱ��	:	2008-2-29 22:31:47
// ============================================================
#pragma once

namespace Ogre
{
	// ���а�������Ľӿ�
	class IClipboardManager
	{
	public:

		IClipboardManager(void)
		{
		}

		virtual ~IClipboardManager(void)
		{
		}

		// �����ݱ��浽���а�
		virtual void save(const void *dataPtr , size_t dataSize , const String &clipboardFormat) = 0;
		// �Ӽ������ȡ����
		virtual size_t load(void *dataPtr , const String &clipboardFormat) = 0;
		// �鿴������������ݴ�С
		virtual size_t getSize(const String &clipboardFormat) = 0;

		// �����ݱ��浽���а�
		virtual void save(const void *dataPtr , size_t dataSize , uint clipboardFormat) = 0;
		// �Ӽ������ȡ����
		virtual size_t load(void *dataPtr , uint clipboardFormat) = 0;
		// �鿴������������ݴ�С
		virtual size_t getSize(uint clipboardFormat) = 0;


		// ���ַ������浽���а�
		virtual void save(const String &text) = 0;
		// �Ӽ������ȡ�ַ���
		virtual String load() = 0;


	};
};