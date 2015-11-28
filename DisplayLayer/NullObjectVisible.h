#pragma once
#include "ISceneObjectVisible.h"
namespace Ogre
{
	class NullObject;
	class IScene;
	// ��������Ŀɼ����������ڱ༭����ѡ��
	class DISPLAYLAYER_API NullObjectVisible : public ISceneObjectVisible
	{
	public:
		NullObjectVisible(NullObject *obj);
		virtual ~NullObjectVisible(void);

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
		// ����һ��Բ��������SceneNode��
		virtual void createVisibleSphere();
		// ����Բ��
		virtual void destroyVisibleSphere();
		// ����
		NullObject *m_object;
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

		// Բ��������ʾ�ã�
		Entity *m_entitySphere;
		// ���ΰ뾶
		float m_sphereRadius;
		// ������ɫ
		ColourValue m_sphereColor;

	};
};