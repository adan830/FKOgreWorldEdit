#pragma once
#include "ICollisionShape.h"
//forward declarations
namespace Opcode
{
	class Model;
}

namespace Ogre
{
	// ��������ײģ��
	class OpcodeCollisionShape : public ICollisionShape
	{
	public:
		OpcodeCollisionShape(void){};
		virtual ~OpcodeCollisionShape(void){};

		// ������ײģ��
		virtual Opcode::Model* getCollisionModel(void) const = 0;

	};

};