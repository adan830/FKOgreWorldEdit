#pragma once
#include "NullObjectVisible.h"
namespace Ogre
{
	class EnvironmentObject;
	class IScene;
	// ��������Ŀɼ����������ڱ༭����ѡ��
	class DISPLAYLAYER_API EnvironmentObjectVisible : public NullObjectVisible
	{
	public:
		EnvironmentObjectVisible(EnvironmentObject *obj);
		virtual ~EnvironmentObjectVisible(void);


	};
};