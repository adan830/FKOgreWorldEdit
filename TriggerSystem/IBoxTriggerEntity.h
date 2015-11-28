#pragma once
#include "ITriggerEntity.h"
namespace Ogre
{
	// ���Ӵ���������
	class IBoxTriggerEntity : public ITriggerEntity
	{
	public:

		IBoxTriggerEntity(void)
		{
		}

		virtual ~IBoxTriggerEntity(void)
		{
		}

		// ���ú���
		virtual void setBox(const AxisAlignedBox &box) = 0;
		// ��ú���
		virtual const AxisAlignedBox &getBox() = 0;
	};

};