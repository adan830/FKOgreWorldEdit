#pragma once
#include "ISceneObjectVisible.h"
namespace Ogre
{
	class SphereTriggerObject;
	class IScene;
	// ��������Ŀɼ����������ڱ༭����ѡ��
	class DISPLAYLAYER_API SphereTriggerObjectVisible : public ISceneObjectVisible
	{
	public:
		SphereTriggerObjectVisible(SphereTriggerObject *obj);
		virtual ~SphereTriggerObjectVisible(void);

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
		SphereTriggerObject *m_object;
		// ������������
		IScene *m_scene;
		// �Ƿ���ʾ�߿�
		bool m_showBorder;
		// �Ƿ���ʾʵ��
		bool m_showEntity;
		// �Ƿ���ʾ���ڰڷŵ�ģ��
		bool m_showPlacingEntity;
		// ��ʾ��ģ��
		Entity *m_entity;
		// ģ�͵Ľڵ�
		SceneNode *m_entityNode;
	};
};