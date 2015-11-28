#pragma once
#include "ISceneObjectVisible.h"
namespace Ogre
{
	class BoxTriggerObject;
	class IScene;
	// ��������Ŀɼ����������ڱ༭����ѡ��
	class DISPLAYLAYER_API BoxTriggerObjectVisible : public ISceneObjectVisible
	{
	public:
		BoxTriggerObjectVisible(BoxTriggerObject *obj);
		virtual ~BoxTriggerObjectVisible(void);

		// ��ʾ�߿�
		virtual void showBorder(bool show);
		// �Ƿ���ʾ�߿�
		virtual bool isShowBorder() const;

		// ��ʾʵ��
		virtual void showEntity(bool show);
		// �Ƿ���ʾʵ��
		virtual bool isShowEntity() const;

		// ��ó�������
		virtual ISceneObject *getSceneObject()const;


		// ��ʾ���ڰڷŵ�ģ��
		virtual void showPlacingEntity(bool show);
		// �Ƿ���ʾ���ڰڷŵ�ģ��
		virtual bool isShowPlacingEntity() const;

		// ��ÿɼ��������ײʵ��
		virtual ICollisionEntity *getCollisionEntity()const;

		// ����ģ��ʵ��
		void createEntity();

		// ����ģ��ʵ��
		void destroyEntity();

		// �ؽ��ɼ�����
		virtual void _notifyRebuild()
		{
			createEntity();
		};

	protected:
		// ����
		BoxTriggerObject *m_object;
		// ������������
		IScene *m_scene;
		// �Ƿ���ʾ�߿�
		bool m_showBorder;
		// �Ƿ���ʾʵ��
		bool m_showEntity;
		// �Ƿ���ʾ���ڰڷŵ�ģ��
		bool m_showPlacingEntity;
		// ��ʾ��ģ��
		ManualObject *m_entity;
	};
};