#pragma once

namespace Ogre
{
	// ��Ⱦ��Ļ�е�ѡ��
	class SelectionRectangle : public ManualObject
	{
	public:
		SelectionRectangle(const String &name);		
		// �����������µ����꣬0~1
		void setCorners(float left, float top, float right, float bottom);
		void setCorners(const Vector2 &topLeft, const Vector2 &bottomRight);
	};
};