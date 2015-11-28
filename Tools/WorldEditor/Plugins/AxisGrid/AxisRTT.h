
#pragma once

#include "EntityRTT.h"
namespace Ogre
{

	class AxisRTT : public EntityRTT
	{
	public:
		AxisRTT();
		virtual ~AxisRTT(void);

		virtual void init(IWorldEditor *worldEditor);
		virtual void destroy();

		// �ӿڸ���
		void preViewportUpdate(const RenderTargetViewportEvent& evt);

		const String &getAxisMaterialName()
		{
			return m_axisMat->getName();
		}
	protected:
		// �����������ʶ
		ManualObject* createAxis();
		// �������������
		void createAxisText();
		// �����������ʵ����ǰ
		virtual void adjustCamera();

		// ���������
		ManualObject *m_axisObject;
		BillboardSet *m_axisText;
		MaterialPtr m_axisMat;

		IWorldEditor *m_worldEditor;

	};

};