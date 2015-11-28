#include "StdAfx.h"
#include "ClipboardManager.h"
namespace Ogre
{

	ClipboardManager::ClipboardManager(void)
	{
	}

	ClipboardManager::~ClipboardManager(void)
	{
	}

	// �����ݱ��浽���а�
	void ClipboardManager::save(const void *dataPtr , size_t dataSize , const String &clipboardFormat)
	{
		save(dataPtr , dataSize , RegisterClipboardFormat(clipboardFormat.c_str()));
	}
	// �Ӽ������ȡ����
	size_t ClipboardManager::load(void *dataPtr , const String &clipboardFormat)
	{
		return load(dataPtr , RegisterClipboardFormat(clipboardFormat.c_str()));
	}
	// �鿴������������ݴ�С
	size_t ClipboardManager::getSize(const String &clipboardFormat)
	{
		return getSize(RegisterClipboardFormat(clipboardFormat.c_str()));
	}

	// �����ݱ��浽���а�
	void ClipboardManager::save(const void *dataPtr , size_t dataSize , uint clipboardFormat)
	{
		if(dataSize == 0) return;

		HANDLE hGlobalMemory = GlobalAlloc(GHND, dataSize); // �����ڴ�
		LPVOID lpGlobalMemory = GlobalLock(hGlobalMemory); // �����ڴ�

		memcpy(lpGlobalMemory , dataPtr , dataSize);

		GlobalUnlock(hGlobalMemory);	//�����ڴ�����
		HWND hWnd = GetActiveWindow();	//��ȡ��ȫ���ھ��
		::OpenClipboard(hWnd);			//�򿪼�����
		::EmptyClipboard();				//��ռ�����

		::SetClipboardData(clipboardFormat, hGlobalMemory);//���ڴ��е����ݷ��õ�������
		::CloseClipboard();				//�رռ�����	
	}

	// �Ӽ������ȡ����
	size_t ClipboardManager::load(void *dataPtr , uint clipboardFormat)
	{
		HWND hWnd = ::GetActiveWindow();	// ��ȡ��ȫ���ھ��
		::OpenClipboard(hWnd);				// �򿪼�����
		HANDLE hClipMemory = ::GetClipboardData(clipboardFormat);		// ��ȡ���������ݾ��
		size_t dwLength = 0;
		if(hClipMemory)
		{
			dwLength = (size_t)GlobalSize(hClipMemory);				// ����ָ���ڴ�����ĵ�ǰ��С
			if(dwLength > 0)
			{
				LPVOID lpClipMemory = GlobalLock(hClipMemory);	// �����ڴ�
				memcpy(dataPtr , lpClipMemory , dwLength);
				GlobalUnlock(hClipMemory);	// �ڴ����
			}
		}
		::CloseClipboard();			// �رռ�����	

		return dwLength;

	}
	// �鿴������������ݴ�С
	size_t ClipboardManager::getSize(uint clipboardFormat)
	{
		HWND hWnd = ::GetActiveWindow();	// ��ȡ��ȫ���ھ��
		::OpenClipboard(hWnd);				// �򿪼�����
		HANDLE hClipMemory = ::GetClipboardData(clipboardFormat);		// ��ȡ���������ݾ��
		size_t dwLength = 0;
		if(hClipMemory)
		{
			dwLength = (size_t)GlobalSize(hClipMemory);				// ����ָ���ڴ�����ĵ�ǰ��С
		}
		::CloseClipboard();			// �رռ�����	
		return dwLength;
	}
	// ���ַ������浽���а�
	void ClipboardManager::save(const String &text)
	{
		save(text.c_str() , text.length() + 1 , CF_TEXT);
	}

	// �Ӽ������ȡ�ַ���
	String ClipboardManager::load()
	{
		size_t textSize = getSize(CF_TEXT);
		if(textSize == 0)
			return StringUtil::BLANK;
		typedef SharedPtr<char> CharPtr;
		CharPtr text(new char[textSize + 1]);
		text.getPointer()[textSize] = 0;
		load(text.getPointer() , CF_TEXT);
		return String(text.getPointer());
	}
};