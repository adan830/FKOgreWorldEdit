#pragma once
namespace Ogre
{
	class ITriggerWorld;
	// ������ϵͳ�ӿ�
	class TRIGGERSYSTEM_API ITriggerSystem : public Singleton<ITriggerSystem>
	{
	public:
		ITriggerSystem(void){};
		virtual~ITriggerSystem(void){};

		// ����ģʽ�ӿ�
		static ITriggerSystem& getSingleton(void);
		static ITriggerSystem* getSingletonPtr(void);

		// ��ʼ��������ϵͳ
		static void init();
		// ���ٴ�����ϵͳ
		static void shutdown();

		// ����һ������������
		virtual ITriggerWorld *createWorld(const String &name) = 0;
		// ���һ������������
		virtual ITriggerWorld *getWorld(const String &name) = 0;
		// �Ƿ���������ƵĴ���������
		virtual bool hasWorld(const String &name) = 0;
		// ����һ������������
		virtual void destroyWorld(const String &name) = 0;
		// ����һ������������
		virtual void destroyWorld(ITriggerWorld *world) = 0;
	};
};