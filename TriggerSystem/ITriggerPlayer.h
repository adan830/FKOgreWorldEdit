#pragma once
#include "ITriggerEntity.h"

namespace Ogre
{

	// ���Խ��ܴ������¼������
	class ITriggerPlayer : public ITriggerEntity
	{
	public:
		ITriggerPlayer(void){};
		virtual ~ITriggerPlayer(void){};

		// ���´��������ݣ���������ᵼ��һ�α�����ѯ�����ص�
		virtual void updateTrigger() = 0;

		// ������ҵĺ���
		virtual void setBox(const AxisAlignedBox &box) = 0;
		// �����ҵĺ���
		virtual const AxisAlignedBox &getBox() = 0;

		// ��֮������ײ�Ĵ�����
		typedef std::map<String , ITriggerEntity*> MapCollideTriggerEntity;
		typedef VectorIterator<MapCollideTriggerEntity> CollideTriggerEntityIterator;
		// ���������֮������ײ�Ĵ�����
		virtual CollideTriggerEntityIterator getCollideTriggerEntityIterator() = 0;

	};

};