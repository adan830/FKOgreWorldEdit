/******************************************************************
** �ļ���:	EntityRtt.h
** ������:	FreeKnight(������)
** ��  ��:	2008-1-13 11:53:28
** ��  ��:	ģ��ʵ��RTT
******************************************************************/

#pragma once
#include "EntityAnimationController.h"
namespace Ogre
{
	class EntityRTT;
	// RTT����ȾĿ������������ʼ���µ�ʱ�����ʾ��Ҫ��Ⱦ�Ķ�������Ⱦ��Ϻ�ȫ�����ص�
	class OGREPRO_API EntityRTTListener : public RenderTargetListener
	{
	public:
		EntityRTTListener(EntityRTT *rtt)
			: m_rtt(rtt)
		{

		}
		virtual void preRenderTargetUpdate(const RenderTargetEvent& evt);
		virtual void postRenderTargetUpdate(const RenderTargetEvent& evt);
	protected:
		EntityRTT *m_rtt;
	};

	// ģ��ʵ��RTT
	class OGREPRO_API EntityRTT : public FrameListener
	{
	public :
		friend class EntityRTTListener;
		//���������
		EntityRTT(const String &name);
		virtual ~EntityRTT();

		// ��ʼ��RTT
		virtual void init(SceneManager *sceneMgr
			, ushort textureWidth = 512
			, ushort textureHeight = 512
			, PixelFormat textureFormat = PF_R8G8B8A8
			, const ColourValue& bgColor = ColourValue::ZERO
			, float nearclipDist = 0.1f
			, float farclipDist = 100.0f
			, const Radian & fovy = Degree(60)
			, float ratio = 1.333f
			);
		// ����RTT
		virtual void destroy();

		// ����Ҫ��Ⱦ��ʵ��
		virtual void setEntity(Entity *entity , const String &defaultAnimationName = StringUtil::BLANK);
		// ���ʵ��
		virtual Entity *getEntity()const
		{
			return m_entity;
		}
		// ��ÿ�¡���ʵ��
		virtual Entity *getCloneEntity()const
		{
			return m_cloneEntity;
		}

		// ����RTT��Ⱦ
		virtual void setEnabled(bool enable);

		// �Ƿ��Ѿ�����RTT��Ⱦ
		virtual bool isEnabled()const;

		// ���RTT����
		virtual TexturePtr &getRTT();

		// ��ô�RTT������
		virtual const String &getName()const
		{
			return m_name;
		};

		// ������ת
		void rotateEntity(const Quaternion &rotate);
		// ������ת��
		void setEntityRotate(const Quaternion &rotate);
		// �����ת��
		const Quaternion &getEntityRotate()const;

		// ����RTT������Ⱦ����
		void setRatio(float ratio);

		// ���ô��ڴ�С������������С�Զ����������С�����������
		void setWindowSize(ushort width , ushort height);
		// ����RTT�����С
		void setTextureSize(ushort width , ushort height);
		// ÿ֡���¶���
		virtual bool frameStarted(const FrameEvent& evt);

		// ��ö����������ӿ�
		IEntityAnimationController *getEntityAnimationController()
		{
			return m_animationController;
		}
	protected:
		// ����RTT������ӿ�
		void createViewport();
		// �ӳٳ�ʼ��
		virtual void delayInit();
		// �����������ʵ����ǰ
		virtual void adjustCamera();
		// �����ӿ�
		virtual void destroyViewport();
		// ����ʵ��
		virtual void destroyEntity();
		// ���һ�����ظ���ʵ������
		virtual String getUniqueEntityName(const String &preName = StringUtil::BLANK);

		// ��RTT������
		String m_name;
		// �Ƿ��Ѿ���ʼ��
		bool m_inited;
		// �Ƿ��Ѿ�����RTT����
		bool m_createdTexture;
		// �Ƿ��Ѿ�setEntity����
		bool m_setedEntity;
		// �Ƿ�������Ⱦ
		bool m_enabled;

		// RTTʹ�õĳ���������
		SceneManager *m_sceneMgr;
		// RTT������
		ushort m_textureWidth;
		// RTT����߶�
		ushort m_textureHeight;
		// RTT�����ʽ
		PixelFormat m_textureFormat;
		// RTT���ӿڱ�����ɫ
		ColourValue m_bgColor;
		// RTT�����������
		float m_nearclipDist;
		// RTT�����Զ����
		float m_farclipDist;
		// RTT�����FOV�Ƕ�
		Radian m_fovy;
		// RTT�������߱�
		float m_ratio;

		// ��Ҫ��Ⱦ��Rttʵ��Ŀ�¡
		Entity *m_cloneEntity;
		// ��Ҫ��Ⱦ��Rttʵ��
		Entity *m_entity;
		// ʵ����ת
		Quaternion m_rotate;
		// ����������
		IEntityAnimationController *m_animationController;

		// RTT����
		TexturePtr m_rtt;
		// С�������ӿ�
		Viewport* m_viewport;
		// С�����������
		Camera* m_camera;
		// ��ȾĿ��
		RenderTarget *m_target;
		// RTT���ڵ�node
		SceneNode *m_node;
		// Entity���ڵ�node������m_node��
		SceneNode *m_nodeEntity;
		// RTTĿ���������
		RenderTargetListener *m_listener;
	};
};