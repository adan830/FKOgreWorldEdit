#pragma once
#include <TListener.h>
namespace Ogre
{
	class ITriggerEntity;
	class ITriggerPlayer;
	// ������
	struct TriggerListener
	{
		// ������һ��������
		// @entity ������Ĵ�����
		// @player ��������
		virtual void onEnter(ITriggerEntity *entity , ITriggerPlayer *player){};

		// ��һ����������ȥ
		// @entity ��Ҵ������������ȥ
		// @player ��ȥ�����
		virtual void onLeave(ITriggerEntity *entity , ITriggerPlayer *player){};
	};

	class ITriggerWorld;
	class ITriggerPlayer;
	// ������ʵ��
	class TRIGGERSYSTEM_API ITriggerEntity
		: public TListener<TriggerListener>
	{
	public:
		ITriggerEntity(void){}
		virtual ~ITriggerEntity(void){}


		// �������
		virtual const String &getName()const = 0;
		// �������
		virtual const String &getType()const = 0;

		// ��ô���������
		virtual ITriggerWorld *getTriggerWorld()const = 0;

		// �����������
		virtual void setPosition(const Vector3 &pos) = 0;
		// ����������
		virtual const Vector3 &getPosition()const = 0;

		// �����������
		virtual void setScale(const Vector3 &scale) = 0;
		// ����������
		virtual const Vector3 &getScale()const = 0;


		// ������Է�λ
		virtual void setOrientation(const Quaternion &orientation) = 0;
		// ����������
		virtual const Quaternion &getOrientation()const = 0;

		// �Ƿ�����ҷ�������ײ
		virtual bool collide(ITriggerPlayer *player) = 0;

		// �����û�����
		virtual void setUserAny(const Any& anything) { m_userAny = anything; }
		// ����û�����
		virtual const Any& getUserAny(void) const { return m_userAny; }

	protected:
		// �û�����
		Any m_userAny;
	};

	// ��MovableObject::getUserAny��ֵת����ITriggerEntity *��ת��ʧ���򷵻�0
	inline ITriggerEntity *trigger_cast(const Any & operand)
	{
		ITriggerEntity ** obj = any_cast<ITriggerEntity*>((Any*)&operand);
		return obj ? *obj : 0;
	};
};