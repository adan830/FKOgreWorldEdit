#pragma once

namespace Ogre
{

	// �����������
	class ObjectAction
	{
	public:
		ObjectAction(void);
		virtual ~ObjectAction(void);

		// ִ���������
		virtual void execute(){};
	};

};