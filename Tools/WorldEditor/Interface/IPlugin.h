// ============================================================
// �ļ���	:	IPlugin.h
// ������	:	������
// ˵��		:	�༭������Ľӿڣ��༭�������Ҫ������̳�
// ����ʱ��	:	2008-1-20 22:45:05
// ============================================================
#pragma once

namespace Ogre
{
	class IWorldEditor;
	// �༭������Ľӿڣ��༭�������Ҫ������̳�
	// �������4��ͼ��RenderTarget�¼�
	class IPlugin : public RenderTargetListener
	{
	public:

		// �������
		struct Description
		{
			Description()
				: version(0)
			{

			};
			// �������
			String name;
			// ����ļ���
			String fileName;
			// ���˵��
			String desc;
			// ���������Ϣ
			String publish;
			// ���������Ĳ�������б�
			StringVector dependentPlugins;
			// �汾��Ϣ
			uint version;
		};

		IPlugin(void)
		{
		}

		virtual ~IPlugin(void)
		{
		}

		// ��ʼ���������������worldEditor������༭���Ľӿ�
		virtual bool init(IWorldEditor *worldEditor) = 0;
		// �رղ��
		virtual void shutdown() = 0;
		// �Ƿ��Ѿ���ʼ��
		virtual bool isInited()const = 0;

		// ������
		virtual void activate() = 0;
		// ��������
		virtual void deactivate() = 0;
		// �Ƿ��Ѿ�����
		virtual bool isActivate()const = 0;

		// ��ò������
		virtual const Description &getDescription()const = 0;

		// ��ѯһЩ���ݣ�Ȼ�󷵻ص�result��
		virtual bool query(const String &name , const NameValuePairList *params , void *result) = 0;
	};

};