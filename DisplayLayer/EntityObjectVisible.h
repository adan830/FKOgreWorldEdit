#pragma once
#include "ISceneObjectVisible.h"
namespace Ogre
{
	class EntityObject;
	class IScene;
	// ��������Ŀɼ����������ڱ༭����ѡ��
	class DISPLAYLAYER_API EntityObjectVisible : public ISceneObjectVisible
	{
	public:
		EntityObjectVisible(EntityObject *obj);
		virtual ~EntityObjectVisible(void);

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
		// �ؽ��ɼ�����
		virtual void _notifyRebuild(){};

	protected:
		void delayInit();
		// �߿�Ŀ�¡Ʒ
		Entity *m_entityBorder;
		// ����
		EntityObject *m_object;
		// ������������
		IScene *m_scene;
		// �Ƿ���ʾ�߿�
		bool m_showBorder;
		// �Ƿ���ʾʵ��
		bool m_showEntity;
		// �Ƿ���ʾ���ڰڷŵ�ģ��
		bool m_showPlacingEntity;
		// ͸��������
		EntityMaterialInstance *m_alphaController;
		// ��ײʵ��
		ICollisionEntity *m_collisionEntity;
	};
};