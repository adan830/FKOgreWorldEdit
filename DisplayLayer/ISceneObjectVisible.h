#pragma once
namespace Ogre
{
	class ISceneObject;
	// ��������Ŀɼ����������ڱ༭����ѡ��
	class ISceneObjectVisible
	{
	public:

		ISceneObjectVisible(void)
		{
		}

		virtual ~ISceneObjectVisible(void)
		{
		}

		// ��ó�������
		virtual ISceneObject *getSceneObject()const = 0;

		// ��ʾ�߿�
		virtual void showBorder(bool show) = 0;
		// �Ƿ���ʾ�߿�
		virtual bool isShowBorder() const = 0;

		// ��ʾʵ��
		virtual void showEntity(bool show) = 0;
		// �Ƿ���ʾʵ��
		virtual bool isShowEntity() const = 0;

		// ��ʾ���ڰڷŵ�ģ��
		virtual void showPlacingEntity(bool show) = 0;
		// �Ƿ���ʾ���ڰڷŵ�ģ��
		virtual bool isShowPlacingEntity() const = 0;

		// �ؽ��ɼ�����
		virtual void _notifyRebuild() = 0;

		// ��ÿɼ��������ײʵ��
		virtual ICollisionEntity *getCollisionEntity()const = 0;
	};
};