#pragma once
#include "NullObjectVisible.h"
namespace Ogre
{
	class CameraObject;
	class IScene;
	// ��������Ŀɼ����������ڱ༭����ѡ��
	class DISPLAYLAYER_API CameraObjectVisible : public NullObjectVisible
	{
	public:
		CameraObjectVisible(CameraObject *obj);
		virtual ~CameraObjectVisible(void);

		// ��ʾ�߿�
		virtual void showBorder(bool show);

	};
};