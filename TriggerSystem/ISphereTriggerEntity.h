#pragma once
#include "ITriggerEntity.h"
namespace Ogre
{

	// ���崥��������
	class ISphereTriggerEntity : public ITriggerEntity
	{
	public:

		ISphereTriggerEntity(void)
		{
		}

		virtual ~ISphereTriggerEntity(void)
		{
		}

		// ������İ뾶
		virtual void setRadius(Real radius) = 0;
		// �����İ뾶
		virtual Real getRadius()const = 0;
	};

};