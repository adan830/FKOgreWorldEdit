#pragma once
#include "OpcodeCollisionShape.h"

//forward declarations
namespace Opcode
{
	class Model;
	class MeshInterface;
}

namespace IceMaths
{
	class Matrix4x4;
	class Point;
}

namespace Ogre
{
	/** An abstract base class for a collision shape
	*/
	class OpcodeCollisionBoundingBoxShape : public OpcodeCollisionShape
	{
	public:

		/** 
		*/
		OpcodeCollisionBoundingBoxShape();

		/** 
		*/
		virtual ~OpcodeCollisionBoundingBoxShape();

		/** Initialize this shape using the mesh and scale provided
		*/
		bool init(const String &name , const AxisAlignedBox &box);

		// ������ײģ��
		virtual Opcode::Model* getCollisionModel(void) const { return mCollisionModel; }

		/** Return index array
		*/
		unsigned int* getIndexList() const { return mIndexList; }

		/** Return vertex array (in opcode Point format)
		*/
		IceMaths::Point* getVertexList() const { return mVertexList; }

		// �������
		virtual const String &getName()const
		{
			return m_name;
		}
		// �������
		virtual const String &getType()const
		{
			return SHAPE_TYPE;
		}

		// ����
		static String SHAPE_TYPE;


	protected:

		String m_name;
		Opcode::Model* mCollisionModel; // The collision model
		Opcode::MeshInterface* mMeshInterface;	// 
		unsigned int* mIndexList; // Index list for the indices
		IceMaths::Point* mVertexList; // Vertex list for the faces

		bool mAutoDeleteIndexList;
		bool mAutoDeleteVertexList;
	};
}